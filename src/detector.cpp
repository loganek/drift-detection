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
	calculate_feature_roi(Rect(160, 60, 540, 400));
	av_computer = new AverageVectorComputer(debugInfo, status, features);
}

void Detector::calculate_feature_roi(const Rect& roi)
{
	feature_roi = Mat(480, 854, CV_8UC1, Scalar(0));

	for (int i = roi.y; i < roi.y + roi.height - 30; i++)
		for (int j = roi.x; j < roi.x + roi.width; j++)
			feature_roi.at<unsigned char>(i, j) = 255;
}

Detector::~Detector()
{
	delete av_computer;
}

const DebugInfo& Detector::get_debug_info()
{
	debugInfo.features = features;

	return debugInfo;
}

void Detector::push_image(const Mat& image)
{
	curr_image.copyTo(prev_image);
	image.copyTo(curr_image);
	cvtColor(curr_image, curr_image, COLOR_BGR2GRAY);

	if (need_features())
		calculate_features();
}

bool Detector::can_process()
{
	return !curr_image.empty() && !prev_image.empty();
}

void Detector::process()
{
	static Size windowSize(21, 21);

	if (!can_process())
		throw runtime_error("Cannot run algorithm: not enough images.");

	vector<float> err;

	calcOpticalFlowPyrLK(prev_image, curr_image, features[0], features[1], status, err, windowSize, 5, term_criteria, 0, 0.001);

	debugInfo.feature_status = status;
	vect = av_computer->ComputeAverageVector();

	update_route();
}

void Detector::update_route()
{
	float angle = vect.angle * M_PI / 180;

	float x = vect.length * cos(angle);
	float y = vect.length * sin(angle);

	route.push_back(cv::Point2f(x, y));
}

bool Detector::need_features()
{
	return features[0].size() < min_features;
}

void Detector::calculate_features()
{
	static Size winSize(10, 10), minusOneSize(-1, -1);

	goodFeaturesToTrack(curr_image, features[0], max_features, 0.03, 50, feature_roi, 3, false, 0.4);
	cornerSubPix(curr_image, features[0], winSize, minusOneSize, term_criteria);
}
