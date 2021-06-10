/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <pthread.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <iostream>
#include <istream> 
#include <unistd.h>
#include "CLI.h"

using namespace std;

// abstract class for Client Handlers
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

// specific ClientHandler for Anomaly Detection
class AnomalyDetectionHandler:public ClientHandler {
public:
    virtual void handle(int clientID){
		// an IO 
		SocketIO sio(clientID);
		CLI cli(&sio);
		cli.start();
    }
};

class Server {
	thread* t; // the thread to run the start() method in
	int fd;
	sockaddr_in server;
	sockaddr_in client;
	volatile bool stopServer;
public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch) throw (const char*);
	void stop();
};

#endif /* SERVER_H_ */
