#pragma once
#include "..\inc\types.h"
#include <cfloat>

class SLVehicle
{
public:
	SLVehicle(const Vehicle vehicle) { Set(vehicle); }
	~SLVehicle() {}

	Vehicle GetVehicleInstance() { return vehicle; }
	void Set(const Vehicle vehicle) { this->vehicle = vehicle; }
	float GetSpeed();
	float MaxSpeed() { return maxSpeed; }
	void MaxSpeed(const float maxSpeed);
	bool IsAlive();
	bool Exists();
	int Health();
private:
	Vehicle vehicle;
	float maxSpeed = FLT_MAX;
};

