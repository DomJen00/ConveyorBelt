#include "Webserver.h"

Webserver::Webserver(int port, ConveyorMotor& motor) : ServerBase(AF_INET, SOCK_STREAM, 0, 5555, INADDR_ANY, 10), _motor(motor) {
	startServerThread();
	cout << "Webserver running on Port: " << port << endl;
}

void Webserver::handleClientConnection(int clientSocket) {
	char buffer[BUFFER_SIZE] = { 0 };
	read(clientSocket, buffer, BUFFER_SIZE);
	string request(buffer);

	// Check if request is POST request
	if (request.find("POST /set-speed") != string::npos) {
		handlePOSTRequest(clientSocket, request);
	}
	else {
		handleGETRequest(clientSocket, request);
	}
}

void Webserver::handlePOSTRequest(int clientSocket, const string& request) {
	size_t bodyStart = request.find("\r\n\r\n");

	if (bodyStart != string::npos) {
		// after header
		string body = request.substr(bodyStart + 4);
		// convert rpm to integer and move motor
		int rpm = atoi(body.c_str());
		_motor.moveMotor(rpm);

		string msg;
		if (rpm == 0) {
			msg = "Motor stopped.";
		}
		else {
			msg = "Motor running with: " + to_string(rpm) + "rpm.";
		}

		sendHttpResponse(clientSocket, msg);
	}
	else {
		sendHttpResponse(clientSocket, "Bad Request", "text/plain", "400 Bad Request");
	}
}

void Webserver::handleGETRequest(int clientSocket, const string& request) {
	string path = "/webpage.html";
	size_t getPos = request.find("GET ");

	if (getPos != string::npos) {
		size_t start = getPos + 4;
		size_t end = request.find(" ", start);
		string reqPath = request.substr(start, end - start);

		if (reqPath != "/") {
			path = reqPath;
		}
	}

	string filename = "../../.." + path;
	string content = loadFile(filename);
	string mime = getMimeType(filename);

	if (content.find("404 Not Found") != string::npos) {
		sendHttpResponse(clientSocket, content, "text/html", "404 Not Found");
	}
	else {
		sendHttpResponse(clientSocket, content, mime);
	}
}

void Webserver::sendHttpResponse(int clientSocket, const string& body, const string& contentType, const string& status) {
	stringstream response;
	response << "HTTP/1.1 " << status << "\r\n"
		<< "Content-Type: " << contentType << "\r\n"
		<< "Content-Length: " << body.size() << "\r\n"
		<< "Connection: close\r\n\r\n"
		<< body;

	write(clientSocket, response.str().c_str(), response.str().size());
	close(clientSocket);
}

string Webserver::loadFile(const string& filename) {
	ifstream file(filename);

	if (!file.is_open()) {
		return "<html><body><h1>404 Not Found</h1></body></html>";
	}

	string content, line;

	while (getline(file, line)) {
		content += line + "\n";
	}

    file.close();

	return content;
}

string Webserver::getMimeType(const string& path) {
	if (path.length() >= 5 && path.substr(path.length() - 5) == ".html") {
		return "text/html";
	}
	if (path.length() >= 4 && path.substr(path.length() - 4) == ".css") {
		return "text/css";
	}
	if (path.length() >= 3 && path.substr(path.length() - 3) == ".js") {
		return "application/javascript";
	}
	return "text/plain";
}