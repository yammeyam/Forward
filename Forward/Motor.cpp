#include "Motor.h"

Motor::Motor()
{
	I = 1;
	overheatingT = 0;
	Hm = 0;
	Hv = 0;
	C = 0;
}

Motor::Motor(double I, vector<int> M, vector<int> V, double overheatingT, double Hm, double Hv, double C)
{
	if (I == 0) {	//To avoid dividing by zero
		this->I = 1;
	} else this->I = I;
	if (M.size()==V.size()){ //Must be the same size
		this->M = M;
		this->V = V;
	}
	this->overheatingT = overheatingT;
	this->Hm = Hm;
	this->Hv = Hv;
	this->C = C;
}

Motor::Motor(const Motor& motor)
{
	I = motor.I;
	M = motor.M;
	V = motor.V;
	overheatingT = motor.overheatingT;
	Hm = motor.Hm;
	Hv = motor.Hv;
	C = motor.C;
}

void Motor::setI(double I)
{
	if (I == 0) { //To not divide by 0
		this->I = 1;
		return;
	}
	this->I = I;
}

void Motor::setM(vector<int> M)
{
	this->M = M;
}

void Motor::setV(vector<int> V)
{
	this->V = V;
}


void Motor::setHm(double Hm)
{
	this->Hm = Hm;
}

void Motor::setHv(double Hv)
{
	this->Hv = Hv;
}

void Motor::setC(double C)
{
	this->C = C;
}

void Motor::setOverheatingT(double overheatingT)
{
	overheatingTMutex.lock();
	this->overheatingT = overheatingT;
	overheatingTMutex.unlock();
}

double Motor::getOverheatingT()
{
	overheatingTMutex.lock();
	double value = overheatingT;
	overheatingTMutex.unlock();
	return value;
}

double Motor::getMotorT()
{

	motorTMutex.lock();
	double value = motorT;
	motorTMutex.unlock();
	return value;
}

double Motor::getMotorV()
{
	motorVMutex.lock();
	double value = motorV;
	motorVMutex.unlock();
	return value;
}

int Motor::getWorkingTime()
{
	workingTimeMutex.lock();
	double value = workingTime;
	workingTimeMutex.unlock();
	return value;
}

void Motor::setMotorIsRuning(bool state)
{
	motorIsRuningMutex.lock();
	motorIsRuning = state;
	motorIsRuningMutex.unlock();
}

bool Motor::getMotorIsRunning()
{
	motorIsRuningMutex.lock();
	bool value = motorIsRuning;
	motorIsRuningMutex.unlock();
	return value;
	
}

void Motor::printParametres()
{
	cout << "Moment of inertia: " << I << endl;
	cout << "Torque steps: " << endl;
	for (int i = 0; i < M.size(); i++) {
		cout << M[i] << " ";
	}
	cout << endl;
	cout << "Shaft rotation speed: " << endl;
	for (int i = 0; i < V.size(); i++) {
		cout << V[i] << " ";
	}
	cout << endl;
	cout << "Temperature of overgeating: " << overheatingT << endl;
	cout << "Heating speed on torque coefficient: " << Hm << endl;
	cout << "Heating speed on rotation speed coefficient: " << Hv << endl;
	cout << "Coefficient of cooling speed on the temperature of the motor and medium: " << C << endl;
	cout << endl;
}
void Motor::simulation(double environmentT)
{
	if (M.size() == V.size() && M.size() != 0) {
		motorIsRuningMutex.lock();
		motorIsRuning = 1;
		motorIsRuningMutex.unlock();

		workingTimeMutex.lock();
		workingTime = 0;						//Time since the start of the simulation
		workingTimeMutex.unlock();
		
		motorVMutex.lock();
		motorV = 0;
		motorVMutex.unlock();

		double Vh = 0;					//Heating rate
		double Vc = 0;					//Cooling rate
		//double currentV = 0;			//Current rotation speed
		double currentM = 0;			//Current moment of inertia
		
		motorTMutex.lock();
		motorT = environmentT;	//Temperature of motor
		motorTMutex.unlock();
		
		motorIsRuningMutex.lock();
		while (motorIsRuning) {
			motorIsRuningMutex.unlock();
			
			motorVMutex.lock();
			currentM = computeM(motorV);
			Vh = currentM * Hm + motorV * motorV * Hv;
			motorV += (currentM / I);	//Because acceleration is a change in speed
		
			motorVMutex.unlock();

			Vc = C * (environmentT - motorT);
			motorTMutex.lock();
			motorT += (Vh + Vc);
			//cout << motorT<<endl;
			motorTMutex.unlock();

			workingTimeMutex.lock();
			workingTime++;
			workingTimeMutex.unlock();

			this_thread::sleep_for(std::chrono::microseconds(10));//For synchronisation

			//Motor ends work when it has max speed of shaft
			if (motorV >= V[V.size() - 1] - eps) {	//Epsilon is subtracted as the moment of inertia may
													//start to approach 0 and since that velocity can't achieve max
				workingTimeMutex.lock();
				workingTime = -1;
				workingTimeMutex.unlock();

				motorIsRuningMutex.lock();
				motorIsRuning = 0;
				motorIsRuningMutex.unlock();

				return;
			}
			motorIsRuningMutex.lock();
		}
		motorIsRuningMutex.unlock();
	}
}


double Motor::computeM(double currentV)
{
	int size = V.size();
	for (int i = 0; i < size - 1; i++) {
		if (currentV == V[i]) return (double)M[i];
		else if (currentV > V[i] && currentV < V[i + 1]) {
			return (M[i] + ((currentV - V[i]) * (M[i + 1] - M[i])) / (V[i + 1] - V[i]));//Calculation using triangle similarity
		}
		else if(currentV==V[size - 1]) return (double)M[size - 1];
	}
	return 0;
}
