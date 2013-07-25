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

	int range = 5;

	int GetMostCommonAngle();
	void CalculateAngles();
	void RemoveStrangePoints(int commonAngle);
	AnglesHistogram BuildHistogram();
	double GetMeanLength();

	void CleanVectors();
public:

	AverageVectorComputer(DebugInfo& debugInfo, std::vector<unsigned char>& status, FeatureList& features);
	virtual ~AverageVectorComputer();

	void ComputeAverageVector();
};

#endif /* AVERAGEVECTORCOMPUTER_H_ */
