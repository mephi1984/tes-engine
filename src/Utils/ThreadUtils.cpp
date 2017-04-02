#include "include/Engine.h"

//#include "boost/thread/condition_variable.hpp"

#include <condition_variable>

namespace SE
{

    namespace ST
    {
        boost::asio::io_service MainThreadIoService;
        
        std::thread::id MainThreadId;

		std::condition_variable FunctionFinishedCondition;
			
		std::mutex FunctionMutex;
    }


#ifndef UTILS_ENGINE
	void AssertIfInMainThread()
	{
		if (std::this_thread::get_id() != ST::MainThreadId)
		{
			throw ErrorToLog("ERROR! AssertIfInMainThread - assert failed!");
		}
	}

	void TryUpdateMainThreadId()
	{
		if (std::this_thread::get_id() != ST::MainThreadId)
		{
			ST::MainThreadId = std::this_thread::get_id();
		}
	}

	void PerformInMainThreadAsync(std::function<void()> f)
	{
		
		if (std::this_thread::get_id() == ST::MainThreadId)
		{
			f();
		}
		else
		{
	
        ST::MainThreadIoService.post(f);

		}
		
	}

	void PerformInMainThreadAsyncLater(std::function<void()> f)
	{
        ST::MainThreadIoService.post(f);	
	}
#endif

} //namespace SE