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
	static void process(cv::Mat& image, const DebugInfo& debug_info)
	{
		for (auto center : debug_info.features[0])
					cv::circle(image, center, 3, cv::Scalar(0, 255, 0));

		for (auto center : debug_info.features[1])
			cv::circle(image, center, 3, cv::Scalar(0, 255, 255));
	}
};

struct ArrowFlowDebugOperator
{
protected:
	static void draw_arrow(cv::Mat& image, cv::Point2f second, cv::Point2f first, double angle, cv::Scalar color = cv::Scalar(240, 55, 24))
	{
		cv::Point2f p(first.x, first.y),
				q(second.x, second.y);
		angle *= M_PI / 180.0;

		cv::line( image, p, q, color);

		p.x = q.x + 9 * cos(angle + M_PI_4);
		p.y = q.y + 9 * sin(angle + M_PI_4);
		cv::line( image, p, q, color);

		p.x = q.x + 9 * cos(angle - M_PI_4);
		p.y = q.y + 9 * sin(angle - M_PI_4);
		cv::line( image, p, q, color);
	}

public:
	static void process(cv::Mat& image, const DebugInfo& debug_info)
	{
		for (size_t i = 0; i < debug_info.feature_status.size(); i++)
		{
			if (debug_info.feature_status[i])
			{
				draw_arrow(image, debug_info.features[0][i], debug_info.features[1][i], debug_info.angles[i]);
			}
		}
	}
};

struct MainArrowDebugOperator : public ArrowFlowDebugOperator
{
	static void process(cv::Mat& image, const DebugInfo& debug_info)
	{
		double angle = debug_info.drift_vector.angle * M_PI / 180;
		const cv::Point2f center(100, 100);
		double x = debug_info.drift_vector.length * cos(angle);
		double y = debug_info.drift_vector.length * sin(angle);

		draw_arrow(image, center, cv::Point2f(x + center.x, y + center.y), debug_info.drift_vector.angle, cv::Scalar(50, 100, 150));
	}
};

struct DrawRouteDebugOperator
{
	static void process(cv::Mat& image, const DebugInfo& debugInfo)
	{
		double dx = 400, dy = 250;
		for (PointList::reverse_iterator delta = debugInfo.route->rbegin(); delta != debugInfo.route->rend(); delta++)
		{
			cv::line(image, cv::Point2f(dx, dy), cv::Point2f(dx + delta->x, dy + delta->y), cv::Scalar(200, 150, 50), 4);
			dx += delta->x;
			dy += delta->y;
		}
	}
};

template<typename... Op>
class DebugImageGenerator
{
private:
	template<std::size_t I = 0, typename... Tp>
	inline typename std::enable_if<I == sizeof...(Tp), void>::type
	process(std::tuple<Tp...>& t, cv::Mat, const DebugInfo& debug_info)
	{}

	template<std::size_t I = 0, typename... Tp>
	inline typename std::enable_if<I < sizeof...(Tp), void>::type
	process(std::tuple<Tp...>& t, cv::Mat& image, const DebugInfo& debug_info)
	{
		std::get<I>(t).process(image, debug_info);
		process<I + 1, Tp...>(t, image, debug_info);
	}

public:
	cv::Mat operator()(const cv::Mat& image, const DebugInfo& debug_info)
	{
		cv::Mat img;

		image.copyTo(img);
		std::tuple<Op...> t;
		process(t, img, debug_info);

		return img;
	}
};

#endif /* DEBUGIMAGEGENERATOR_H_ */
