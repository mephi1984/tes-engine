#ifndef THREAD_UTILS_IMPL_H_INCLUDED
#define THREAD_UTILS_IMPL_H_INCLUDED

#include "include/Engine.h"


namespace SE
{
	
	template<typename RETURNTYPE>
	RETURNTYPE PerformInMainThread(boost::function<RETURNTYPE()> f)
	{
		if (boost::this_thread::get_id() == ST::MainThreadId)
		{
			return f();
		}
		else
		{
			RETURNTYPE result;

			boost::mutex serviceLock;

			boost::function<void()> func = 
				[&result, &f, &serviceLock] ()
				{
					result = f();
					serviceLock.unlock();
				};

			serviceLock.lock();

            ST::MainThreadIoService.post(func);

			serviceLock.lock();
            serviceLock.unlock();
            
			return result;

		}
	}

} //namespace SE

#endif