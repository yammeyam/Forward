#include "Motor.h"
#include "MotorsTesting.h"
#include <iostream>

int main()
{
    //M and V determine the piecewise linear dependence of the engine torque on the crankshaft rotation speed
    vector<int> M = { 20,75,100,105,75,0 };
    vector<int> V = {0,75,150,200,250,300};
    //An example of creating a motor with certain characteristics and running a test bench with a given overheating temperature
    Motor testMotor(10, M, V, 110, 0.01, 0.0001, 0.1);
    MotorsTesting testingStand(testMotor);
    double temperature;
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

