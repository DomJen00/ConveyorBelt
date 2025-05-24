#include "Webserver.h"

using json = nlohmann::json;

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
		size_t bodyStart = request.find("\r\n\r\n");

		if (bodyStart != string::npos) {
			// after header
			string body = request.substr(bodyStart + 4);
			// convert rpm to integer and move motor
			int rpm = atoi(body.c_str());
			cout << rpm << endl;
			//_motor.moveMotor(rpm);

			string msg = "Motor running with: " + to_string(rpm) + "rpm";

			ostringstream response;
			response << "HTTP/1.1 200 OK\r\n"
				<< "Content-Type: text/plain\r\n"
				<< "Content-Length: " << msg.size() << "\r\n"
				<< "Connection: close\r\n\r\n"
				<< msg;

			write(clientSocket, response.str().c_str(), response.str().size());
			close(clientSocket);
			return;
		}
	} else {
		string path = "/webpage.html";
		size_t getPos = request.find("GET ");

		if (getPos != string::npos) {
			size_t start = getPos + 4;
			size_t end = request.find(" ", start);
			string reqPath = request.substr(start, end - start);

			cout << reqPath << endl;

			if (reqPath != "/") {
				path = reqPath;
			}
		}

		string filename = "../../.." + path;
		string content = loadFile(filename);
		string mime = getMimeType(filename);

		stringstream response;
		response << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: " << mime << "\r\n"
			<< "Content-Length: " << content.size() << "\r\n"
			<< "Connection: close\r\n\r\n"
			<< content;

		write(clientSocket, response.str().c_str(), response.str().size());
		close(clientSocket);
	}	
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