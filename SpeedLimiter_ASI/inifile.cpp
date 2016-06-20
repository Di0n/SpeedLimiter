//#include "stdafx.h"
#include "stdafx.h"
#include "inifile.h"

string IniFile::ReadString(const LPCSTR section, const LPCSTR key, const LPCSTR defaultValue)
{
	char result[255];
	memset(result, 0x00, 255);
	GetPrivateProfileString(section, key, defaultValue, result, 255, path);
	return result;
}

int IniFile::ReadInt(const LPCSTR section, const LPCSTR key, const INT defaultValue)
{
	return GetPrivateProfileInt(section, key, defaultValue, path);
}
