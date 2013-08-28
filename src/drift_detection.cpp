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
#include <ctime>
using namespace cv;
using namespace std;

template<typename DisplayPolicy>
int MainLoop()
{
	Mat image;
	ImageSource<DisplayPolicy> imgProvider("/home/loganek/Videos/quadro_videos/test3.wmv");
	Detector detector;
	time_t start,end;
	image = imgProvider.GetImage();
	detector.PushImage(image);
	time(&start);
	int counter=0;
	int i =0;
	while (!image.empty())
	{
		cv::moveWindow("TEST", -60, 10);
		image = imgProvider.GetImage();
		detector.PushImage(image);
		if (i > 2)
		{

			detector.Process();
			time(&end);
			++counter;
			double sec=difftime(end,start);
			double fps=counter/sec;
			printf("\n%lf",fps);
			imshow("TEST", DebugImageGenerator<
					//FeatureListDebugOperator,
					//ArrowFlowDebugOperator,
					MainArrowDebugOperator,
					DrawRouteDebugOperator
					>()(image, detector.GetDebugInfo()));
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
	return MainLoop<Continuous>();
}
