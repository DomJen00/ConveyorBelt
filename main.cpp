#include "src/Webserver.h"
#include "src/TelnetServer.h"
#include "src/ConveyorMotor.h"
#include <unistd.h>

using namespace std;

void getCWD() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        cout << "Aktuelles Arbeitsverzeichnis: " << cwd << endl;
    }
    else {
        cerr << "Fehler beim Ermitteln des Arbeitsverzeichnisses." << endl;
    }
}

int main()
{
    ConveyorMotor motor;
    TelnetServer telnet(5555, motor);   
    Webserver server(4444, motor);

    while (1) {
        cout << "Actual RPM: " << motor.getSpeedRPM() << endl;
        sleep(2);
    }

    server.stopServerThread();

    return 0;
}