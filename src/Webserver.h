#pragma once

#include <string.h>
#include "../lib/json.hpp"
#include "ServerBase.h"
#include "ConveyorMotor.h"

#define BUFFER_SIZE		4096

using namespace std;

class Webserver : public ServerBase
{
private:
	ConveyorMotor& _motor;
	void handleClientConnection(int clientSocket);
	
public:
	Webserver(int port, ConveyorMotor& motor);
	~Webserver() override = default;
};