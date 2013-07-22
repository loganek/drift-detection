/*
 * detector.cpp
 *
 *  Created on: 20 lip 2013
 *      Author: loganek
 */

#include "detector.h"
#include <stdexcept>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

Detector::Detector()
{
	CalculateFeatureROI(Rect(160, 60, 540, 400));
}

void Detector::CalculateFeatureROI(const Rect& roi)
{
	featureROI = Mat(480, 854, CV_8UC1, Scalar(0));

	for (int i = roi.y; i < roi.y + roi.height - 30; i++)
		for (int j = roi.x; j < roi.x + roi.width; j++)
			featureROI.at<unsigned char>(i, j) = 255;
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
	static Size windowSize(100, 100);

	if (!CanProcess())
		throw runtime_error("Cannot run algorithm: not enough images.");

	vector<unsigned char> status;
	vector<float> err;

	calcOpticalFlowPyrLK(prevImage, currImage, features[0], features[1], status, err, windowSize, 5, termCriteria, 0, 0.001);
	debugInfo.featureStatus = status;
}

bool Detector::NeedFeatures()
{
	return features[0].size() < minFeatures;
}

void Detector::CalculateFeatures()
{
	static Size winSize(10, 10), minusOneSize(-1, -1);

	goodFeaturesToTrack(currImage, features[0], maxFeatures, 0.1, 50, featureROI, 3, false, 0.4);
	cornerSubPix(currImage, features[0], winSize, minusOneSize, termCriteria);
}
