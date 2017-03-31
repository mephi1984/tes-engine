#ifndef THREAD_UTILS_IMPL_H_INCLUDED
#define THREAD_UTILS_IMPL_H_INCLUDED

#include "include/Engine.h"

#include <thread>


namespace SE
{

	namespace ST
	{
		extern std::condition_variable FunctionFinishedCondition;
			
		extern std::mutex FunctionMutex;
	}

	
	template<typename RETURNTYPE>
	RETURNTYPE PerformInMainThread(std::function<RETURNTYPE()> f)
	{
		if (std::this_thread::get_id() == ST::MainThreadId)
		{
			return f();
		}
		else
		{
			RETURNTYPE result;

			bool functionCalled = false;
			
			std::function<void()> func =
				[&] ()
				{
					result = f();
                    
					{   
						//std::mutex::scoped_lock lock(ST::FunctionMutex);
						ST::FunctionMutex.lock();
						functionCalled = true;
						ST::FunctionMutex.unlock();
					}
					ST::FunctionFinishedCondition.notify_one();
				};


            ST::MainThreadIoService.post(func);

			//std::mutex::scoped_lock lock(ST::FunctionMutex);

			std::unique_lock<std::mutex> lock(ST::FunctionMutex);
			//ST::FunctionMutex.lock();

			//std::lock(lock);

			while (!functionCalled)
			{
				ST::FunctionFinishedCondition.wait(lock);
			}

			//ST::FunctionMutex.unlock();
            
			return result;

		}
	}

} //namespace SE

#endif