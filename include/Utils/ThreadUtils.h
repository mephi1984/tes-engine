#ifndef THREAD_UTILS_H_INCLUDED
#define THREAD_UTILS_H_INCLUDED

#include "boost/asio.hpp"
#include "boost/signals2.hpp"
#include "boost/thread.hpp"


namespace SE
{
    
    namespace ST
    {
        extern boost::asio::io_service MainThreadIoService;
    
        extern boost::thread::id MainThreadId;
    }

	#ifndef UTILS_ENGINE
	
	struct TFuncToPerform
	{
	private:
	public:

		TFuncToPerform()
			: LockerPtr(new boost::mutex)
		{
		}

		std::shared_ptr<boost::mutex> LockerPtr;

		boost::function<void()> Func;
	};	
	void AssertIfInMainThread();

	void TryUpdateMainThreadId();

	void PerformInMainThreadAsync(boost::function<void()> f);
	void PerformInMainThreadAsyncLater(boost::function<void()> f);

	template<typename RETURNTYPE>
	RETURNTYPE PerformInMainThread(boost::function<RETURNTYPE()> f);

#endif

} //namespace SE

#endif
