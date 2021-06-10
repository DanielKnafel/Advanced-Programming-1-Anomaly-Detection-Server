#ifndef CLI_H_
#define CLI_H_

// default correlation value for anomally detection
#define CORRELATION 0.9
// amount of commands in menu
#define EXIT_COMMAND 6

#include <string>
#include "commands.h"

using namespace std;

class CLI {
private:
	DefaultIO* dio;
	// array of commands for the menu
	Command *commands[EXIT_COMMAND];
    // data storage between commands
	AlgorithmSettings *settings;
    DetectionResults *results;
	void printCommands();
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
