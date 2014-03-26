/*
 * drift_detection.h
 *
 *  Created on: 20 lip 2013
 *      Author: loganek
 */

#include "image_source.h"
#include "detector.h"
#include "debug_image_generator.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

template<typename DisplayPolicy>
void MainLoop(const std::string& filename)
{
	cv::Mat image;
	ImageSource<DisplayPolicy> img_provider(filename);
	Detector detector;

	for (int i = 0; i < 3; i++)
	{
		image = img_provider.get_image();
		detector.push_image(image);
		cv::imshow("TEST", image);
	}
	cv::moveWindow("TEST", -60, 10);

	while (true)
	{
		detector.process();
		cv::imshow("TEST", DebugImageGenerator<
			   //FeatureListDebugOperator,
			   //ArrowFlowDebugOperator
			   //MainArrowDebugOperator,
			   DrawRouteDebugOperator
				>()(image, detector.get_debug_info()));
		image = img_provider.get_image();
		if (image.empty())
			break;

		detector.push_image(image);
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
		std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;

	MainLoop<
	  FrameByFrame
	  //Continuous
	>(argv[1]);

	std::cin.ignore().get(); 

	return 0;
}
