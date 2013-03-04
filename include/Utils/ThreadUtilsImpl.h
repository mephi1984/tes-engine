#ifndef THREAD_UTILS_IMPL_H_INCLUDED
#define THREAD_UTILS_IMPL_H_INCLUDED

#include "include/Engine.h"


namespace SE
{

	namespace ST
	{
		extern boost::condition_variable FunctionFinishedCondition;
			
		extern boost::mutex FunctionMutex;
	}

	
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

			bool functionCalled = false;
			
			boost::function<void()> func = 
				[&] ()
				{
					result = f();
                    
					{   
						boost::mutex::scoped_lock lock(ST::FunctionMutex);
						functionCalled = true;
					}
					ST::FunctionFinishedCondition.notify_one();
				};


            ST::MainThreadIoService.post(func);

			boost::mutex::scoped_lock lock(ST::FunctionMutex);

			while (!functionCalled)
			{
				ST::FunctionFinishedCondition.wait(lock);
			}
            
			return result;

		}
	}

} //namespace SE

#endif