#include "include/Engine.h"

namespace SE
{

	void AssertIfInMainThread()
	{
		if (boost::this_thread::get_id() != ResourceManager->MainThreadId)
		{
			throw ErrorToLog("ERROR! AssertIfInMainThread - assert failed!");
		}
	}

	void TryUpdateMainThreadId()
	{
		if (boost::this_thread::get_id() != ResourceManager->MainThreadId)
		{
			ResourceManager->MainThreadId = boost::this_thread::get_id();
		}
	}

	void PerformInMainThreadAsync(boost::function<void()> f)
	{
		if (boost::this_thread::get_id() == ResourceManager->MainThreadId)
		{
			f();
		}
		else
		{
			ResourceManager->MainThreadAsyncFunctionArr.push_back(f);
		}
	}

} //namespace SE