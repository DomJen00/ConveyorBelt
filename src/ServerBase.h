#pragma once

#include <pthread.h>
#include <atomic>
#include "SocketBase.h"

#define BUFFER_SIZE		4096

using namespace std;

class ServerBase
{
private:
	SocketBase* m_socket;
	pthread_t m_serverThread;
	atomic <bool> m_running;

	struct ClientThreadData {
		ServerBase* server;
		int clientSocket;
	};

	static void* serverThread(void* arg);
	static void* clientThread(void* arg);

protected:	
	bool isRunning();
	virtual void handleClientConnection(int clientSocket) = 0;
	SocketBase* getServerSocket();

public:
	ServerBase(int domain, int service, int protocol, int port, long interface, int backlog);
	virtual ~ServerBase();	
	int startServerThread();
	int stopServerThread();
};