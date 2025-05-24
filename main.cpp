#include "src/Webserver.h"
#include "src/TelnetServer.h"
#include "src/ConveyorMotor.h"
#include <unistd.h>

using namespace std;

int main()
{
    /*
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Aktuelles Arbeitsverzeichnis: " << cwd << std::endl;
    }
    else {
        std::cerr << "Fehler beim Ermitteln des Arbeitsverzeichnisses." << std::endl;
    }
    */

    ConveyorMotor motor;

    //TelnetServer telnet(5555, motor);
    Webserver server(5555, motor);

    while (1) {
    }

    server.stopServerThread();

    return 0;
}