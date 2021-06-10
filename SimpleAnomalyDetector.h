/*
 * SimpleAnomalyDetector.h
 *
 * Author: Daniel Knafel
 *         316012624
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <iostream>
#include "minCircle.h"

#define THRESHOLD_MULTIPLYER 1.1

typedef map<string, vector<float>> myMap;

struct correlatedFeatures{
	string feature1, feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	Circle minCircle;
	float threshold;
	void print() {
		cout << feature1 << "-" << feature2 << endl;
	}
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	float linearCorrelation;
	float circleCorrelation;
	vector<correlatedFeatures> cf; 
	vector<AnomalyReport> anomalies;
	virtual void makeCorrelatedFeature(float pearson, myMap::iterator f1, myMap::iterator f2);
	virtual void makeAnomalyReport(correlatedFeatures c, vector<float> feature1, vector<float> feature2);
	AnomalyReport makeReport(correlatedFeatures c, int i);
public:
	SimpleAnomalyDetector(float linearCorrelation, float circleCorrelation):linearCorrelation(linearCorrelation), circleCorrelation(circleCorrelation){}
	virtual ~SimpleAnomalyDetector();
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	vector<correlatedFeatures> getNormalModel() { return cf; }
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
