#ifndef THREAD_UTILS_H_INCLUDED
#define THREAD_UTILS_H_INCLUDED

#include "boost/asio.hpp"
#include "boost/signal.hpp"
#include "boost/thread.hpp"


namespace SE
{

	extern boost::asio::io_service MainThreadIoService;

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

	template<typename RETURNTYPE>
	RETURNTYPE PerformInMainThread(boost::function<RETURNTYPE()> f);

#endif

} //namespace SE

#endif