#include "SocketBase.h"

using namespace std;

SocketBase::SocketBase(int domain, int service, int protocol, int port, long interface) {
	// Create a socket
	m_socket_fd = socket(domain, service, protocol);
	if (m_socket_fd < 0) {
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	// Define address structure
	m_address.sin_family = domain;
	m_address.sin_port = htons(port);
	m_address.sin_addr.s_addr = htonl(interface);	
}

SocketBase::~SocketBase() {
	if (m_socket_fd >= 0) {
		close(m_socket_fd);
		m_socket_fd = -1;
	}
}

int SocketBase::socketBind() {
	return bind(m_socket_fd, (struct sockaddr*)&m_address, sizeof(m_address));
}

int SocketBase::socketListen(int backlog) {
	return listen(m_socket_fd, backlog);
}

int SocketBase::socketConnect() {
	return connect(m_socket_fd, (struct sockaddr*)&m_address, sizeof(m_address));
}

int SocketBase::getSocket() {
	return m_socket_fd;
}

sockaddr_in SocketBase::getAddress() {
	return m_address;
}