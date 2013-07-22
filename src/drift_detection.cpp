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

using namespace cv;
using namespace std;

template<typename DisplayPolicy>
int MainLoop()
{
	Mat image;
	ImageSource<DisplayPolicy> imgProvider("/home/loganek/Downloads/naturalny.wmv");
	Detector detector;

	image = imgProvider.GetImage();
	detector.PushImage(image);
	int i =0;
	while (!image.empty())
	{
		image = imgProvider.GetImage();
		detector.PushImage(image);
		if (i%2)
		{
			detector.Process();
			imshow("TEST", DebugImageGenerator<ArrowFlowDebugOperator>()(image, detector.GetDebugInfo()));
		}
		else
		{
			imshow("TEST", image);
		}
		i++;
	}

	return 0;
}

int main()
{
	return MainLoop<FrameByFrame>();
}
