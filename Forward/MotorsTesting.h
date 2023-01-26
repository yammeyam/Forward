#pragma once
#include "Motor.h"
#include <thread>
//Engine test bench class
class MotorsTesting {
public: 
	MotorsTesting(Motor &motor);
	Motor* currentMotor; //Motor being tested
	int overheatingTest(double temperature); //The method simulates the operation of the engine and determines the moment of its overheating
};