#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include "ServerBase.h"
#include "ConveyorMotor.h"

using namespace std;

class Webserver : public ServerBase
{
private:
	ConveyorMotor& m_motor;

	void handleClientConnection(int clientSocket);
	void handlePOSTRequest(int clientSocket, const string& request);
	void handleGETRequest(int clientSocket, const string& request);
	void sendHttpResponse(int clientSocket, const std::string& body, const std::string& contentType = "text/plain", const std::string& status = "200 OK");	
	string loadFile(const string& filename);
	string getMimeType(const string& path);	
	
public:
	Webserver(int port, ConveyorMotor& motor);
	~Webserver() override = default;
};