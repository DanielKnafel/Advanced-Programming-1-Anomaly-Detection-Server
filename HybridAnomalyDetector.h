// 316012624

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"

#define CIRCLE_THRESHOLD 0.5

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector(float linearCorrelation);
	virtual ~HybridAnomalyDetector();
	virtual void makeCorrelatedFeature(float pearson, myMap::iterator f1, myMap::iterator f2);
	virtual void makeAnomalyReport(correlatedFeatures c, vector<float> feature1, vector<float> feature2);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
