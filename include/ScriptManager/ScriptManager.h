#ifndef SCRIPT_MANAGER_H_INCLUDED
#define SCRIPT_MANAGER_H_INCLUDED

#include "sqplus.h"
#include <iostream>
#include <string>

namespace SE
{

struct TScriptInfo
{
	std::string LongName;
	std::string ShortDescription;
	std::string Description;
	bool Registered;

	TScriptInfo()
		: Registered(false)
	{
	}

	TScriptInfo(const std::string& longName, const std::string& shortDescription = "", const std::string& description = "")
		: LongName(longName)
		, ShortDescription(shortDescription)
		, Description(description)
		, Registered(false)
	{
	}
};


class TFunctionBinderInterface
{
public:
	static std::vector<TFunctionBinderInterface*> BinderList;

	TFunctionBinderInterface()
	{
		BinderList.push_back(this);
	}

	virtual void BindFunctions() = 0;
};


class TScriptManager : public TSerializeInterface
{
protected:	
	HSQUIRRELVM virtualMachine;
	std::map<std::string, TScriptInfo> FuncInfoMap;

	template<typename T1,typename T2>
	void RegisterFunc(T1& object, T2 method, const std::string& funcName)
	{
		SqPlus::RegisterGlobal(virtualMachine, object, method, _SC(funcName.c_str()));
	}

	template<typename T>
	void RegisterFunc(T globalFunc, const std::string& funcName)
	{
		SqPlus::RegisterGlobal(virtualMachine, globalFunc, _SC(funcName.c_str()));
	}

public:
	TScriptManager();
	~TScriptManager();

	void AddFuncDescription(const std::string& shortName, TScriptInfo scriptInfo);

	void RunScript(const std::string& scriptCode);

	void BindBasicFunctions();
	
	template<typename T1, typename T2>
	void AddFunction(const std::string& shortName, TScriptInfo scriptInfo, T1& object, T2 method)
	{
		AddFuncDescription(shortName, scriptInfo);
		RegisterFunc(object, method, shortName);
		RegisterFunc(object, method, scriptInfo.LongName);
		FuncInfoMap[shortName].Registered = true;
	}


	template<typename T>
	void AddFunction(const std::string& shortName, TScriptInfo scriptInfo, T globalFunc)
	{
		AddFuncDescription(shortName, scriptInfo);
		RegisterFunc(globalFunc, shortName);
		RegisterFunc(globalFunc, scriptInfo.LongName);
		FuncInfoMap[shortName].Registered = true;
	}

	void BindFunctionsFromObject(TFunctionBinderInterface* binderObject);

	void PrintRegisteredFunctionList();

	void PrintFunctionInfo(const std::string& funcName);

	virtual void Serialize(boost::property_tree::ptree& propertyTree);

};



void SQ_Print(const SQChar *inString);
void SQ_PrintWidgetList();
void SQ_MoveWidget(const SQChar *widgetName, float x, float y);
//void SQ_PrintTextureList();
//void SQ_PrintAnimationList();

//void SQ_MoveWidgetTransformTaskTest(const SQChar *groupName, float x, float y, cardinal totalTime);


} //namespace SE

#endif