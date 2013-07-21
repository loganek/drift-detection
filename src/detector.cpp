/*
 * detector.cpp
 *
 *  Created on: 20 lip 2013
 *      Author: loganek
 */

#include "detector.h"
#include <stdexcept>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;

Detector::Detector()
{
}

Detector::~Detector()
{
}

const Detector::DebugInfo& Detector::GetDebugInfo()
{
	debugInfo.features = features;

	return debugInfo;
}

void Detector::PushImage(const Mat& image)
{
	currImage.copyTo(prevImage);
	image.copyTo(currImage);
	cvtColor(currImage, currImage, COLOR_BGR2GRAY);
}

bool Detector::CanProcess()
{
	return !currImage.empty() && !prevImage.empty();
}

void Detector::Process()
{
	if (!CanProcess())
		throw runtime_error("Cannot run algorithm: not enough images.");

	if (NeedFeatures())
		UpdateFeatures();
}

bool Detector::NeedFeatures()
{
	return true;
}

void Detector::UpdateFeatures()
{
	static Size winSize(10, 10), minusOneSize(-1, -1);

	goodFeaturesToTrack(currImage, features[1], maxFeatures, 0.1, 1, Mat(), 3, false, 0.4);
	cornerSubPix(currImage, features[1], winSize, minusOneSize, criteria);
}
