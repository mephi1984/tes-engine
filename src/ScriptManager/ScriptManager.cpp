#include "include/Engine.h"
#include "include/ScriptManager/ScriptManager.h"


namespace SE
{

std::vector<TFunctionBinderInterface*> TFunctionBinderInterface::BinderList;


TScriptManager::TScriptManager()
{
	//SquirrelVM::Init();

	//virtualMachine = SquirrelVM::GetVMPtr();

	//SqPlus::RegisterGlobal(virtualMachine, SQ_Print, _SC("Print"));
	//SqPlus::RegisterGlobal(virtualMachine, SQ_Print, _SC("p"));

	//boost::function<void()> f = xxx;

	/*
	SqPlus::RegisterGlobal(virtualMachine, ResourceManager->GUIManager, &TGUIManager::PrintWidgetList, _SC("PrintWidgetList"));
	//SqPlus::RegisterGlobal(virtualMachine, SQ_PrintWidgetList, _SC("pwl"));
	//SqPlus::RegisterGlobal(virtualMachine, Tm<boost::function<void()> >(f), &Tm<boost::function<void()> >::operator(), _SC("pwl"));
	RegisterFunc(ResourceManager->GUIManager, &TGUIManager::PrintWidgetList, "");
	*/

	/*
	AddFunction("pwl", 
		TScriptInfo("PrintWidgetList", "Print out list of widgets registered in system", "Print out list of widgets registered in system"),
		ResourceManager->GUIManager,
		&TGUIManager::PrintWidgetList);*/

	/*
	AddFuncDescription("pwl", TScriptInfo("PrintWidgetList", "Print out list of widgets registered in system", "Print out list of widgets registered in system"));
	SqPlus::RegisterGlobal(virtualMachine, ResourceManager->GUIManager, &TGUIManager::PrintWidgetList, _SC("PrintWidgetList"));
	SqPlus::RegisterGlobal(virtualMachine, ResourceManager->GUIManager, &TGUIManager::PrintWidgetList, _SC("pwl"));

	SqPlus::RegisterGlobal(virtualMachine, SQ_MoveWidget, _SC("MoveWidget"));
	SqPlus::RegisterGlobal(virtualMachine, SQ_MoveWidget, _SC("mw"));

	SqPlus::RegisterGlobal(virtualMachine, SQ_MoveWidgetTransformTaskTest, _SC("MoveWidgetTransformTaskTest"));
	SqPlus::RegisterGlobal(virtualMachine, SQ_MoveWidgetTransformTaskTest, _SC("mwttt"));

	SqPlus::RegisterGlobal(virtualMachine, SQ_PrintTextureList, _SC("PrintTextureList"));
	SqPlus::RegisterGlobal(virtualMachine, SQ_PrintTextureList, _SC("ptl"));

	SqPlus::RegisterGlobal(virtualMachine, SQ_PrintAnimationList, _SC("PrintAnimationList"));
	SqPlus::RegisterGlobal(virtualMachine, SQ_PrintAnimationList, _SC("pal"));
	*/

	
	*Console<<"Script manager ready";
}


TScriptManager::~TScriptManager()
{
	*Console<<"Script manager shutdown";
	//SquirrelVM::Shutdown();
}

void TScriptManager::RunScript(const std::string& scriptCode)
{
	/*
	try
	{
		SquirrelObject m = SquirrelVM::CompileBuffer(_T(scriptCode.c_str()));

		SquirrelVM::RunScript(m);
	}
	catch (SquirrelError)
	{
		*Console<<"Script failed to run";
	}
	*/
}


void TScriptManager::BindBasicFunctions()
{
	//Hack =D
	/*
	static bool alreadyBind = false;
	
	if (!alreadyBind)
	{
	*Console<<"Bind basic functions";

	AddFunction("p", TScriptInfo("Print"), SQ_Print);

	AddFunction("help", TScriptInfo("Help"), *this, &TScriptManager::PrintRegisteredFunctionList);

	AddFunction("man", TScriptInfo("Manual"), *this, &TScriptManager::PrintFunctionInfo);

	std::vector<TFunctionBinderInterface*>::iterator i;

	for (i = TFunctionBinderInterface::BinderList.begin(); i != TFunctionBinderInterface::BinderList.end(); ++i)
	{

		(*i)->BindFunctions();
	}

	alreadyBind = true;
	}

*/

	
}


void TScriptManager::AddFuncDescription(const std::string& shortName, TScriptInfo scriptInfo)
{
	/*
	if (FuncInfoMap.count(shortName) > 0)
	{
		//If this description is already exists
		scriptInfo.Registered = FuncInfoMap[shortName].Registered;
	}
	
	FuncInfoMap[shortName] = scriptInfo;*/


}


void TScriptManager::BindFunctionsFromObject(TFunctionBinderInterface* binderObject)
{
	//binderObject->BindFunctions();
}


void TScriptManager::PrintRegisteredFunctionList()
{
	/*
	std::map<std::string, TScriptInfo>::iterator i;

	Console->PrintImmediate("========== Console function list ===========");

	for (i = FuncInfoMap.begin(); i != FuncInfoMap.end(); ++i)
	{
		if (i->second.Registered)
		{
			Console->PrintImmediate(i->first + " | " + i->second.ShortDescription);
		}
	}*/
}

void TScriptManager::PrintFunctionInfo(const std::string& funcName)
{
	/*
	if (FuncInfoMap.count(funcName) > 0 && FuncInfoMap[funcName].Registered)
	{
		Console->PrintImmediate( funcName + " | " + FuncInfoMap[funcName].LongName + " | " + FuncInfoMap[funcName].Description);
	}*/
}

void TScriptManager::Serialize(boost::property_tree::ptree& propertyTree)
{
	/*
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree.get_child("FunctionInfoList"))
	{
		std::string shortName = v.second.get<std::string>("ShortName");
		std::string longName = v.second.get<std::string>("LongName");
		std::string shortDescription = v.second.get<std::string>("ShortDescription");
		std::string description = v.second.get<std::string>("Description");
		
		AddFuncDescription(shortName, TScriptInfo(longName, shortDescription, description));
	}*/
}


//=============================================
//============= Typical commands ==============
//=============================================
/*
void SQ_Print(const SQChar *inString)
{
	Console->PrintImmediate(std::string(inString));
}

void SQ_PrintWidgetList()
{
	ResourceManager->GUIManager.PrintWidgetList();
}

void SQ_MoveWidget(const SQChar *widgetName, float x, float y)
{
	ResourceManager->GUIManager.MoveWidget(std::string(widgetName), vec2(x,y));
}

void SQ_MoveWidgetTransformTaskTest(const SQChar *groupName, float x, float y, cardinal totalTime)
{
	ResourceManager->GUIManager.AddWidgetTransformTask(TWidgetTransformTask(std::string(groupName), vec2(x,y), totalTime, 0, TWidgetTransformTask::MS_SQRT));
}


void SQ_PrintTextureList()
{
	ResourceManager->TexList.PrintTextureList();
}

void SQ_PrintAnimationList()
{
	ResourceManager->HalibutAnimationManager.PrintAnimationList();
}*/

} //namespace SE