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

    sleep(1);
    motor.moveMotor(200);    
    int cnt = 0;

    while (cnt < 10) {
        cout << "Speed: " << motor.getSpeedRPM() << endl;
        sleep(1);
        cnt++;
    }

    motor.stopMotor();
    
    return 0;
}