#include "Webserver.h"

using json = nlohmann::json;

Webserver::Webserver(int port, ConveyorMotor& motor) : ServerBase(AF_INET, SOCK_STREAM, 0, 5555, INADDR_ANY, 10), _motor(motor) {
	startServerThread();
}

void Webserver::handleClientConnection(int clientSocket) {
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	read(clientSocket, buffer, BUFFER_SIZE);

	// Cast char to string
	string request(buffer);

	// Extract JSON (after \r\n\r\n the body section starts)
	size_t bodyStart = request.find("\r\n\r\n");
	if (bodyStart != string::npos) {
		string body = request.substr(bodyStart + 4);
			
		try {
			json data = json::parse(body);
			if (data.contains("rpm")) {
				int rpm = data["rpm"];
				_motor.moveMotor(rpm);
				cout << "Speed [rpm]: " << rpm << endl;
				// HTTP response
				string response = "HTTP/1.1 200 OK\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 3\r\n"
					"\r\n"
					"OK\n";
				cout << "Client request" << endl;
				write(clientSocket, response.c_str(), response.size());
			}else {
				cerr << "No rpm field in JSON" << endl;
			}
		}
		catch (exception& e) {
			cerr << "Invalid JSON" << endl;
			string error = "HTTP/1.1 400 Bad Request\r\n Content-Type: text/plain\r\n Content-Length: 14\r\n\r\nInvalid\n";
			write(clientSocket, error.c_str(), error.size());
		}
	}

	close(clientSocket);
}