

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>

using namespace std;

// abstract class for IO
class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	void uploadFile(string inPath, string outPath) {
		ifstream inStream;
		inStream.open(inPath);
		ofstream outStream;
		outStream.open(outPath);
		string line;

		while (getline(inStream, line) && line != "done") {
			outStream << line;
		}
		inStream.close();
		outStream.close();
	}
	
	void uploadFile(string path) {
		ofstream outStream;
		outStream.open(path);
		string line;

		while ((line = read()) != "done") {
			outStream << line << endl;
		}
		outStream.close();
	}
	
	vector<pair<int,int>> uploadAnomalyFile() {
		string line;
		vector<string> v;
		vector<pair<int,int>> anomalies;
		while ((line = read()) != "done") {
			v = TextFileReader::splitLineByDelimiter(line);
			int start = atoi(v[0].c_str());
			int end = atoi(v[1].c_str());
			pair<int, int> p(start, end);
			anomalies.push_back(p);
		}
		return anomalies;
	}

	vector<pair<int,int>> uploadAnomalyFile(string path) {
		ifstream inStream;
		inStream.open(path);
		string line;
		vector<string> v;
		vector<pair<int,int>> anomalies;
		while (getline(inStream, line) && line != "done") {
			v = TextFileReader::splitLineByDelimiter(line);
			int start = atoi(v[0].c_str());
			int end = atoi(v[1].c_str());
			pair<int, int> p(start, end);
			anomalies.push_back(p);
		}
		inStream.close();
		return anomalies;
	}
};

// specific class for SocketIO
class SocketIO : public DefaultIO {
	int clientID;
public:
	SocketIO(int clientID): clientID(clientID) {}

	virtual string read() {
		string str="";
		char c=0;
		recv(clientID,&c,sizeof(char), 0);
		while(c!='\n'){				
			str+=c;
			recv(clientID,&c,sizeof(char), 0);
		}
		return str;
	}

	virtual void write(string text) {
		send(clientID, text.c_str(), text.length(), 0);
	}

	virtual void write(float f) {
		stringstream ss;
		ss << f;
		write(ss.str());
	}

	virtual void read(float* f) {
		*f = stof(read());
	}
};

class AlgorithmSettings {
private:
	float pearson;
public:
	AlgorithmSettings(float pearson): pearson(pearson) {}
	// set new correlation threshold for correlation to replace deafault one 
	void setPearson(float newPearson) {
		pearson = newPearson;
	}
	float getPearson() {
		return pearson;
	}
	~AlgorithmSettings(){}
};

class DetectionResults {
private:
	vector<AnomalyReport> anomalies;
	int sampleSize;
public:
	DetectionResults(): sampleSize(0) {}
	void setSampleSize(int sampleSize) {
		this->sampleSize = sampleSize;
	}
	int getSampleSize() {
		return sampleSize;
	}
	
	void setAnomalies(vector<AnomalyReport> anomalies) {
		for(int i =0; i < anomalies.size(); i++) {
			this->anomalies.push_back(anomalies[i]);
		}
	}
	vector<AnomalyReport> getAnomalies() {
		return anomalies;
	}
	~DetectionResults(){}
};

class Command {
protected:
	DefaultIO* dio;
public:
	string descrition;
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute()=0;
	virtual ~Command(){}
	void print() { dio->write(descrition); }
};

class UploadCommand : public Command {
public:
	UploadCommand(DefaultIO* dio):Command(dio){ 
		descrition = "upload a time series csv file\n";
	}
	void execute() {
		dio->write("Please upload your local train CSV file.\n");
		//dio->uploadFile(dio->read(), "anomalyTest.csv");
		dio->uploadFile("anomalyTrain.csv");
		string done = "Upload complete.\n";
		dio->write(done);

		dio->write("Please upload your local test CSV file.\n");
		//dio->uploadFile(dio->read(), "anomalyTest.csv");
		dio->uploadFile("anomalyTest.csv");
		dio->write(done);
	}
};

class ChangeSettingsCommand : public Command {
	 AlgorithmSettings *settings;
public:
	ChangeSettingsCommand(DefaultIO* dio, AlgorithmSettings *settings):Command(dio), settings(settings) { 
		descrition = "algorithm settings\n";
	}
	void execute() {
		string currentCorrelation = "The current correlation threshold is ";
		dio->write(currentCorrelation);
		dio->write(settings->getPearson());
		dio->write("\n");

		string prompt = "Type a new threshold\n";
		dio->write(prompt);
		float newCorrelation;
		dio->read(&newCorrelation);

		string error = "please choose a value between 0 and 1.\n";
		while (newCorrelation < 0 || newCorrelation > 1)  {
			dio->write(error);
			dio->read(&newCorrelation);
		}
		settings->setPearson(newCorrelation);
	}
};

class AnomalyDetectionCommand : public Command {
	AlgorithmSettings *settings;
	DetectionResults *results;
public:
	AnomalyDetectionCommand(DefaultIO* dio, AlgorithmSettings *settings, DetectionResults *results)
								:Command(dio), settings(settings), results(results) {
		descrition = "detect anomalies\n";
	}
	void execute() {
		HybridAnomalyDetector ad(settings->getPearson());
		TimeSeries train("anomalyTrain.csv");
		ad.learnNormal(train);
		TimeSeries test("anomalyTest.csv");
		results->setAnomalies(ad.detect(test));
		dio->write("anomaly detection complete.\n");
		results->setSampleSize(test.getSampleSize());
	}
};

class DisplayResultsCommand : public Command {
	DetectionResults *results;
public:
	DisplayResultsCommand(DefaultIO* dio, DetectionResults *results):Command(dio), results(results){ 
		descrition = "display results\n";
	}
	void execute() {
		for (AnomalyReport a : results->getAnomalies()) {
			dio->write(to_string(a.timeStep) + "\t" + a.description);
			dio->write("\n");
		}
		dio->write("Done.\n");
	}
};

class StatisticsCommand : public Command {
	DetectionResults *results;
public:
	StatisticsCommand(DefaultIO* dio, DetectionResults *results):Command(dio), results(results) { 
		descrition = "upload anomalies and analyze results\n";
	}
	bool isNumberInRange(int n, pair<int, int> range) {
		return n <= range.second && n >= range.first;
	}
	bool hasIntersection(AnomalySeries& a ,pair<int, int>& range) {
		return isNumberInRange(a.timeStep.first, range) || isNumberInRange(a.timeStep.second, range)
									 || (a.timeStep.first < range.first && a.timeStep.second > range.second);
	}
	void execute() {
		dio->write("Please upload your local anomalies file.\n");
		//vector<pair<int,int>> v = dio->uploadAnomalyFile(dio->read());
		vector<pair<int,int>> input = dio->uploadAnomalyFile();
		string done = "Upload complete.\n";
		dio->write(done);

		vector<AnomalySeries> as = AnomalySeries::makeSeries(results->getAnomalies());
		int P = input.size();
		int N = results->getSampleSize();
		int FP = 0;
		int TP = 0;

		for (pair<int,int> range : input) {
			// N = n - all positive
			N -= (range.second - range.first) + 1;
			for (AnomalySeries a : as) {
				if (hasIntersection(a, range)) {
					TP++;
					break;
				}
			}
		}
		for (AnomalySeries a : as) {
			bool isFP = true;
			for (pair<int,int> range : input) {
				if (hasIntersection(a, range)) {
					isFP = false;
					break;
				}
			}
			if (isFP) {
				FP++;
			}
		}

		// True Possitive Rate = True Possitive / all Possitive
		float TPR = (float)TP / (float)P;
		// False Possitive Rate = False Possitive / all False
		float FPR = (float)FP / (float)N;

		// show precision of 3 digits.
		TPR = floor(TPR * 1000.0) / 1000.0;
		FPR = floor(FPR * 1000.0) / 1000.0;

		dio->write("True Positive Rate: ");
		dio->write(TPR);
		dio->write("\n");
		dio->write("False Positive Rate: ");
		dio->write(FPR);
		dio->write("\n");
	}
};

class ExitCommand : public Command {
public:
	ExitCommand(DefaultIO* dio):Command(dio){ 
		descrition = "exit\n";
	}
	void execute() {
		return;
	}
};

#endif /* COMMANDS_H_ */
