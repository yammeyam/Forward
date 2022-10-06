#pragma once
#include "Motor.h"
#include <thread>

class MotorsTesting {
public: 
	MotorsTesting(Motor &motor);
	Motor* currentMotor;
	int overheatingTest(double temperature);
};