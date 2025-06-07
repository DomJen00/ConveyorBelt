#include "src/Webserver.h"
#include "src/TelnetServer.h"
#include "src/ConveyorMotor.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstdlib>

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
    Webserver server(8080, motor); 

    sleep(1);
    motor.moveMotor(200);    
    int cnt = 0;

    while (cnt < 10) {
        cout << "Speed: " << motor.getSpeedRPM() << endl;
        sleep(1);
        cnt++;
    }

    motor.stopMotor();

    while (1) {
        cout << "Speed: " << motor.getSpeedRPM() << endl;
        sleep(1);
    }

    motor.stopMotor();
    
    return 0;
}