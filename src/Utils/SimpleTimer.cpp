#include "include/Utils/SimpleTimer.h"


namespace SE
{

TSimpleTimer::TSimpleTimer()
	: Timer(0)
{
}


void TSimpleTimer::SetTimer(int timer)
{
	Timer = timer;
}

void TSimpleTimer::Update(size_t dt)
{
	if (Timer > 0)
	{
		Timer = max(Timer - static_cast<int>(dt), 0);
	}
}

bool TSimpleTimer::IsOver()
{
	return Timer == 0;
}

boost::posix_time::time_duration TSimpleTimer::GetPosixTime()
{
	return boost::posix_time::seconds(Timer / 1000);
}


} //namespace SE