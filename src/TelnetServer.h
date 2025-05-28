#pragma once

#include <string.h>
#include "ServerBase.h"
#include "ConveyorMotor.h"

using namespace std;

class TelnetServer : public ServerBase
{
private:	
	string m_cmd;
	int m_rpm;	
	ConveyorMotor& m_motor;
	void handleClientConnection(int clientSocket);
	bool parseCommand(char* input, string* command, int* value);

public:
	TelnetServer(int port, ConveyorMotor& motor);
	~TelnetServer() override = default;
};