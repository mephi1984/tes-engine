#ifndef SMART_VALUE_MANAGER_H_INCLUDED
#define SMART_VALUE_MANAGER_H_INCLUDED

#include "include/Utils/Utils.h"

namespace SE
{


typedef boost::variant<
	std::shared_ptr<TBindableVar<int>>,
	std::shared_ptr<TBindableVar<float>>,
	std::shared_ptr<TBindableVar<bool>>,
	std::shared_ptr<TBindableVar<std::string>>,
	std::shared_ptr<TBindableVar<vec2>>,
	std::shared_ptr<TBindableVar<vec3>>,
	std::shared_ptr<TBindableVar<vec4>>> TSmartValue;


class TSmartValueManager : public TSerializeInterface
{
protected:

	std::map<std::string, std::map<std::string, TSmartValue>> DisctionariesMap;

public:

	TSmartValue& GetSmartValue(const std::string& dictName, const std::string& valueName);

	template<typename TYPENAME>
	TYPENAME GetImmediateValue(const std::string& dictName, const std::string& valueName)
	{
		return (*boost::get<std::shared_ptr<TBindableVar<TYPENAME>>>(GetSmartValue(dictName, valueName)))();
	}

	template<typename TYPENAME>
	void SetImmediateValue(const std::string& dictName, const std::string& valueName, TYPENAME value)
	{
		*boost::get<std::shared_ptr<TBindableVar<TYPENAME>>>(GetSmartValue(dictName, valueName)) = value;
	}

	template<typename TYPENAME>
	boost::signals2::signal<void(const TYPENAME&)>& GetSignal(const std::string& dictName, const std::string& valueName)
	{
		return boost::get<std::shared_ptr<TBindableVar<TYPENAME>>>(GetSmartValue(dictName, valueName))->OnVarChanged;
	}

	virtual void Serialize(boost::property_tree::ptree& propertyTree);

};


} //namespace SE

#endif
