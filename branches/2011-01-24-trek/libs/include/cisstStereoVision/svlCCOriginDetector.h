/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: cameraCalibration.cpp 2426 2011-05-21 00:53:58Z wliu25 $

  Author(s):  Wen P. Liu
  Created on: 2011

  (C) Copyright 2006-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/
#ifndef _svlCCOriginDetector_h
#define _svlCCOriginDetector_h
#include <highgui.h>
#include <cv.h>
#include <math.h>
#include <iostream>
//there aren't directives in OpenCV v.5, but they exist in OpenCV v.4
#undef CV_MIN
#undef CV_MAX
#define  CV_MIN(a, b)   ((a) <= (b) ? (a) : (b)) 
#define  CV_MAX(a, b)   ((a) >= (b) ? (a) : (b))

class svlCCOriginDetector
{
	public:
		enum originDetectionEnum { NO_ORIGIN, COLOR};
		enum colorIndexEnum {RED_INDEX, GREEN_INDEX, BLUE_INDEX, YELLOW_INDEX};
		enum colorModeEnum {RGB, RGY};

		svlCCOriginDetector(int colorModeFlag);
		void detectOrigin(IplImage* iplImage);
		int getOriginDetectionFlag(){return originDetectionFlag;};
		int getOriginColorModeFlag(){return originColorModeFlag;};
		std::vector<cv::Point2f> getColorBlobs() { return colorBlobs;};
		cv::Point2f getOrigin() { return origin;};
	
	private:
		void reset();
		void findOriginByColor( IplImage* img); 
		bool findColorBlobs(IplImage* iplImage, float radius, int flags[],int thresholds[]);
		
		void drawColorBlobs(IplImage* iplImage);
		float distanceBetweenTwoPoints ( float x1, float y1, float x2, float y2);
		cv::Point2f intersectionByColorBlobs(float point_x, float point_y, float line_x1, float line_y1, float line_x2, float line_y2);
	
		////////// Parameters //////////
		const static bool debug = false;
		int originDetectionFlag;
		int originColorModeFlag;
		std::vector<cv::Point2f> colorBlobs;
		cv::Point2f origin;

};
#endif