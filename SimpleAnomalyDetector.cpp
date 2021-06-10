/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Daniel Knafel
 *         316012624
 */

#include "SimpleAnomalyDetector.h"
#include <vector>
#include "anomaly_detection_util.h"
#include <string>
#include <map>

using namespace std;

void printCorrelatedFeatures(correlatedFeatures correlatedFeature) {
	cout << correlatedFeature.feature1 << "-" << correlatedFeature.feature2 << " pearson: " << correlatedFeature.corrlation << endl;
}

void printPearsonArray(vector<correlatedFeatures> pearsonVector) {
	for (correlatedFeatures correlatedFeature : pearsonVector)
	{
		printCorrelatedFeatures(correlatedFeature);
	}
}

const float thresholdForTwoFeatures(float *arrayI, float *arrayJ, int size, Line regLine) {
	float threshold = 0;
	for (int i = 0; i < size; i++) {
		Point p(arrayI[i], arrayJ[i]);
		// find the farthest point from the line
		threshold = max(threshold, dev(p, regLine));
	}
	return THRESHOLD_MULTIPLYER * threshold;
}

void SimpleAnomalyDetector::makeCorrelatedFeature(float pearson, myMap::iterator f1, myMap::iterator f2) {
	// if the pearson value between 2 features is over the threshold, calculate correlatedFeatures report for it.
	if (pearson >= linearCorrelation) {
		// make pointers to the first cell of each vector
		float *arrayI = &(f1->second[0]);
		float *arrayJ = &(f2->second[0]);
		// size of the data
		int size = f1->second.size();
		Line regLine = linear_reg(arrayI, arrayJ, size);
		float threshold = thresholdForTwoFeatures(arrayI, arrayJ, size, regLine);
		// initialize the correlatedFeatures object
		correlatedFeatures c = {f1->first, f2->first, pearson, regLine, Circle() ,threshold};
		cf.push_back(c);
	}
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
	// retrieve data from the TimeSeries object
	myMap featuresMap = ts.getFeatureData();
	
	// define varubales to be used later
	float *arrayI = nullptr, *arrayJ = nullptr;
	float currentPearson, maxPearson;
	// used to store the most correlative feature
	myMap::iterator correlativeTo;
	for(myMap::iterator it1 = featuresMap.begin(); it1 != featuresMap.end(); it1++) {
		// initialize
		maxPearson = 0;
		currentPearson = 0;
		// make a pointer to the first cell of the vector
		arrayI = &(it1->second[0]);
		// start checking correlation with the next feature in line
		myMap::iterator it2 = it1;
		it2++;
		for(; it2 != featuresMap.end(); it2++) {
			// make a pointer to the first cell of the vector
			arrayJ = &(it2->second[0]);
			// calculate pearson for current 2 features
			currentPearson = std::abs(pearson(arrayI, arrayJ, it2->second.size()));
			// store max pearson values for each feature
			if (currentPearson > maxPearson) {
				maxPearson = currentPearson;
				correlativeTo = it2;
			}
		}
		makeCorrelatedFeature(maxPearson, it1, correlativeTo);
	}
}

AnomalyReport SimpleAnomalyDetector::makeReport(correlatedFeatures c, int i) {
	string description = c.feature1 + "-" + c.feature2;
	return AnomalyReport(description, i + 1);
}

void SimpleAnomalyDetector::makeAnomalyReport(correlatedFeatures c, vector<float> feature1, vector<float> feature2) {
	for (int i = 0; i < feature1.size(); i++) {
		Point p(feature1[i], feature2[i]);
		float deviation = dev(p, c.lin_reg);

		if (deviation > c.threshold) {
			anomalies.push_back(makeReport(c, i));
		}
	}
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
	myMap featuresMap = ts.getFeatureData();
	
	for (correlatedFeatures c : this->cf) {
		vector<float> feature1 = featuresMap[c.feature1];
		vector<float> feature2 = featuresMap[c.feature2];
		makeAnomalyReport(c, feature1, feature2);
	}
	return anomalies;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}