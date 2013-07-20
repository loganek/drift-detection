/*
 * detector.cpp
 *
 *  Created on: 20 lip 2013
 *      Author: loganek
 */

#include "detector.h"
#include <stdexcept>

using namespace cv;
using namespace std;

Detector::Detector()
{
}

Detector::~Detector()
{
}

void Detector::PushImage(const Mat& image)
{
	currImage.copyTo(prevImage);
	image.copyTo(currImage);
}

bool Detector::CanProcess()
{
	return !currImage.empty() && !prevImage.empty();
}

void Detector::Process()
{
	if (!CanProcess())
		throw runtime_error("Cannot run algorithm: not enough images.");
}
