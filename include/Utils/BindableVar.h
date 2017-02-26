#ifndef BINDABLE_VAR_H_INCLUDED
#define BINDABLE_VAR_H_INCLUDED

#include "boost/signals2.hpp"
#include "boost/variant.hpp"


namespace SE
{


//Must be stored in shared_ptr:

template<typename TYPENAME>
class TBindableVar
{
protected:
	TYPENAME Variable;

public:

    boost::signals2::signal<void(const TYPENAME&)> OnVarChanged;

	TBindableVar()
	{
	}

	TBindableVar(const TYPENAME&& var)
		: Variable(var)
	{
	}

	operator TYPENAME()
	{
		return Variable;
	}

	TYPENAME operator()()
	{
		return Variable;
	}

	TBindableVar<TYPENAME>& operator=(const TYPENAME& var)
	{
		Variable = var;
		OnVarChanged(Variable);
		return *this;
	}

};


} //namespace SE

#endif
