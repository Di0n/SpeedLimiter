#pragma once
#include <time.h>

class Timer
{
public:
	Timer() {};
	Timer(const double seconds)
	{
		this->seconds = seconds;
	}
	~Timer() {};

	void SetNow(const double seconds)
	{
		this->seconds = seconds;
		time(&timeSet);
	}
	bool TimeElapsed()
	{
		const double diffSeconds = difftime(time(NULL), timeSet);
		return diffSeconds >= seconds ? true : false;
	}
private:
	double seconds;
	time_t timeSet;
};

