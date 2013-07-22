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
	typedef std::vector<cv::Point2f> PointList;
	typedef std::array<PointList, 2> FeatureList;
	typedef std::array<unsigned char, 360> AnglesHistogram;

	struct DebugInfo
	{
		Detector::FeatureList features;
		std::vector<unsigned char> featureStatus;
		std::vector<int> angles;
	};
private:
	cv::Mat prevImage;
	cv::Mat currImage;
	cv::Mat featureROI;

	std::vector<unsigned char> status;
	std::vector<int> angles;

	FeatureList features;

	constexpr static int maxFeatures = 50;
	constexpr static size_t minFeatures = 5;
	const cv::TermCriteria termCriteria = cv::TermCriteria(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);

	bool NeedFeatures();
	bool CanProcess();
	void CalculateFeatures();
	void CalculateAngles();
	AnglesHistogram BuildHistogram();
	int GetMostCommonAngle(int range);

	void RemoveStrangePoints();

	void CalculateFeatureROI(const cv::Rect& roi);

	DebugInfo debugInfo;
public:
	Detector();
	virtual ~Detector();

	void PushImage(const cv::Mat& image);
	void Process();
	const DebugInfo& GetDebugInfo();
};

#endif /* DETECTOR_H_ */
