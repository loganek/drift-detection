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
#include <cmath>

using namespace cv;
using namespace std;

Detector::Detector()
{
	debugInfo.route = &route;
	CalculateFeatureROI(Rect(160, 60, 540, 400));
	avComputer = new AverageVectorComputer(debugInfo, status, features);
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
	delete avComputer;
}

const DebugInfo& Detector::GetDebugInfo()
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
	static Size windowSize(21, 21);

	if (!CanProcess())
		throw runtime_error("Cannot run algorithm: not enough images.");

	vector<float> err;

	Mat pyramid = prevImage;

	calcOpticalFlowPyrLK(pyramid, currImage, features[0], features[1], status, err, windowSize, 5, termCriteria, 0, 0.001);

	debugInfo.featureStatus = status;
	vect = avComputer->ComputeAverageVector();

	UpdateRoute();
}

void Detector::UpdateRoute()
{
	float angle = vect.angle * M_PI / 180;

	float x = vect.length * cos(angle);
	float y = vect.length * sin(angle);

	route.push_back(cv::Point2f(x, y));
}

bool Detector::NeedFeatures()
{
	return features[0].size() < minFeatures;
}

void Detector::CalculateFeatures()
{
	static Size winSize(10, 10), minusOneSize(-1, -1);

	goodFeaturesToTrack(currImage, features[0], maxFeatures, 0.03, 50, featureROI, 3, false, 0.4);
	cornerSubPix(currImage, features[0], winSize, minusOneSize, termCriteria);
}
