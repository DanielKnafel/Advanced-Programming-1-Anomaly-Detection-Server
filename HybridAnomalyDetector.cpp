#include "HybridAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "minCircle.h"
#include <vector>
	
	
HybridAnomalyDetector::HybridAnomalyDetector(float linearCorrelation): SimpleAnomalyDetector(linearCorrelation, CIRCLE_THRESHOLD) {}

vector<Point*> makePointsVector(myMap::iterator f1, myMap::iterator f2) {
	vector<Point*> points;
	for (int i = 0; i < f1->second.size(); i++) {
		points.push_back(new Point(f1->second[i], f2->second[i]));
	}
	return points;
}

void deletePointsVector(vector<Point*> points) {
	for (Point *p : points) {
		delete p;
	}
}

void HybridAnomalyDetector::makeCorrelatedFeature(float pearson, myMap::iterator f1, myMap::iterator f2) {
	SimpleAnomalyDetector::makeCorrelatedFeature(pearson, f1, f2);

	if (pearson > CIRCLE_THRESHOLD && pearson < linearCorrelation) {
		vector<Point*> points = makePointsVector(f1, f2);
		Circle minCircle = findMinCircle(&(points[0]), points.size());
		minCircle.setRadius(minCircle.getRadius() * THRESHOLD_MULTIPLYER);
		correlatedFeatures c = {f1->first, f2->first, pearson, Line(), minCircle, minCircle.getRadius()};
		cf.push_back(c);
		deletePointsVector(points);
	}
}

void HybridAnomalyDetector::makeAnomalyReport(correlatedFeatures c, vector<float> feature1, vector<float> feature2) {
	SimpleAnomalyDetector::makeAnomalyReport(c, feature1, feature2);

	if (c.corrlation > CIRCLE_THRESHOLD && c.corrlation < linearCorrelation) {
		for (int i = 0; i < feature1.size(); i++) {
			Point p(feature1[i], feature2[i]);
			bool anomaly = c.minCircle.isPointInCircle(&p);

			if (!anomaly) {
				anomalies.push_back(makeReport(c, i));
			}
		}
	}
}

HybridAnomalyDetector::~HybridAnomalyDetector() {}