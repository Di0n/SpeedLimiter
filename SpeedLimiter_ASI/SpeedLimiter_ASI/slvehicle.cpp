#include "slvehicle.h"
#include "..\inc\natives.h"

float SLVehicle::GetSpeed()
{
	if (!Exists()) return 0;

	float speed = ENTITY::GET_ENTITY_SPEED(vehicle);
	float metersPerHour = speed * 3600;
	return metersPerHour / (float)1609.344;
}

void SLVehicle::MaxSpeed(const float maxSpeed)
{
	if (!Exists()) return;

	this->maxSpeed = maxSpeed;
	VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(vehicle, 2); // ?
	ENTITY::SET_ENTITY_MAX_SPEED(vehicle, this->maxSpeed);
}

bool SLVehicle::Exists()
{
	return ENTITY::DOES_ENTITY_EXIST(vehicle);
}
bool SLVehicle::IsAlive()
{
	if (!Exists()) return false;
	
	return !ENTITY::IS_ENTITY_DEAD(vehicle);
}

int SLVehicle::Health()
{
	if (!Exists()) return 0;

	return ENTITY::GET_ENTITY_HEALTH(vehicle);
}