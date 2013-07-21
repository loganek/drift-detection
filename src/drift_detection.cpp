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

	while (!image.empty())
	{
		image = imgProvider.GetImage();
		detector.PushImage(image);

		detector.Process();
		imshow("TEST", DebugImageGenerator<FeatureListDebugOperator>()(image, detector.GetDebugInfo()));
	}

	return 0;
}

int main()
{
	return MainLoop<FrameByFrame>();
}
