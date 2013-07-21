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
struct FeatureListDebugOperator
{
	static void Process(cv::Mat& image, const Detector::DebugInfo& debugInfo)
	{
		for (auto center : debugInfo.features[1])
			cv::circle(image, center, 3, cv::Scalar(0, 255, 255));
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
