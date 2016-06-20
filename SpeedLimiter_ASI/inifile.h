#pragma once
#include <string>

using std::string;

class IniFile
{
public:
	IniFile(const LPCSTR path)
	{
		this->path = path;
	}
	~IniFile() {};
	string ReadString(const LPCSTR section, const LPCSTR key, const LPCSTR defaultValue);
	int ReadInt(const LPCSTR section, const LPCSTR key, const INT defaultValue);
private:
	LPCSTR path;
};

