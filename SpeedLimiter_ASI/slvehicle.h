#pragma once
#include "..\inc\types.h"
#include <cfloat>

class SLVehicle
{
public:
	SLVehicle(const Vehicle vehicle) { this->vehicle = vehicle; }
	~SLVehicle() {}

	Vehicle Instance() { return vehicle; }

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

