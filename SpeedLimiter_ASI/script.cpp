#include "script.h"
#include "keyboard.h"
#include "timer.h"
#include "slvehicle.h"
#include "inifile.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

bool Initialize();
void KeyEvent(), Update(), CleanupVehicleVec();
void LimitSpeedKey_Pressed(), AddSpeedKey_Pressed(), SubtractSpeedKey_Pressed(), ToggleModKey_Pressed();

struct Keys
{
	DWORD limitSpeedKey;
	DWORD addSpeedKey;
	DWORD subtractSpeedKey;
	DWORD toggleModKey;
};

struct Message
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
string unit;


void Update()
{
	if (timer.TimeElapsed())
	{
		CleanupVehicleVec();
		timer.SetNow(500);
	}
}

void KeyEvent()
{
	if (IsKeyJustUp(keys.limitSpeedKey))
		LimitSpeedKey_Pressed();

	if (IsKeyJustUp(keys.addSpeedKey))
		AddSpeedKey_Pressed();

	if (IsKeyJustUp(keys.subtractSpeedKey))
		SubtractSpeedKey_Pressed();

	if (IsKeyJustUp(keys.toggleModKey))
		ToggleModKey_Pressed();
}

void LimitSpeedKey_Pressed()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && !PED::IS_PED_DEAD_OR_DYING(playerPed, 1))
	{
		SLVehicle curVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		for (int i = 0; i < vehicles.size(); i++)
		{
			if (vehicles[i].Instance() == curVehicle.Instance())
			{
				vehicles[i].MaxSpeed(FLT_MAX);
				vehicles.erase(vehicles.begin() + i);
				Message::ShowSubtitle("Removed Limiter.", 2000);
				return;
			}
		}

		SLVehicle vehicle = curVehicle;
		vehicle.MaxSpeed(vehicle.GetSpeed());
		vehicles.push_back(vehicle);

		Message::ShowSubtitle("Limiter set.", 2000);
	}

	else
		Message::ShowNotification("You are currently ~r~not~s~ in a vehicle!");
}

void AddSpeedKey_Pressed()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && !PED::IS_PED_DEAD_OR_DYING(playerPed, 1))
	{
		SLVehicle curVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		for (SLVehicle & vehicle : vehicles)
			if (vehicle.Instance() == curVehicle.Instance())
			{
				if (vehicle.MaxSpeed() + 1 > FLT_MAX) return;
				vehicle.MaxSpeed(vehicle.MaxSpeed() + 1);

				Message::ShowSubtitle("Limiting speed to: ~b~" + std::to_string(unit == "MPH" ? vehicle.MaxSpeedMph() : vehicle.MaxSpeedKmh()) + "~s~ " + unit, 2000);
			}
	}
	else
		Message::ShowNotification("You are currently ~r~not~s~ in a vehicle!");
}

void SubtractSpeedKey_Pressed()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && !PED::IS_PED_DEAD_OR_DYING(playerPed, 1))
	{
		SLVehicle curVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		for (SLVehicle & vehicle : vehicles)
			if (vehicle.Instance() == curVehicle.Instance())
			{
				if (vehicle.MaxSpeed() - 1 < 0) return;
				vehicle.MaxSpeed(vehicle.MaxSpeed() - 1);

				Message::ShowSubtitle("Limiting speed to: ~b~" + std::to_string(unit == "MPH" ? vehicle.MaxSpeedMph() : vehicle.MaxSpeedKmh()) + "~s~ " + unit, 2000);
			}
	}
}

void ToggleModKey_Pressed()
{
	modEnabled = !modEnabled;
	const string status = modEnabled ? "Enabled" : "Disabled";
	Message::ShowNotification(status + " ~b~SpeedLimiter.asi~s~");
}

void CleanupVehicleVec()
{
	for (int i(0); i < vehicles.size(); i++)
	{
		if (!vehicles[i].Exists())
			vehicles.erase(vehicles.begin() + i);
	}
}

bool Initialize()
{
	modEnabled = true;

	try
	{
		IniFile ini = "./speedlimiter.ini";
		keys.toggleModKey = ini.ReadInt("KEYS", "ENABLE_MOD", VK_PRIOR); // Page UP
		keys.limitSpeedKey = ini.ReadInt("KEYS", "TOGGLE_LIMITER", 0x38); // 8
		keys.addSpeedKey = ini.ReadInt("KEYS", "ADD_SPEED", 0x39); // 9
		keys.subtractSpeedKey = ini.ReadInt("KEYS", "SUBTRACT_SPEED", 0x30); // 0
		unit = ini.ReadString("UNIT", "SPEED_UNIT", "KMH");
		for (char & c : unit) toupper(c);
	}
	catch (...)
	{
		Message::ShowNotification("~b~SpeedLimiter~s~ failed to load ini file!");
		return false;
	}

	Message::ShowNotification("~b~SpeedLimiter~s~ successfully loaded!");
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