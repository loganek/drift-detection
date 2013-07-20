#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "image_source.h"

using namespace cv;
using namespace std;

template<typename DisplayPolicy>
int MainLoop()
{
	Mat img;
	ImageSource<DisplayPolicy> imgProvider("/home/loganek/Downloads/naturalny.wmv");

	img = imgProvider.GetImage();

	while (!img.empty())
	{
		imshow("TEST", img);
		img = imgProvider.GetImage();
	}

	return 0;
}

int main()
{
	return MainLoop<Continuous>();
}
