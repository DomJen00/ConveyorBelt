#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
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
	string loadFile(const string& filename);
	string getMimeType(const string& path);
	
public:
	Webserver(int port, ConveyorMotor& motor);
	~Webserver() override = default;
};