#include "src/Webserver.h"
#include "src/TelnetServer.h"
#include "src/ConveyorMotor.h"

using namespace std;

int main()
{

    ConveyorMotor motor;

    //TelnetServer telnet(5555, motor);
    Webserver server(5555, motor);

    while (1) {
    }

    server.stopServerThread();

    return 0;
}