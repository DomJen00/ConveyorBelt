#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

class SocketBase
{
private:
	int m_socket_fd;
	struct sockaddr_in m_address;

public:
	SocketBase(int domain, int service, int protocol, int port, long interface);
	~SocketBase();
	int socketBind();
	int socketListen(int backlog);
	int socketConnect();
	int getSocket();
	sockaddr_in getAddress();
};