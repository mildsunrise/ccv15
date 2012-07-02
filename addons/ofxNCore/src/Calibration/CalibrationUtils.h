/*
*  CalibrationUtils.h
*
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*  Anatoly Churikov: Adapted for multiple camera use 27.08.2011
*
*/

#ifndef CALIBRATIONUTILS_H
#define CALIBRATIONUTILS_H

//Used other calibration
#include "rect2d.h"
#include "ofxVec2f.h"
//#include "windows.h"
#include "memory.h"
#include "ofMain.h"

class CalibrationUtils
{
	public:
		CalibrationUtils();
		~CalibrationUtils();
		void setCalibrationProperties(unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,bool isInterleaveMode);
		virtual void beginCalibration();
		virtual void nextCalibrationStep();
		virtual void revertCalibrationStep();
		void nextCameraCalibration();
		ofxVec2f* screenPoints;
		ofxVec2f* cameraPoints;
		ofxVec2f* drawingPoints;
		bool bCalibrating;
		bool bGoToNextStep;
		bool bNextCamera;
		int  calibrationStep;
		int  GRID_POINTS;
		int  GRID_X;
		int  GRID_Y;
		unsigned int screenWidth,screenHeight;
		unsigned int selectedCameraIndex;
	private:
		int GRID_INDICES;
		unsigned int cameraGridWidth,cameraGridHeight;
		bool bscreenPoints;
		bool bcameraPoints;
		bool isInterleaveMode;
		void setCalibrationCameraIndex(unsigned int cameraIndex);
		void initScreenPoints(int cameraPosition);
		void initCameraPoints();
};
#endif
