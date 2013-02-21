#include "include/Engine.h"

namespace SE
{
    
    namespace ST
    {
        boost::asio::io_service MainThreadIoService;
        
        boost::thread::id MainThreadId;
    }
#ifndef UTILS_ENGINE
	void AssertIfInMainThread()
	{
		if (boost::this_thread::get_id() != ST::MainThreadId)
		{
			throw ErrorToLog("ERROR! AssertIfInMainThread - assert failed!");
		}
	}

	void TryUpdateMainThreadId()
	{
		if (boost::this_thread::get_id() != ST::MainThreadId)
		{
			ST::MainThreadId = boost::this_thread::get_id();
		}
	}

	void PerformInMainThreadAsync(boost::function<void()> f)
	{
		
		if (boost::this_thread::get_id() == ST::MainThreadId)
		{
			f();
		}
		else
		{
	
		boost::mutex serviceLock;

		boost::function<void()> func = 
			[&f, &serviceLock] ()
			{
				f();
				serviceLock.unlock();
			};

		
		serviceLock.lock();
        ST::MainThreadIoService.post(func);

		serviceLock.lock();
		serviceLock.unlock();
		}
		


	}
#endif

} //namespace SE