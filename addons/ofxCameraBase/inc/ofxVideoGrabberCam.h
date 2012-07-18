/*
*  ofxVideoGrabberCam.h
*
*  NOTE: This is a PROVISIONAL bridge to the generic,
*  very basic VideoGrabber of OpenFrameworks.
*  We should remove this when specific frameworks
*  are available for other platforms.
*
*  Created on 18/07/12.
*  Copyright 2012 NUI Group. All rights reserved.
*  Author: Xavier Mendez
*
*/
#ifndef OFX_VIDEOGRABBERCAM_H
#define OFX_VIDEOGRABBERCAM_H

#include "video/ofVideoGrabber.h"
#include "ofxCameraBase.h"

class ofxVideoGrabberCam: public ofxCameraBase {
public:
    ofxVideoGrabberCam();
	~ofxVideoGrabberCam();
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
    ofVideoGrabber grabber;
};

#endif // OFX_VIDEOGRABBERCAM_H
