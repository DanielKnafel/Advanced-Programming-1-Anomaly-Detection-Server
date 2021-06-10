#include "CLI.h"

CLI::CLI(DefaultIO* dio): dio(dio) {
    // data storage between commands
    settings = new AlgorithmSettings(CORRELATION);
    results = new DetectionResults();

    // initiallize commands
    commands[0] = new UploadCommand(dio);
    commands[1] = new ChangeSettingsCommand(dio, settings);
    commands[2] = new AnomalyDetectionCommand(dio, settings, results);
    commands[3] = new DisplayResultsCommand(dio, results);
    commands[4] = new StatisticsCommand(dio, results);
    commands[5] = new ExitCommand(dio);
}

// prints all commands in order
void CLI::printCommands() {
    for (int i = 0; i < EXIT_COMMAND; i++) {
        dio->write(i+1);
        dio->write(".");
        commands[i]->print();
    }
}

// runs the menu and executes commands selected by user until exit request.
void CLI::start(){
    string welcome = "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n";
    int userChoise = 0;
    while (userChoise != EXIT_COMMAND) {
        dio->write(welcome);
        printCommands();
        userChoise = atoi(dio->read().c_str());
		commands[userChoise-1]->execute();
    }
}


CLI::~CLI() {
    for (auto c : commands)
        delete c;
    delete settings;
    delete results;
}

