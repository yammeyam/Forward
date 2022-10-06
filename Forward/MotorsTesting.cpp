#include "MotorsTesting.h"

MotorsTesting::MotorsTesting(Motor &motor)
{
    currentMotor = &motor;
}

int MotorsTesting::overheatingTest(double temperature)
{
    double overheatingTime=0;
    //Two threads, the first one performs the simulation, the second monitors the temperature
    thread threadSimulation([&](){
        currentMotor->simulation(temperature);
    });
    thread timeChecking([&](){   
        this_thread::sleep_for(std::chrono::microseconds(10));//For synchronisation
        while (currentMotor->getMotorT() < currentMotor->getOverheatingT() && currentMotor->getMotorIsRunning());
        currentMotor->setMotorIsRuning(0);
        overheatingTime = currentMotor->getWorkingTime();
    });
    timeChecking.join();
    threadSimulation.join();
    return overheatingTime;
}
