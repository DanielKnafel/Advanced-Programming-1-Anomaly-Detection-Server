
#include "Server.h"

Server::Server(int port) throw (const char*) {
	stopServer = false;
	// open socket and save fileDescriptor
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
		throw("Socket Failed");
	// use IPV4
	server.sin_family = AF_INET;
	// current machine
	server.sin_addr.s_addr = INADDR_ANY;
	// convert to Big-Endian
	server.sin_port = htons(port);

	if(bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0)
		throw("Bind failed");
	
	// allow a max of 3 users in waiting
	if(listen(fd, 3) < 0) 
		throw("Listen failed");
}

void Server::start(ClientHandler& ch) throw (const char*){	
	// run server in new thread
	t = new thread([&ch, this](){
				while (!stopServer) {
					socklen_t clientSize = sizeof(client);
					// accept client connection
					int aClient = accept(fd, (struct sockaddr*)&client, &clientSize);
					if(aClient < 0)
						throw("Accept falied");
					// handle the client
					ch.handle(aClient);
					// close connection to client
					close(aClient);
				}
				// close server
				close(fd);
	});
}

void Server::stop(){
	stopServer = true;
	// join the thread to kill it
	t->join();
}

Server::~Server() {
}

