#ifndef SIMPLE_TIMER_H_INCLUDED
#define SIMPLE_TIMER_H_INCLUDED

#include "include/Utils/DataTypes/DataTypes.h"

#include "boost/date_time/posix_time/posix_time.hpp"

namespace SE
{

class TSimpleTimer
{
protected:
	int Timer;
public:

	TSimpleTimer();

	void SetTimer(int timer);
	void Update(cardinal dt);
	bool IsOver();

	boost::posix_time::time_duration GetPosixTime();

};

} //namespace SE


#endif