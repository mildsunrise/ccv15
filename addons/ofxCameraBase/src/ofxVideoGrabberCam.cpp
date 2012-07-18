#include "ofxVideoGrabberCam.h"

ofxVideoGrabberCam::ofxVideoGrabberCam() {
}

ofxVideoGrabberCam::~ofxVideoGrabberCam() {
}

void ofxVideoGrabberCam::getNewFrame(unsigned char * newFrame) {
    grabber.grabFrame();
    unsigned char * frame = grabber.getPixels();
    if (depth == 1) {
        //Do a fast average on each pixel
        for (int y=0; y<grabber.height; y++) {
            for (int x=0; x<grabber.width; x++) {
                int idx = y*grabber.width + x;
                int orIdx = idx*3;

                unsigned char r = frame[orIdx  ];
                unsigned char g = frame[orIdx+1];
                unsigned char b = frame[orIdx+2];

                unsigned char value = (r+g+b)/3;
                newFrame[idx] = value;
            }
        }
    } else {
        memcpy(newFrame, frame, grabber.width*grabber.height*3);
    }
}

void ofxVideoGrabberCam::cameraInitializationLogic() {
    int devId = guid.Data1;
    grabber.setDeviceID(devId);

    if (depth != 1 && depth != 3) depth = 1; //Fallback to monochrome
    grabber.initGrabber(width, height); //FIXME: check status

    //grabber.grabFrame();
    //width = grabber.width
    //height = grabber.height

    for (unsigned int i=0;i<cameraBaseSettings->propertyType.size();i++)
		setCameraFeature(cameraBaseSettings->propertyType[i],cameraBaseSettings->propertyFirstValue[i],cameraBaseSettings->propertySecondValue[i],cameraBaseSettings->isPropertyAuto[i],cameraBaseSettings->isPropertyOn[i]);
}

void ofxVideoGrabberCam::cameraDeinitializationLogic() {
    grabber.close();
}

void ofxVideoGrabberCam::callSettingsDialog() {
    grabber.videoSettings();
}

unsigned int ofxVideoGrabberCam::getCameraBaseCount() {
    return 0; //not possible for now, see github.com/openframeworks/openFrameworks/issues/461
}
GUID* ofxVideoGrabberCam::getBaseCameraGuids(unsigned int* camCount) {
    *camCount = 0;
    return NULL; //not possible for now, see github.com/openframeworks/openFrameworks/issues/461
}

void ofxVideoGrabberCam::setCameraFeature(CAMERA_BASE_FEATURE featureCode, int firstValue, int secondValue,bool isAuto,bool onOff) {
    //Support for controls is not provided.
}
void ofxVideoGrabberCam::getCameraFeature(CAMERA_BASE_FEATURE featureCode, int* firstValue, int* secondValue, bool* isAuto, bool* onOff, int* minValue, int* maxValue) {
    //Support for controls is not provided.
}

CAMERA_BASE_FEATURE* ofxVideoGrabberCam::getSupportedFeatures(unsigned int* featuresCount) {
    //Support for controls is not provided.
    *featuresCount = 0;
    return NULL;
}

void ofxVideoGrabberCam::setCameraType() {
	cameraType = VIDEOGRAB;
	cameraTypeName = "VIDEOGRAB";
}
