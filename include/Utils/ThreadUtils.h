#ifndef THREAD_UTILS_H_INCLUDED
#define THREAD_UTILS_H_INCLUDED

#include "boost/signal.hpp"
#include "boost/thread.hpp"


namespace SE
{

	struct TFuncToPerform
	{
	private:
	public:

		TFuncToPerform()
			: LockerPtr(new boost::mutex)
		{
		}

		bool Executed;

		std::shared_ptr<boost::mutex> LockerPtr;

		boost::function<void()> Func;
	};
	
	void AssertIfInMainThread();

	void TryUpdateMainThreadId();

	void PerformInMainThreadAsync(boost::function<void()> f);

	template<typename RETURNTYPE>
	RETURNTYPE PerformInMainThread(boost::function<RETURNTYPE()> f);

} //namespace SE

#endif