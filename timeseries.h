/*
 * timeseries.h
 *
 * Author: Daniel Knafel
 *         316012624
 */
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <map>
#include "anomaly_detection_util.h"

using namespace std;

//represents a time-series data file
class TimeSeries{
	private:
	// members
   	map<string, vector<float>> featuresMap;
    vector<string> keys;
	void retrieveDataFromFile(const char* CSVfileName);

	public:
	TimeSeries(const char* CSVfileName);
	vector<string> getFeatureNamesVector() const;
	const map<string, vector<float>> getFeatureData() const;
	const vector<float> getFeatureDataByName(string name);
	vector<Point*> makePointsArray(string feature1, string feature2);
	int getSampleSize();
	~TimeSeries();
};

// retrieves data from text file
class TextFileReader {
    public:
        static vector<string> splitLineByDelimiter(string line);
        static vector<float> splitOneLineOfData(string line);
};


#endif /* TIMESERIES_H_ */
