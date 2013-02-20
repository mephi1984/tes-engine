#ifndef THREAD_UTILS_IMPL_H_INCLUDED
#define THREAD_UTILS_IMPL_H_INCLUDED

#include "include/Engine.h"


namespace SE
{

	template<typename RETURNTYPE>
	struct TCoverFunc
	{
	public:
		typedef void result_type;

		boost::function<RETURNTYPE()> Func;

		void operator()(RETURNTYPE& rx)
		{
			rx = Func();
		}
	};
	
	template<typename RETURNTYPE>
	RETURNTYPE PerformInMainThread(boost::function<RETURNTYPE()> f)
	{
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
		}
		/*
		if (boost::this_thread::get_id() == ResourceManager->MainThreadId)
		{
			return f();
		}
		else
		{
			RETURNTYPE result;

			TCoverFunc<RETURNTYPE> cover_f;

			cover_f.Func = f;

			TFuncToPerform funcToPerform;

			funcToPerform.Executed = false;
			funcToPerform.Func = boost::bind(cover_f, boost::ref(result));
			funcToPerform.LockerPtr->lock();

			ResourceManager->FuncListMutex.lock();
			auto itr = ResourceManager->MainThreadSyncFunctionList.insert(ResourceManager->MainThreadSyncFunctionList.end(), funcToPerform);
			ResourceManager->FuncListMutex.unlock();

			itr->LockerPtr->lock(); //wait until lock will be released
			itr->LockerPtr->unlock();

			ResourceManager->FuncListMutex.lock();
			ResourceManager->MainThreadSyncFunctionList.erase(itr);
			ResourceManager->FuncListMutex.unlock();


			return result;

		}*/
	}

} //namespace SE

#endif