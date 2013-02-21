#ifndef THREAD_UTILS_IMPL_H_INCLUDED
#define THREAD_UTILS_IMPL_H_INCLUDED

#include "include/Engine.h"


namespace SE
{
	
	template<typename RETURNTYPE>
	RETURNTYPE PerformInMainThread(boost::function<RETURNTYPE()> f)
	{
		/*
		if (boost::this_thread::get_id() == ResourceManager->MainThreadId)
		{
			return f();
		}
		else
		{
			RETURNTYPE result;
            
            boost::mutex ServiceLock;

            ServiceLock.lock();
            
			boost::function<void()> cover_f = [&result, &ServiceLock, f]()
            {
                result = f();
                ServiceLock.unlock();
            };

			MainThreadIoService.post(cover_f);
            
            ServiceLock.lock();
            ServiceLock.unlock();
            
			return result;
		}*/
		
		if (boost::this_thread::get_id() == ResourceManager->MainThreadId)
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

			MainThreadIoService.post(func);

			serviceLock.lock();
            serviceLock.unlock();
            
			return result;

		}
	}

} //namespace SE

#endif