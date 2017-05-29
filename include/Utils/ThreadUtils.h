#ifndef THREAD_UTILS_H_INCLUDED
#define THREAD_UTILS_H_INCLUDED

#include "boost/asio.hpp"
#include "boost/signals2.hpp"
#include <thread>

#include <mutex>

namespace SE
{
    
    namespace ST
    {
        extern boost::asio::io_service MainThreadIoService;
    
        extern std::thread::id MainThreadId;
    }

	#ifndef UTILS_ENGINE
	
	struct TFuncToPerform
	{
	private:
	public:

		TFuncToPerform()
			: LockerPtr(new std::mutex)
		{
		}

		std::shared_ptr<std::mutex> LockerPtr;

		boost::function<void()> Func;
	};	
	void AssertIfInMainThread();

	void TryUpdateMainThreadId();

	void PerformInMainThreadAsync(std::function<void()> f);
	void PerformInMainThreadAsyncLater(std::function<void()> f);

	template<typename RETURNTYPE>
	RETURNTYPE PerformInMainThread(std::function<RETURNTYPE()> f);

#endif

} //namespace SE

#endif
