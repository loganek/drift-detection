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
	features[0].clear();

	return debugInfo;
}

void Detector::PushImage(const Mat& image)
{
	currImage.copyTo(prevImage);
	image.copyTo(currImage);
	cvtColor(currImage, currImage, COLOR_BGR2GRAY);

	if (NeedFeatures())
		CalculateFeatures();
}

bool Detector::CanProcess()
{
	return !currImage.empty() && !prevImage.empty();
}

void Detector::Process()
{
	static Size windowSize(31, 31);

	if (!CanProcess())
		throw runtime_error("Cannot run algorithm: not enough images.");

	vector<unsigned char> status;
	vector<float> err;

	calcOpticalFlowPyrLK(prevImage, currImage, features[0], features[1], status, err, windowSize, 3, termCriteria, 0, 0.01);
}

bool Detector::NeedFeatures()
{
	return features[0].size() < minFeatures;
}

void Detector::CalculateFeatures()
{
	static Size winSize(10, 10), minusOneSize(-1, -1);

	goodFeaturesToTrack(currImage, features[0], maxFeatures, 0.1, 10, Mat(), 3, false, 0.4);
	cornerSubPix(currImage, features[0], winSize, minusOneSize, termCriteria);
}
