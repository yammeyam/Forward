#pragma once
#include <iostream>
#include <vector>
#include <mutex>
using namespace std;
//Class for the simulation of motor
class Motor
{
public:
	Motor();
	Motor(double I, vector<int> M, vector<int> V, double overheatingT, double Hm, double Hv, double C);
	Motor(const Motor& motor);

	void setI(double I);
	void setM(vector<int> M);
	void setV(vector<int> V);
	void setOverheatingT(double overheatingT);
	double getOverheatingT();
	void setHm(double Hm);
	void setHv(double Hv);
	void setC(double C);
	double getMotorT();
	double getMotorV();
	int getWorkingTime();
	void setMotorIsRuning(bool state);
	bool getMotorIsRunning();

	void printParametres(); //For checking
	//Motor simulation process
	void simulation(double environmentT);

private:
	const double eps = 0.000000001;
	//Motor basic parameters 
	double I;				//Moment of inertia
	vector<int> M;			//Torque
	vector<int> V;			//Rotation speed
	double Hm;				//Heating speed on torque coefficient 
	double Hv;				//Heating speed on rotation speed coefficient
	double C;				//Coefficient of cooling speed on the temperature of the motor and medium

	//Motor parametres for testing and its mutexes
	mutex motorTMutex;		//Mutex for external checking of current temperature
	mutex workingTimeMutex;		//Mutex for external checking of current time
	mutex motorVMutex;		//Mutex for external checking of current rotation speed
	mutex motorIsRuningMutex;	//Mutex for external checking of motor state
	mutex overheatingTMutex;	//Mutex for external checking of overheating temperature

	double motorT = 0;		//Motor current temperature
	double motorV = 0;		//Current rotation speed
	int workingTime = 0;	//Current time of work
	bool motorIsRuning = 0; //Simulation start flag
	double overheatingT;	//Temperature of overheating


	//Calculate the torque value for the current value of the rotation speed
	double computeM(double currentV);
};