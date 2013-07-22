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
	static Size windowSize(21, 21);

	if (!CanProcess())
		throw runtime_error("Cannot run algorithm: not enough images.");

	vector<float> err;

	Mat pyramid = prevImage;

	calcOpticalFlowPyrLK(pyramid, currImage, features[0], features[1], status, err, windowSize, 5, termCriteria, 0, 0.001);

	CalculateAngles();
	RemoveStrangePoints();

	debugInfo.featureStatus = status;
}

// TODO: split CalculateAngles and RemoveStrangePoints together (in one loop and 'if' statement)

void Detector::CalculateAngles()
{
	angles = vector<int>(status.size(), -1);

	for (size_t i = 0; i < status.size(); i++)
	{
		if (status[i])
			angles[i] = int(atan2(features[1][i].y - features[0][i].y, features[1][i].x - features[0][i].x) * 180 / M_PI);
	}

	debugInfo.angles = angles;
}

void Detector::RemoveStrangePoints()
{
	int range = 5;
	int commonAngle = GetMostCommonAngle(range);

	for (size_t i = 0; i < angles.size(); i++)
	{
		if (status[i] && (angles[i] < commonAngle - range || angles[i] > commonAngle + range))
			status[i] = 0;
	}
}

Detector::AnglesHistogram Detector::BuildHistogram()
{
	AnglesHistogram histogram;
	histogram.fill(0);

	for (auto angle : angles)
	{
		if (angle >= 0)
			histogram[angle]++;
	}

	return histogram;
}

int Detector::GetMostCommonAngle(int range)
{
	AnglesHistogram newHistogram, histogram;

	histogram = BuildHistogram();

	// TODO I'm sure might be done better (more effective)

	newHistogram.fill(0);
	newHistogram[359] = std::accumulate(histogram.end() - range, histogram.end(), 0, std::plus<unsigned char>()) +
			std::accumulate(histogram.begin(), histogram.begin() + range, 0, std::plus<unsigned char>());

	int max = 0, maxIt = 0;

	for (size_t i = 0; i < 360; i++)
	{
		// TODO prove instruction shown below!
		newHistogram[i] = newHistogram[i < 1 ? 360 - i - 1 : i-1] + histogram[(i+range-1)%360] - histogram[i < range ? 360 - (range - i + 1) : i-range];
		if (newHistogram[i] > max)
		{
			max = newHistogram[i];
			maxIt = i;
		}
	}

	return maxIt;
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
