/*
 * average_vector_computer.cpp
 *
 *  Created on: 24 lip 2013
 *      Author: loganek
 */
#include <iostream>
#include "average_vector_computer.h"
#include "detector.h"

using namespace std;

AverageVectorComputer::AverageVectorComputer(DebugInfo& debugInfo, vector<unsigned char>& status, FeatureList& features)
	: debugInfo(debugInfo),
	  status(status),
	  features(features)
{
}

AverageVectorComputer::~AverageVectorComputer()
{
}

DriftVector AverageVectorComputer::ComputeAverageVector()
{
	CleanVectors();
	CalculateAngles();
	int commonAngle = GetMostCommonAngle();
	RemoveStrangePoints(commonAngle);
	CleanVectors();
	DriftVector vect;

	int cnt = 0;
	for (auto s : status)
		if (s) cnt++;

	if (cnt)
	{
		vect.length = GetMeanLength();
		vect.angle = commonAngle;
	}

	debugInfo.driftVector = vect;
	debugInfo.featureStatus = status;
	debugInfo.angles = angles;

	return vect;
}
// TODO: split CalculateAngles and RemoveStrangePoints together (in one loop and 'if' statement)

void AverageVectorComputer::CleanVectors()
{
	/*size_t i = 0;

	while (i < status.size())
	{
		if (status[i])
		{
			status.erase(status.begin() + i);
			features[0].erase(features[0].begin() + i);
			features[1].erase(features[1].begin() + i);
		}
		else
			i++;
	}*/
}

AnglesHistogram AverageVectorComputer::BuildHistogram()
{
	AnglesHistogram histogram;
	histogram.fill(0);

	for (auto angle : angles)
	{
		if (angle >= 0)
			histogram[angle]++;
	}

	return histogram;
}

void AverageVectorComputer::CalculateAngles()
{
	angles = vector<int>(status.size(), -1);

	for (size_t i = 0; i < status.size(); i++)
	{
		if (status[i])
		{
			angles[i] = int(atan2(features[1][i].y - features[0][i].y, features[1][i].x - features[0][i].x) * 180 / M_PI);
			if (angles[i] < 0)
				angles[i] += 360;
		}
	}
}

void AverageVectorComputer::RemoveStrangePoints(int commonAngle)
{
	for (size_t i = 0; i < angles.size(); i++)
	{
		if (status[i] && (angles[i] < commonAngle - (int)range || angles[i] > commonAngle + (int)range))
			status[i] = 0;
	}
}

int AverageVectorComputer::GetMostCommonAngle()
{
	AnglesHistogram newHistogram, histogram;

	histogram = BuildHistogram();

	// TODO I'm sure might be done better (more effective)

	newHistogram.fill(0);
	newHistogram[359] = std::accumulate(histogram.end() - range, histogram.end(), 0, std::plus<unsigned char>()) +
			std::accumulate(histogram.begin(), histogram.begin() + range, 0, std::plus<unsigned char>());

	int max = 0, maxIt = 0;

	for (size_t i = 0; i < 360; i++)
	{
		// TODO prove instruction shown below!
		newHistogram[i] = newHistogram[i < 1 ? 360 - i - 1 : i-1] + histogram[(i+range-1)%360] - histogram[i < range ? 360 - (range - i + 1) : i-range];
		if (newHistogram[i] > max)
		{
			max = newHistogram[i];
			maxIt = i;
		}
	}

	return maxIt;
}

double AverageVectorComputer::GetMeanLength()
{
	double length = 0.0;
	size_t cnt = 0;

	for (size_t i = 0; i < status.size(); i++)
	{
		if (status[i])
		{
			length += sqrt((features[0][i].x - features[1][i].x) * (features[0][i].x - features[1][i].x) +
					(features[0][i].y - features[1][i].y) * (features[0][i].y - features[1][i].y));
			cnt++;
		}
	}

	return length / cnt;
}
