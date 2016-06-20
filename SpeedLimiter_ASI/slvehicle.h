#pragma once
#include "..\inc\types.h"
#include <cfloat>
#include <math.h>

class SLVehicle
{
public:
	SLVehicle(const Vehicle vehicle) { this->vehicle = vehicle; }
	~SLVehicle() {}

	Vehicle Instance() { return vehicle; }

	float GetSpeed();

	float MaxSpeed() { return maxSpeed; }
	void MaxSpeed(const float maxSpeed);

	float MaxSpeedMph()
	{
		float mph = GetSpeed() / 0.44704f;
		return roundf(mph);
	}

	float MaxSpeedKmh()
	{
		float kmh = GetSpeed() * 3.6f;
		return roundf(kmh);
	}

	bool IsAlive();

	bool Exists();

	int Health();
private:
	Vehicle vehicle;
	float maxSpeed = FLT_MAX;
};

