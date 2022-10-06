#include "Motor.h"
#include "MotorsTesting.h"
#include <iostream>

int main()
{
    vector<int> M = { 20,75,100,105,75,0 };
    vector<int> V = {0,75,150,200,250,300};
    Motor testMotor(10, M, V, 110, 0.01, 0.0001, 0.1);
    MotorsTesting testingStand(testMotor);
    double temperature;
  //testMotor.printParametres();
    cout << "Enter temperature of environment: ";
    cin >> temperature;
    double testTime = testingStand.overheatingTest(temperature);
    if (testTime == -1) {
        cout << "Engine does not overheat";
    }
    else if(testTime== 0) { 
        cout << "Simulation error"; 
    }
    else cout<<"Engine overheats after "<< testTime <<" seconds";
}

