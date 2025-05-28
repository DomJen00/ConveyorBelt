#include "ServerBase.h"

ServerBase::ServerBase(int domain, int service, int protocol, int port, long interface, int backlog) {
	m_running = false;
	m_socket = new SocketBase(domain, service, protocol, port, interface);
	if (m_socket->socketBind() < 0) {
		perror("In bind");
		exit(EXIT_FAILURE);
	}

	if (m_socket->socketListen(backlog) < 0) {
		perror("In listen");
		exit(EXIT_FAILURE);
	}
}

ServerBase::~ServerBase() {
	stopServerThread();
	delete m_socket;
}

int ServerBase::startServerThread() {
	m_running = true;

	if (pthread_create(&m_serverThread, nullptr, serverThread, this) != 0) {
		perror("Failed to start server thread");
		m_running = false;
		return -1;
	}	

	return 0;
}

int ServerBase::stopServerThread() {
	m_running = false;

	if (shutdown(m_socket->getSocket(), SHUT_RDWR) < 0) {
		perror("Failed to shutdown server socket");
		return -1;
	}

	if (pthread_join(m_serverThread, nullptr) != 0) {
		perror("Failed to join server");
		return -1;
	}

	return 0;
}

SocketBase* ServerBase::getServerSocket() {
	return m_socket;
}

bool ServerBase::isRunning() {
	return m_running;
}

void* ServerBase::serverThread(void* arg) {
	ServerBase* server = static_cast<ServerBase*>(arg);
	int clientSocket;
	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);

	while (server->m_running) {
		clientSocket = accept(server->m_socket->getSocket(), (sockaddr *)&clientAddr, &clientLen);

		if (clientSocket < 0) {
			if (server->m_running) {
				perror("Accept client failed");
			}
			
			break;
		}

		ClientThreadData* data = new ClientThreadData{ server, clientSocket };
		pthread_t cliThread;
		if (pthread_create(&cliThread, nullptr, clientThread, data) != 0) {
			perror("Create client thread failed");
			delete data;
		}
		else {
			pthread_detach(cliThread);
		}		
	}

	return nullptr;
}

void* ServerBase::clientThread(void* arg) {
	ClientThreadData* data = static_cast<ClientThreadData*>(arg);
	data->server->handleClientConnection(data->clientSocket);
	close(data->clientSocket);
	delete data;
	return nullptr;
}