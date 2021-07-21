# Advanced-Programming-1-Anomaly-Detection-Server
A project created by me as part of Advanced Programming 1.
## Introduction
In this C++ project, I created a TCP server capable of detecting anomalies in a time-series data file.
It does so by first learning what a "normal" data set is considered to be, and then analyzing any data set with the same features and calling out anomalies in it.

*This project shows the use of:*
1) The Command and Bridge design patterns.
2) Server-Client communication over the TCP protocol.

## Detailed Explanation
'AnomalyDetector.h' - declares an interface for TimeSeries anomaly detectors as well as 2 ways of representing anomalies.

`SimpleAnomalyDetector.h` - declares a specific anomaly detector implementing the AnomalyDetector interface. It uses a linear regression algorithm for the anomaly detection.
`SimpleAnomalyDetector.cpp` - the main unit of the project. It studies the correlations in a 'normal' data-set, and then searches for anomalies in any data-set, based on the knowledge gained during the learning phase. 
`HybridAnomalyDetector.h` - declares another specific anomaly detector. It uses both linear regression and minimal encolsing circle algorithms for the anomaly detection.
`HybridAnomalyDetector.cpp` - the hybrid anomaly detector inherits from SimpleAnomalyDetector and overrides its relevant methods. it utilizes dynamic binding of class methods to insure the override is successful during run-time.
`CLI.cpp` - implements the CLI for the server-client communication by using an array of Commands.
`Server.h` - declares the Server and ClientHandler classes. Implements the Brigde design pattern.
`Server.cpp` - creates and runs a TCP server.
`anomaly_detection_util.cpp` - includes the computational part of the linear regression algorithm, bases on statistic linear regression methods using pearson's correlation.
`commands.h` - declares and implements a set of commands to be used in the CLI. implements the Command design pattren.
`minCircle.cpp` - impelements Welzl's algorithm for finding the minimal enclosing circle of a set of points in a 2D plane.
`timeSeries.cpp` - a class representing the time-series data. It extracts the feature vectors from a columb-based CSV file.
`MainTrain.cpp` - a main function to run the program, along with a test of 2 clients.




