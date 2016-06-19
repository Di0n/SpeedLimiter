#include "script.h"
#include "keyboard.h"
#include "timer.h"
#include "slvehicle.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

bool Initialize();
void KeyEvent();
void Update();
void CleanupVehicleVec();

struct Keys
{
	DWORD limitSpeedKey;
	DWORD addSpeedKey;
	DWORD subtractSpeedKey;
	DWORD enableModKey;
};

struct GUI
{
	static void ShowSubtitle(const string& text, const int duration)
	{
		string str;
		vector<char> writeable(str.begin(), str.end());
		writeable.push_back('\0');
		UI::_SET_TEXT_ENTRY_2("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING(&writeable[0]);
		UI::_DRAW_SUBTITLE_TIMED(duration, true);
	}
	static void ShowNotification(const string& text)
	{
		string str;
		vector<char> writeable(str.begin(), str.end());
		writeable.push_back('\0');
		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING3(&writeable[0]);
		UI::_DRAW_NOTIFICATION(false, false);
	}
};

Timer timer;
Keys keys;
vector<SLVehicle> vehicles;
bool modEnabled;


void Update()
{
	if (timer.TimeElapsed())
	{
		CleanupVehicleVec();
		timer.SetNow(500000);
	}
}

void KeyEvent()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (IsKeyJustUp(keys.limitSpeedKey))
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && !PED::IS_PED_DEAD_OR_DYING(playerPed, 1))
		{
			SLVehicle curVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

			for (int i = 0; i < vehicles.size(); i++)
			{
				if (vehicles[i].Instance() == curVehicle.Instance())
				{
					vehicles[i].MaxSpeed(FLT_MAX);
					vehicles.erase(vehicles.begin() + i);
					GUI::ShowSubtitle("Removed Limiter", 2000);
					return;
				}
			}

			SLVehicle vehicle = curVehicle;
			vehicle.MaxSpeed(vehicle.GetSpeed());
			vehicles.push_back(vehicle);
		}

		else
			GUI::ShowNotification("~r~You are currently not in a vehicle!");

	}

	else if (IsKeyJustUp(keys.addSpeedKey))
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && !PED::IS_PED_DEAD_OR_DYING(playerPed, 1))
		{
			SLVehicle curVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

			for (SLVehicle & vehicle : vehicles)
				if (vehicle.Instance() == curVehicle.Instance())
				{
					vehicle.MaxSpeed(vehicle.MaxSpeed() + 1);
					// Print
				}
		}
		else
			GUI::ShowNotification("You are currently not in a vehicle!");
	}

	else if (IsKeyJustUp(keys.subtractSpeedKey))
	{

	}

	if (IsKeyJustUp(keys.enableModKey))
	{

	}

}

void CleanupVehicleVec()
{
	for (int i(0); i < vehicles.size(); i++)
	{
		if (!vehicles[i].Exists())
			vehicles.erase(vehicles.begin()+i);
	}
}

bool Initialize()
{
	modEnabled = true;

	keys.enableModKey = VK_ACCEPT;
	keys.addSpeedKey = VK_F1;
	keys.limitSpeedKey = VK_F2;
	keys.subtractSpeedKey = VK_F3;
	return true;
}

void main()
{
	if (!Initialize()) return;
	while (true)
	{
		Update();
		KeyEvent();
		WAIT(0);
	}
}

void ScriptMain()
{
	main();
}