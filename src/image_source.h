/*
 * image_source.h
 *
 *  Created on: 20 lip 2013
 *      Author: loganek
 */

#ifndef IMAGE_SOURCE_H_
#define IMAGE_SOURCE_H_

#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <stdexcept>

struct FrameByFrame
{
	static const int Value = 0;
};

struct Continuous
{
	static const int Value = 1;
};

template<typename DisplayPolicy>
class ImageSource
{
private:
	cv::VideoCapture video;
public:
	ImageSource(const std::string& filename)
	{
		if (!video.open(filename))
			throw std::runtime_error("Cannot open file: " + filename);
	}

	ImageSource(int deviceId)
	{
		if (!video.open(deviceId))
			throw std::runtime_error("Cannot open device: " + std::to_string(deviceId));
	}
	virtual ~ImageSource() {}

	cv::Mat GetImage()
	{
		cv::Mat img;

		cv::waitKey(DisplayPolicy::Value);
		video >> img;

		return img;
	}
};

#endif /* IMAGE_SOURCE_H_ */
