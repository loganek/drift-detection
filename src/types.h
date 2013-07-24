/*
 * types.h
 *
 *  Created on: 24 lip 2013
 *      Author: loganek
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <vector>
#include <array>
#include <opencv2/core/types.hpp>

typedef std::vector<cv::Point2f> PointList;
typedef std::array<PointList, 2> FeatureList;
typedef std::array<unsigned char, 360> AnglesHistogram;

#endif /* TYPES_H_ */
