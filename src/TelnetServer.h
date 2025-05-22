#pragma once

#include <string.h>
#include "ServerBase.h"
#include "ConveyorMotor.h"

#define BUFFER_SIZE		2048

using namespace std;

class TelnetServer : public ServerBase
{
private:	
	string _cmd;
	int _rpm;	
	ConveyorMotor& _motor;
	void handleClientConnection(int clientSocket);
	bool parseCommand(char* input, string* command, int* value);

public:
	TelnetServer(int port, ConveyorMotor& motor);
	~TelnetServer() override = default;
};