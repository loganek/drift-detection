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

class Detector
{
public:
	typedef std::array<std::vector<cv::Point2f>, 2> FeatureList;

	struct DebugInfo
	{
		Detector::FeatureList features;
	};
private:
	cv::Mat prevImage;
	cv::Mat currImage;

	FeatureList features;

	constexpr static int maxFeatures = 100;
	constexpr static size_t minFeatures = 5;
	const cv::TermCriteria termCriteria = cv::TermCriteria(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);

	bool NeedFeatures();
	bool CanProcess();
	void CalculateFeatures();

	DebugInfo debugInfo;
public:
	Detector();
	virtual ~Detector();

	void PushImage(const cv::Mat& image);
	void Process();
	const DebugInfo& GetDebugInfo();
};

#endif /* DETECTOR_H_ */
