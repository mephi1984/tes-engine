#include "include/Engine.h"

namespace SE
{

    namespace ST
    {
        boost::asio::io_service MainThreadIoService;
        
        boost::thread::id MainThreadId;

		boost::condition_variable FunctionFinishedCondition;
			
		boost::mutex FunctionMutex;
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
	
        ST::MainThreadIoService.post(f);

		}
		
	}

	void PerformInMainThreadAsyncLater(boost::function<void()> f)
	{
        ST::MainThreadIoService.post(f);	
	}
#endif

} //namespace SE