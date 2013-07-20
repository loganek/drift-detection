/*
 * detector.h
 *
 *  Created on: 20 lip 2013
 *      Author: loganek
 */

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <opencv2/core/mat.hpp>

class Detector
{
private:
	cv::Mat prevImage;
	cv::Mat currImage;

	bool CanProcess();
public:
	Detector();
	virtual ~Detector();

	void PushImage(const cv::Mat& image);
	void Process();
};

#endif /* DETECTOR_H_ */
