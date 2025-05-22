#include "SocketBase.h"

using namespace std;

SocketBase::SocketBase(int domain, int service, int protocol, int port, long interface) {
	// Create a socket
	_socket_fd = socket(domain, service, protocol);
	if (_socket_fd < 0) {
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	// Define address structure
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);	
}

SocketBase::~SocketBase() {
	if (_socket_fd >= 0) {
		close(_socket_fd);
		_socket_fd = -1;
	}
}

int SocketBase::socketBind() {
	return bind(_socket_fd, (struct sockaddr*)&_address, sizeof(_address));
}

int SocketBase::socketListen(int backlog) {
	return listen(_socket_fd, backlog);
}

int SocketBase::socketConnect() {
	return connect(_socket_fd, (struct sockaddr*)&_address, sizeof(_address));
}

int SocketBase::getSocket() {
	return _socket_fd;
}

sockaddr_in SocketBase::getAddress() {
	return _address;
}