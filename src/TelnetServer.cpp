#include "TelnetServer.h"

TelnetServer::TelnetServer(int port, ConveyorMotor& motor) : ServerBase(AF_INET, SOCK_STREAM, 0, 5555, INADDR_ANY, 10), m_motor(motor) {
    m_cmd = " ";
    m_rpm = 0;
    startServerThread();
}

void TelnetServer::handleClientConnection(int clientSocket) {
    // Sending welcome message to the client
    char* message = "Welcome to the Telnet Server\n";
    write(clientSocket, message, strlen(message));
    char buffer[BUFFER_SIZE];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int recvBytes = read(clientSocket, buffer, BUFFER_SIZE);

        if (recvBytes <= 0) {
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = 0;

        if (strcmp(buffer, "exit") == 0) {      
            m_motor.stopMotor();
            break;
        }

        int value = 0;

        if (parseCommand(buffer, &m_cmd, &value)) {
            m_rpm = value;

            if (m_cmd == "move") {
                m_motor.moveMotor(m_rpm);
                cout << "Motor started" << endl;
            }
            else if (m_cmd == "stop") {
                m_motor.stopMotor();
                cout << "Motor stopped" << endl;
            }
            else if (m_cmd == "status") {
                cout << m_motor.getSpeedRPM() << endl;
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