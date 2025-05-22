#include "TelnetServer.h"

TelnetServer::TelnetServer(int port, ConveyorMotor& motor) : ServerBase(AF_INET, SOCK_STREAM, 0, 5555, INADDR_ANY, 10), _motor(motor) {
    _cmd = " ";
    _rpm = 0;
    startServerThread();
}

void TelnetServer::handleClientConnection(int clientSocket) {
    // Sending welcome message to the client
    char* message = "Welcome to the Telnet Server\n";
    write(clientSocket, message, strlen(message));

    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int recvBytes = read(clientSocket, buffer, sizeof(buffer) - 1);

        if (recvBytes <= 0) {
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = 0;

        if (strcmp(buffer, "exit") == 0) {      
            _motor.stopMotor();
            break;
        }

        int value = 0;

        if (parseCommand(buffer, &_cmd, &value)) {
            _rpm = value;

            if (_cmd == "move") {
                _motor.moveMotor(_rpm);
                cout << "Motor started" << endl;
            }
            else if (_cmd == "stop") {
                _motor.stopMotor();
                cout << "Motor stopped" << endl;
            }
            else if (_cmd == "status") {
                cout << _motor.getSpeedRPM() << endl;
            }
            else {
                cout << "Invalid command. Format: move (-)rpm or stop" << endl;
            }
        }
        else {
            cout << "Invalid command. Format: move (-)rpm or stop" << endl;
        }
    }
    cout << "Client connection closed" << endl;
    close(clientSocket);
}

bool TelnetServer::parseCommand(char* input, string* command, int* value) {

    while (*input == ' ') {
        input++;
    }

    if (strncmp(input, "move", 4) == 0) {
        *command = "move";
        input += 4;

        while (*input == ' ') {
            input++;
        }

        bool isNegative = false;
        if (*input == '-') {
            isNegative = true;
            input++;
        }
        else if (*input == ' ') {
            input++;
        }

        if (!isdigit(*input)) {
            return false;
        }

        int result = 0;
        while (isdigit(*input)) {
            result = result * 10 + (*input - '0');
            input++;
        }

        if (isNegative) {
            result = result * (-1);
        }
        *value = result;
        return true;
    }

    if (strncmp(input, "stop", 4) == 0) {
        *command = "stop";
        *value = 0;
        return true;
    }

    if (strncmp(input, "status", 6) == 0) {
        *command = "status";
        return true;
    }

    return false;
}