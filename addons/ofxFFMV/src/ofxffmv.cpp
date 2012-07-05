/*
*  ofxffmv.cpp
*
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#include "ofxffmv.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#pragma warning(disable : 4018)	// signed/unsigned mismatch

//GUID converting stuff
void ofxToPgrGuid(const GUID& origin, FlyCapture2::PGRGuid& dest) {
    dest.value[0] = origin.Data1;

    dest.value[1] = (((origin.Data2)<<16) | ((origin.Data3) & 0xffff)); //TODO MIGRATION: is this endian-correct?

    unsigned int v = 0;
    v = (v << 8) | origin.Data4[0];
    v = (v << 8) | origin.Data4[1];
    v = (v << 8) | origin.Data4[2];
    v = (v << 8) | origin.Data4[3];
    dest.value[2] = v;

    v = 0;
    v = (v << 8) | origin.Data4[4];
    v = (v << 8) | origin.Data4[5];
    v = (v << 8) | origin.Data4[6];
    v = (v << 8) | origin.Data4[7];
    dest.value[3] = v;
}

void ofxFromPgrGuid(const FlyCapture2::PGRGuid& origin, GUID& dest) {
    dest.Data1 = origin.value[0];

    unsigned int bint = origin.value[1];
    dest.Data2 = (bint & 0xffff0000) >> 16;
    dest.Data3 = (bint & 0x0000ffff)      ;

    bint = origin.value[2];
    dest.Data4[0] = (bint & 0xff000000) >> 24;
    dest.Data4[1] = (bint & 0x00ff0000) >> 16;
    dest.Data4[2] = (bint & 0x0000ff00) >> 8 ;
    dest.Data4[3] = (bint & 0x000000ff)      ;

    bint = origin.value[3];
    dest.Data4[4] = (bint & 0xff000000) >> 24;
    dest.Data4[5] = (bint & 0x00ff0000) >> 16;
    dest.Data4[6] = (bint & 0x0000ff00) >> 8 ;
    dest.Data4[7] = (bint & 0x000000ff)      ;
}

ofxffmv::ofxffmv() {
}

ofxffmv::~ofxffmv() {
	deinitializeCamera();
}

void ofxffmv::callSettingsDialog() {
	settingsDlg.Show();
}

CAMERA_BASE_FEATURE* ofxffmv::getSupportedFeatures(unsigned int* featuresCount) {
	*featuresCount = 7;
	CAMERA_BASE_FEATURE* features = (CAMERA_BASE_FEATURE*)malloc(*featuresCount * sizeof(CAMERA_BASE_FEATURE));
	features[0] = BASE_BRIGHTNESS;
	features[1] = BASE_EXPOSURE;
	features[2] = BASE_SATURATION;
	features[3] = BASE_GAMMA;
	features[4] = BASE_SHUTTER;
	features[5] = BASE_GAIN;
	features[6] = BASE_FRAMERATE;
	return features;
}

void ofxffmv::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool onOff) {
	FlyCapture2::PropertyType deviceProperty;
	      if (featureCode == BASE_BRIGHTNESS)
		deviceProperty = FlyCapture2::BRIGHTNESS;
	else if (featureCode == BASE_EXPOSURE)
		deviceProperty = FlyCapture2::AUTO_EXPOSURE;
	else if (featureCode == BASE_SATURATION)
		deviceProperty = FlyCapture2::SATURATION;
	else if (featureCode == BASE_GAMMA)
		deviceProperty = FlyCapture2::GAMMA;
	else if (featureCode == BASE_SHUTTER)
		deviceProperty = FlyCapture2::SHUTTER;
	else if (featureCode == BASE_GAIN)
		deviceProperty = FlyCapture2::GAIN;
	else if (featureCode == BASE_FRAMERATE)
		deviceProperty = FlyCapture2::FRAME_RATE;
	else return;

    FlyCapture2::Property prop (deviceProperty);
    prop.valueA = firstValue;
    prop.valueB = secondValue;
    prop.onOff = onOff;
    prop.autoManualMode = isAuto;
    camera.SetProperty(&prop);
    //FIXME MIGRATION: should we broadcast? should auto be negated?
}

void ofxffmv::getCameraFeature(CAMERA_BASE_FEATURE featureCode, int* firstValue, int* secondValue, bool* isAuto, bool* onOff, int* minValue, int* maxValue) {
	*onOff = false;
	*firstValue = 0;
	*secondValue = 0;
	*isAuto = false;
	*minValue = 0;
	*maxValue = 0;

	FlyCapture2::PropertyType deviceProperty;
	      if (featureCode == BASE_BRIGHTNESS)
		deviceProperty = FlyCapture2::BRIGHTNESS;
	else if (featureCode == BASE_EXPOSURE)
		deviceProperty = FlyCapture2::AUTO_EXPOSURE;
	else if (featureCode == BASE_SATURATION)
		deviceProperty = FlyCapture2::SATURATION;
	else if (featureCode == BASE_GAMMA)
		deviceProperty = FlyCapture2::GAMMA;
	else if (featureCode == BASE_SHUTTER)
		deviceProperty = FlyCapture2::SHUTTER;
	else if (featureCode == BASE_GAIN)
		deviceProperty = FlyCapture2::GAIN;
	else if (featureCode == BASE_FRAMERATE)
		deviceProperty = FlyCapture2::FRAME_RATE;
	else return;

    FlyCapture2::Property prop (deviceProperty);
    camera.GetProperty(&prop);

    *firstValue = prop.valueA;
    *secondValue = prop.valueB;
    *onOff = prop.onOff;
    *isAuto = prop.autoManualMode;

    FlyCapture2::PropertyInfo pinfo (deviceProperty);
    camera.GetPropertyInfo(&pinfo);

    *minValue = pinfo.min;
    *maxValue = pinfo.max;
}

unsigned int ofxffmv::getCameraBaseCount() {
    FlyCapture2::BusManager bus;
    unsigned int ret;
    bus.GetNumOfCameras(&ret);
    return ret;
}

void ofxffmv::getNewFrame(unsigned char* newFrame) {
	camera.RetrieveBuffer(&image);//FIXME MIGRATION: does RetrieveBuffer convert to current settings?
	memcpy((void*)newFrame,(void*)image.GetData(),image.GetDataSize());
}

void ofxffmv::setCameraType()
{
	cameraType = FFMV;
	cameraTypeName = "FFMV";
}

void ofxffmv::cameraInitializationLogic() {
    //Connect to the camera
    FlyCapture2::PGRGuid tg;
    ofxToPgrGuid(guid, tg);
	camera.Connect(&tg);
	settingsDlg.Connect(&camera);

    //Retrieve info, set properties
    camera.GetCameraInfo(&info);
    //FIXME MIGRATION: support top and left
    if (top || left) std::cout << "WARNING: The top and left values are being ignored!" << std::endl; //FIXME MIGRATION: what's that 100? stride? what's depth?
	//flycaptureStartCustomImage(cameraContext,(unsigned int)cameraPixelMode,left,top,width,height,100,depth == 1 ? (info.CameraType == FLYCAPTURE_COLOR ? FLYCAPTURE_RAW8 :  FLYCAPTURE_MONO8) : FLYCAPTURE_RGB8);
	image.SetDimensions(height, width, image.GetStride(), depth == 1 ? (info.isColorCamera ? FlyCapture2::PIXEL_FORMAT_RAW8 :  FlyCapture2::PIXEL_FORMAT_MONO8) : FlyCapture2::PIXEL_FORMAT_RGB8, FlyCapture2::NONE);

    //Start capture, retrieve dimensions
    camera.StartCapture();
    camera.RetrieveBuffer( &image );
	width = image.GetCols();
	height = image.GetRows();
	//depth = (width*height!=0) ? image.iRowInc/image.GetRows() : 0; TODO MIGRATION
	depth = 1;

	//Set final properties
	image.SetColorProcessing(depth == 1 ? FlyCapture2::NO_COLOR_PROCESSING : FlyCapture2::NEAREST_NEIGHBOR);
	for (unsigned int i=0;i<cameraBaseSettings->propertyType.size();i++)
		setCameraFeature(cameraBaseSettings->propertyType[i],cameraBaseSettings->propertyFirstValue[i],cameraBaseSettings->propertySecondValue[i],cameraBaseSettings->isPropertyAuto[i],cameraBaseSettings->isPropertyOn[i]);
}

void ofxffmv::cameraDeinitializationLogic() {
    camera.StopCapture();
    settingsDlg.Disconnect();
    camera.Disconnect();
}

GUID* ofxffmv::getBaseCameraGuids(unsigned int* camCount) {
    FlyCapture2::BusManager bus;
    bus.GetNumOfCameras(camCount);
	GUID* guids = (GUID*)malloc((*camCount)*sizeof(GUID));
    FlyCapture2::PGRGuid guid;
	for (unsigned int i=0;i<(*camCount);i++) {
        bus.GetCameraFromIndex(i, &guid);
		ofxFromPgrGuid(guid, guids[i]);
	}
	return guids;
}
