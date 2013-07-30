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
	std::vector<unsigned char> featureStatus;
	std::vector<int> angles;
	DriftVector driftVector;
	PointList* route;
};

class Detector
{
private:
	cv::Mat prevImage;
	cv::Mat currImage;
	cv::Mat featureROI;

	DriftVector vect;
	std::vector<unsigned char> status;

	PointList route;

	FeatureList features;

	constexpr static int maxFeatures = 100;
	constexpr static size_t minFeatures = 5;
	const cv::TermCriteria termCriteria = cv::TermCriteria(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);

	bool NeedFeatures();
	bool CanProcess();
	void CalculateFeatures();
	void UpdateRoute();

	void CalculateFeatureROI(const cv::Rect& roi);

	DebugInfo debugInfo;

	AverageVectorComputer* avComputer;
public:
	Detector();
	virtual ~Detector();

	void PushImage(const cv::Mat& image);
	void Process();
	const DebugInfo& GetDebugInfo();
};

#endif /* DETECTOR_H_ */
