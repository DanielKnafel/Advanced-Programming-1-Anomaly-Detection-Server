    /*
    * timeseries.cpp
    *
    * Author: Daniel Knafel
    *         316012624
    */

    #include "timeseries.h"
    #include <iostream>
    #include <fstream>
    #include <vector>
    #include <map>
    #include <string.h>
    #include "anomaly_detection_util.h"

   vector<string> TextFileReader::splitLineByDelimiter(string line) {
        vector<string> split;
        char delimiter = ',';
        int end = 0;
        string data;

        while ((end = line.find(delimiter)) != string::npos) {
            data = line.substr(0, end);
            split.push_back(data);
            line.erase(0, end + 1);
        }

        // add last element in line
        data = line.substr(0, line.length());
        split.push_back(data);
        return split;
    }

   vector<float> TextFileReader::splitOneLineOfData(string line) {
        vector<string> stringData = TextFileReader::splitLineByDelimiter(line);
        vector<float> dataVector;
        for (string featureName : stringData) {
            // use atof to get float from string
            float f = atof(featureName.c_str());
            dataVector.push_back(f);
        }
        return dataVector;
    }

    // returns a vector contaning the keys in a map
    vector<string> getMapKeys(map<string, vector<float>> myMap) {
        vector<string> keysVector;
         for (auto const& element : myMap) {
             keysVector.push_back(element.first);
         }
         return keysVector;
    }

    // decleration
    void retrieveDataFromFile(const char* CSVfileName);

	TimeSeries::TimeSeries(const char* CSVfileName) {
        retrieveDataFromFile(CSVfileName);
        keys = getMapKeys(featuresMap);
	}

    void TimeSeries::retrieveDataFromFile(const char* CSVfileName) {
        vector<string> featureNames;
        vector<vector<float>> featuresVector;
        string line, featuresNamesString;
        // open the file for reading
        ifstream file(CSVfileName);

        // read the first line from the CSV and extract feature names
        getline(file, featuresNamesString);
        featureNames = TextFileReader::splitLineByDelimiter(featuresNamesString);
        for (string feature : featureNames) {
            featuresVector.push_back(vector<float>());
        }

        // get rest of data line by line
        vector<float> dataVector;
        // read the file until getline is false
        while (getline(file, line)) {
            dataVector = TextFileReader::splitOneLineOfData(line);
            for (int i = 0; i < dataVector.size(); i++) {
                featuresVector[i].push_back(dataVector[i]);
            }
        }

        for (int i = 0; i < featureNames.size(); i++) {
            pair<string, vector<float>> p(featureNames[i], featuresVector[i]);
            featuresMap.insert(p);
        }
        // close the file
        file.close();
    }

    vector<string> TimeSeries::getFeatureNamesVector() const {
        return keys;
    }

    const map<string, vector<float>> TimeSeries::getFeatureData() const {
        return featuresMap;
    }
    
    const vector<float> TimeSeries:: getFeatureDataByName(string name) {
        return featuresMap[name];
    }

    // make 2D points from 2 vectors
    vector<Point*> TimeSeries:: makePointsArray(string feature1, string feature2) {
        vector<float> f1 = featuresMap[feature1];
        vector<float> f2 = featuresMap[feature2];
        int size = f1.size();
        vector<Point*> points;
        for (int i = 0; i < size; i++) {
            points[i] = new Point(f1[i], f2[i]);
        }
        return points;
}

	int TimeSeries::getSampleSize() {
        return featuresMap[getFeatureNamesVector()[0]].size();
    }

    TimeSeries::~TimeSeries(){}
