/*
 * average_vector_computer.h
 *
 *  Created on: 24 lip 2013
 *      Author: loganek
 */

#ifndef AVERAGEVECTORCOMPUTER_H_
#define AVERAGEVECTORCOMPUTER_H_

#include "types.h"

struct DebugInfo;

class AverageVectorComputer
{
private:
	DebugInfo& debugInfo;
	std::vector<unsigned char>& status;
	FeatureList& features;
	std::vector<int> angles;

	int GetMostCommonAngle(int range);
	void CalculateAngles();
	void RemoveStrangePoints();
	AnglesHistogram BuildHistogram();
	double GetMeanLength();

public:

	AverageVectorComputer(DebugInfo& debugInfo, std::vector<unsigned char>& status, FeatureList& features);
	virtual ~AverageVectorComputer();

	void ComputeAverageVector();
};

#endif /* AVERAGEVECTORCOMPUTER_H_ */
