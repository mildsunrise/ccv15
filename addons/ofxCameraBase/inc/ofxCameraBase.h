/*
*  ofxCameraBase.h
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFX_CAMERABASE_H
#define OFX_CAMERABASE_H

#define _MAX_CAMERAS_ 16
#define _MAX_FRAME_LIFETIME_ 3

//#include <windows.h>
#include "ofxCameraBaseSettings.h"
#include "ofMain.h"
#include "ofxGUIDHelper.h"
#include "ofxXmlSettings.h"
#include "ofxThread.h"

//forward declarations
class ofxCameraBase;

//capturing thread logic
class ofxCameraCaptureThread: public ofxThread {
public:
    ofxCameraCaptureThread(ofxCameraBase& parent): parent_(parent) {
        isPaused=false;
    }
    ~ofxCameraCaptureThread() {};
    void threadedFunction();
    friend class ofxCameraBase;
private:
    ofxCameraBase& parent_; //It would be dangerous if this was "protected"
protected:
    bool isPaused;
};

class ofxCameraBase {
public:
	//Constructor for CameraBase class
	ofxCameraBase() : captureThread(ofxCameraCaptureThread(*this)) { //nasty thing here
		depth = 1;
		width = 640;
		height = 480;
		left = 0;
		top = 0;
		framerate = 30;
		isInitialized = false;
		index = 0;
		memset((void*)&guid,0,sizeof(GUID));
		isUsedForTracking = false;
		isNewFrame = true;
		newFrameCurrentLifetime = 0;
		cameraPixelMode = 0;
		cameraBaseSettings = NULL;
		rawCameraFrame = NULL;
		isRaw = 0;
	}
	//Virtual destructor for CameraBase class
	virtual ~ofxCameraBase(){}
	//Initialization procedure that get
	//GUID cameraGuid - global idetifier of camera. PS3 Eye uses all GUID data structure, but FFMV, CMU and etc. uses only 64 bit for global identifier. For these cameras use first 64 bits of GUID (Data1)
	void initializeWithGUID(GUID cameraGuid);
	//Deinitialization of CameraBase class
	void deinitializeCamera();
	//setting base feature to CameraBase class
	virtual void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue, int secondValue,bool isAuto,bool isEnabled){}
	//get current value of feature with min and max value
	virtual void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue, int* secondValue, bool* isAuto, bool* isEnabled, int* minValue, int* maxValue) {}
	//get number of connected cameras
	virtual unsigned int getCameraBaseCount() {return 0;}
	//public getter of camera frame size and depth
	void getCameraSize(unsigned int* cameraWidth,unsigned int* cameraHeight,unsigned char* cameraDepth,unsigned char* pixelMode);
	//public getter of cameraFrame
	void getCameraFrame(unsigned char* newFrameData);
	//public getter of camera index position
	unsigned int getCameraIndex(){return index;}
	//public getter of camera global identifier
	GUID getCameraGUID(){return guid;}
	//public getter of camera initialized status
	bool isCameraInitialized() { return isInitialized; }
	//public getter of camera isNewFrame property
	bool isCapturedNewFrame() { return isNewFrame;}
	//start camera logic
	void startCamera();
	//pause camera active capturing when it's not in use
	void pauseCamera() { captureThread.isPaused = true; }
	//fast resume capturing from camera
	void resumeCamera() { captureThread.isPaused = false; }
	//check is camera capturing is paused
	bool isCameraPaused() { return captureThread.isPaused;}
	//get camera Type
	CAMERATYPE getBaseCameraType() { return cameraType; }
	//get camera type name
	std::string getBaseCameraTypeName() { return cameraTypeName;}
	//get camera GUID
	GUID getBaseCameraGuid() { return guid;}
	//get camera GUIDs
	virtual GUID* getBaseCameraGuids(int* camCount) { return NULL; }
	//get camera supported features
	virtual CAMERA_BASE_FEATURE* getSupportedFeatures(unsigned int* featuresCount) { *featuresCount = 0; return NULL; }
	//reading camera settings from XML
	void loadCameraSettings();
	//saving camera settings to XML
	void saveCameraSettings();
	//start settings dialog
	virtual void callSettingsDialog() { };
protected:
	//logic for updating current frame
	void updateCurrentFrame();
	//capturing thread start
	void StartThreadingCapture();
	//capturing thread stop
	void StopThreadingCapture();
	//specific logic for getting frame from each camera
	virtual void getNewFrame(unsigned char* newFrame) {}
	//specific logic for initialization camera
	virtual void cameraInitializationLogic() {}
	//specific logic for deinitialization camera
	virtual void cameraDeinitializationLogic() {}
	//setting camera type
	virtual void setCameraType() {}
	void loadDefaultCameraSettings();
private:
	ofxCameraCaptureThread captureThread; //It would be dangerous if this was "protected"
	void receiveSettingsFromCamera();
	void loadCameraSettings(ofxXmlSettings* xmlSettings);
	friend class ofxCameraCaptureThread;
protected:
	GUID guid;
	std::string cameraTypeName;
	CAMERATYPE cameraType;
	unsigned int width,height,framerate;
	int isRaw;
	unsigned int index,left,top;
	unsigned char depth;
	bool isInitialized,isNewFrame,isUsedForTracking;
	int newFrameCurrentLifetime;
	unsigned char cameraPixelMode;
	unsigned char* cameraFrame;
	unsigned char* rawCameraFrame;
	ofxCameraBaseSettings* cameraBaseSettings;
};

#endif // OFX_CAMERABASE_H
