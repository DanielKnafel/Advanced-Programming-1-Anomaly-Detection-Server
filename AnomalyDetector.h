#ifndef ANOMALYDETECTOR_H_
#define ANOMALYDETECTOR_H_

#include <string>
#include <vector>
#include "timeseries.h"
#include "math.h"
using namespace std;


class AnomalyReport{
public:
	const string description;
	const long timeStep;
	AnomalyReport(string description, long timeStep):description(description),timeStep(timeStep){}
};


class TimeSeriesAnomalyDetector {
public:
	virtual void learnNormal(const TimeSeries& ts)=0;
	virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
	virtual ~TimeSeriesAnomalyDetector(){}
};

// class for representing subsequent anomally reports as intervals 
class AnomalySeries {
public:
	const string description;
	const pair<int, int> timeStep;
	AnomalySeries(string description, pair<int, int> timeStep):description(description), timeStep(timeStep) {}
	static bool isSubsequent(AnomalyReport i, AnomalyReport j) {
		return (j.timeStep == i.timeStep + 1) && (i.description == j.description);
	}
	static vector<AnomalySeries> makeSeries(vector<AnomalyReport> ar) {
		vector<AnomalySeries> v;
		int i = 0;
		while (i < ar.size()) {
			int start = ar[i].timeStep;
			int end = start;
			int j = i + 1;
			while (j < ar.size() && isSubsequent(ar[j - 1], ar[j])) {
					end++;
					j++;
			}
			v.push_back(AnomalySeries(ar[i].description, pair<int, int>(start, end)));
			i = j;
		}
		return v;
		}
};

#endif /* ANOMALYDETECTOR_H_ */
