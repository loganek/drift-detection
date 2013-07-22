/*
 * debug_image_generator.h
 *
 *  Created on: 21 lip 2013
 *      Author: loganek
 */

#ifndef DEBUGIMAGEGENERATOR_H_
#define DEBUGIMAGEGENERATOR_H_

#include "detector.h"
#include <tuple>

#define _USE_MATH_DEFINES
#include <cmath>

struct FeatureListDebugOperator
{
	static void Process(cv::Mat& image, const Detector::DebugInfo& debugInfo)
	{
		for (auto center : debugInfo.features[0])
					cv::circle(image, center, 3, cv::Scalar(0, 255, 0));

		for (auto center : debugInfo.features[1])
			cv::circle(image, center, 3, cv::Scalar(0, 255, 255));
	}
};

struct ArrowFlowDebugOperator
{
private:

	static void DrawArrow(cv::Mat& image, cv::Point2f second, cv::Point2f first)
	{
		cv::Scalar color(0, 255, 0);
		cv::Point2f p(first.x, first.y),
				q(second.x, second.y);
		double angle = atan2(p.y - q.y, p.x - q.x);

		cv::line( image, p, q, color);

		p.x = q.x + 9 * cos(angle + M_PI_4);
		p.y = q.y + 9 * sin(angle + M_PI_4);
		cv::line( image, p, q, color);

		p.x = q.x + 9 * cos(angle - M_PI_4);
		p.y = q.y + 9 * sin(angle - M_PI_4);
		cv::line( image, p, q, color);
	}

public:
	static void Process(cv::Mat& image, const Detector::DebugInfo& debugInfo)
	{
		for (size_t i = 0; i < debugInfo.featureStatus.size(); i++)
		{
			if (debugInfo.featureStatus[i])
			{
				DrawArrow(image, debugInfo.features[0][i], debugInfo.features[1][i]);
			}
		}
	}
};

template<typename... Op>
class DebugImageGenerator
{
private:
	template<std::size_t I = 0, typename... Tp>
	inline typename std::enable_if<I == sizeof...(Tp), void>::type
	Process(std::tuple<Tp...>& t, cv::Mat, const Detector::DebugInfo& debugInfo)
	{}

	template<std::size_t I = 0, typename... Tp>
	inline typename std::enable_if<I < sizeof...(Tp), void>::type
	Process(std::tuple<Tp...>& t, cv::Mat& image, const Detector::DebugInfo& debugInfo)
	{
		std::get<I>(t).Process(image, debugInfo);
		Process<I + 1, Tp...>(t, image, debugInfo);
	}

public:
	cv::Mat operator()(const cv::Mat& image, const Detector::DebugInfo& debugInfo)
	{
		cv::Mat img;

		image.copyTo(img);
		std::tuple<Op...> t;
		Process(t, img, debugInfo);

		return img;
	}
};

#endif /* DEBUGIMAGEGENERATOR_H_ */
