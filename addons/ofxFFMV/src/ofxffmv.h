/* ofxffmv.h
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFXFFMV_H_INCLUDED
#define OFXFFMV_H_INCLUDED

#include "ofxCameraBase.h"
#include "FlyCapture2GUI.h"
#include "FlyCapture2.h"
#include "ofxXmlSettings.h"

class ofxffmv : ofxCameraBase
{
public:
    ofxffmv();
	~ofxffmv();
	void setCameraFeature(CAMERA_BASE_FEATURE featureCode, int firstValue, int secondValue,bool isAuto,bool onOff);
	void getCameraFeature(CAMERA_BASE_FEATURE featureCode, int* firstValue, int* secondValue, bool* isAuto, bool* onOff, int* minValue, int* maxValue);
	unsigned int getCameraBaseCount();
	GUID* getBaseCameraGuids(unsigned int* camCount);
	CAMERA_BASE_FEATURE* getSupportedFeatures(unsigned int* featuresCount);
	void callSettingsDialog();
protected:
	void getNewFrame(unsigned char* newFrame);
	void cameraInitializationLogic();
	void cameraDeinitializationLogic();
	void setCameraType();
private:
    FlyCapture2::Image image;
	FlyCapture2::Camera camera;
    FlyCapture2::CameraInfo  info;
    FlyCapture2::CameraControlDlg settingsDlg;
};

void ofxToPgrGuid(const GUID& origin, FlyCapture2::PGRGuid& dest);
void ofxFromPgrGuid(const FlyCapture2::PGRGuid& origin, GUID& dest);

#endif // OFXFFMV_H_INCLUDED
