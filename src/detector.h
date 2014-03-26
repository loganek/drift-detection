/*
 * detector.h
 *
 *  Created on: 20 lip 2013
 *      Author: loganek
 */

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <opencv2/core/mat.hpp>
#include <vector>
#include <array>
#include "types.h"
#include "average_vector_computer.h"
#include "drift_vector.h"

struct DebugInfo
{
	FeatureList features;
	std::vector<unsigned char> feature_status;
	std::vector<int> angles;
	DriftVector drift_vector;
	PointList* route;
};

class Detector
{
private:
	cv::Mat prev_image;
	cv::Mat curr_image;
	cv::Mat feature_roi;

	DriftVector vect;
	std::vector<unsigned char> status;

	PointList route;

	FeatureList features;

	constexpr static int max_features = 100;
	constexpr static size_t min_features = 5;
	const cv::TermCriteria term_criteria = cv::TermCriteria(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);

	bool need_features();
	bool can_process();
	void calculate_features();
	void update_route();

	void calculate_feature_roi(const cv::Rect& roi);

	DebugInfo debugInfo;

	AverageVectorComputer* av_computer;
public:
	Detector();
	virtual ~Detector();

	void push_image(const cv::Mat& image);
	void process();
	const DebugInfo& get_debug_info();
};

#endif /* DETECTOR_H_ */
