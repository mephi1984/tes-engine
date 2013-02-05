#include "include/Engine.h"

namespace SE
{

const std::string CONST_HOLD_SIGNAL_NAME = "OnHold";
const std::string CONST_CLICK_SIGNAL_NAME = "OnClick";
const std::string CONST_DRAG_SIGNAL_NAME = "OnDrag";


TGUIManager::~TGUIManager()
{

	WidgetTrasfromTaskList.clear();

	WidgetArr.clear();

	*Console<<"GUI Manager deleting...";
}


void TGUIManager::AddWidget(std::shared_ptr<TInstancingWidgetAncestor> widgetAncestor, const std::string& name, const std::string& groupName, int order)
{
	for (TWidgetArr::iterator i = WidgetArr.begin(); i != WidgetArr.end(); i++)
	{
		if (i->Name == name)
		{
			return;
		}
	}
	WidgetArr.push_back(TWidgetStruct(name, groupName, widgetAncestor, order));
	std::stable_sort(WidgetArr.begin(), WidgetArr.end());
}

void TGUIManager::DeleteWidget(std::string name)
{
	for (TWidgetArr::iterator i = WidgetArr.begin(); i != WidgetArr.end(); i++)
	{
		if (i->Name == name)
		{
			WidgetArr.erase(i);
			return;
		}
	}
}

void TGUIManager::DeleteWidgetGroup(std::string groupName)
{
	bool widgetErased;
	for (TWidgetArr::iterator i = WidgetArr.begin(); i != WidgetArr.end();)
	{
		widgetErased = false;
		if (i->GroupName == groupName)
		{
			i = WidgetArr.erase(i);
			widgetErased = true;
		}

		if (i != WidgetArr.end() & !widgetErased)
		{
			++i;
		}
	}
}

void TGUIManager::DeleteWidgetOnUpdate(const std::string& name)
{
	PostUpdateSignal.connect(boost::bind(&TGUIManager::DeleteWidget, this, name));
}

void TGUIManager::DeleteWidgetGroupOnUpdate(const std::string& groupName)
{
	PostUpdateSignal.connect(boost::bind(&TGUIManager::DeleteWidgetGroup, this, groupName));
}



void TGUIManager::AddWidgetTransformTask(TWidgetTransformTask widgetTransformTask)
{
	WidgetTrasfromTaskList.push_back(widgetTransformTask);
}

void TGUIManager::Update(cardinal dt)
{

	WidgetListMutex.lock();

	for (TWidgetArr::iterator i = WidgetArr.begin(); i != WidgetArr.end(); ++i)
	{
		i->Widget->Update(dt);

		if (i->IsMouseDown)
		{
			(*(i->SignalMap[CONST_HOLD_SIGNAL_NAME]))(TSignalParam(dt));

		}
	}

	for (TWidgetTrasfromTaskList::iterator i = WidgetTrasfromTaskList.begin(); i != WidgetTrasfromTaskList.end(); )
	{
		int prevCurrentTime = i->CurrentTime;

		i->CurrentTime += dt;

		if (i->CurrentTime >= i->TotalTime)
		{
			i->CurrentTime = i->TotalTime;
		}

		vec2 oldPos;
		vec2 newPos;

		float t0 = clamp<float>((prevCurrentTime + 0.f)/i->TotalTime, 0, 1);
		float t = clamp<float>((i->CurrentTime + 0.f)/i->TotalTime, 0, 1);

		if (i->MoveStyle == TWidgetTransformTask::MS_LINEAR)
		{
			oldPos = InterpolateLinear<vec2>(vec2(0,0), i->PosTo, t0);
			newPos = InterpolateLinear<vec2>(vec2(0,0), i->PosTo, t);
		}
		if (i->MoveStyle == TWidgetTransformTask::MS_SQR)
		{
			oldPos = InterpolateSqr<vec2>(vec2(0,0), i->PosTo, t0);
			newPos = InterpolateSqr<vec2>(vec2(0,0), i->PosTo, t);
		}
		if (i->MoveStyle == TWidgetTransformTask::MS_SQRT)
		{
			oldPos = InterpolateSqrt<vec2>(vec2(0,0), i->PosTo, t0);
			newPos = InterpolateSqrt<vec2>(vec2(0,0), i->PosTo, t);
		}

		vec2 moveDistance = newPos - oldPos;

		for (TWidgetArr::iterator j = WidgetArr.begin(); j != WidgetArr.end(); ++j)
		{
			if (j->GroupName == i->GroupName)
			{
				MoveWidgetByIterator(j, moveDistance);
			}
		}


		if (i->CurrentTime == i->TotalTime)
		{
			i = WidgetTrasfromTaskList.erase(i);
		}
		else
		{
			++i;
		}
	}

	WidgetListMutex.unlock();

	PostUpdateSignal();

	PostUpdateSignal.disconnect_all_slots();
}

void TGUIManager::Draw()
{
	
	TWidgetArr::iterator i;
	for (i = WidgetArr.begin(); i != WidgetArr.end(); ++i)
	{

		TRenderPairList::iterator j;
		
		for (j = i->Widget->TriangleListVector.begin(); j != i->Widget->TriangleListVector.end(); ++j)
		{

			TRenderParamsSetter paramSetter(j->first);

			Renderer->DrawTriangleList(j->second);

			CheckGlError("TGUIManager::Draw()");

		}
	}
}

TWidgetArr::iterator TGUIManager::FindWidgetInArr(const std::string& widgetName)
{
	TWidgetArr::iterator i;

	for (i = WidgetArr.begin(); i != WidgetArr.end(); ++i)
	{
		if (i->Name == widgetName)
		{
			break;
		}
	}

	if (i == WidgetArr.end())
	{
		throw ErrorToLog("ERROR: widget not found: "+widgetName);
	}

	return i;
}

void TGUIManager::MoveWidgetByIterator(TWidgetArr::iterator widget, vec2 shift)
{
	widget->Widget->LeftBottomPos += shift;

	std::vector<vec3>::iterator i;

	TRenderPairList::iterator itr;

	for (itr = widget->Widget->TriangleListVector.begin(); itr != widget->Widget->TriangleListVector.end(); ++itr)
	{
		std::vector<vec3>& vertexCoordVec = itr->second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB];

		for (i = vertexCoordVec.begin(); i != vertexCoordVec.end(); ++i)
		{
			*i += vec3(shift, 0);
		}

		itr->second.RefreshBuffer();
	}
}

void TGUIManager::MoveWidget(const std::string& widgetName, vec2 shift)
{
	
	TWidgetArr::iterator widget = FindWidgetInArr(widgetName);
	MoveWidgetByIterator(widget, shift);
}

void TGUIManager::MoveWidgetGroup(const std::string& widgetGroupName, const std::string& exceptWidget, vec2 shift)
{
	for (TWidgetArr::iterator i = WidgetArr.begin(); i != WidgetArr.end(); ++i)
	{
		if (i->GroupName == widgetGroupName && i->Name != exceptWidget)
		{
			MoveWidgetByIterator(i, shift);
		}
	}
}

void TGUIManager::OnMouseDown(vec2 pos)
{
	//Xperimental - need to call widget methods and signals	NOT IN "FOR" LOOP

	WidgetListMutex.lock();

	TWidgetArr::reverse_iterator i;

	LastTapPos = pos;
	TotalShift = vec2(0,0);

	for (i = WidgetArr.rbegin(); i != WidgetArr.rend(); ++i)
	{
		if (i->Widget->CheckClick(pos))
		{
			bool isTransparentForInput = i->Widget->IsTransparentForInput();

			i->Widget->OnTapDown(pos);
			i->IsMouseDown = true;
			
			if (! isTransparentForInput)
			{
				WidgetListMutex.unlock();
				return;
			}
		}
	}

	WidgetListMutex.unlock();
}

void TGUIManager::OnMouseUp(vec2 pos)
{
	//Xperimental - need to call widget methods and signals	NOT IN "FOR" LOOP

	WidgetListMutex.lock();

	TWidgetArr::reverse_iterator i;

	for (i = WidgetArr.rbegin(); i != WidgetArr.rend(); ++i)
	{
		if (i->Widget->CheckClick(pos))
		{
			bool isTransparentForInput = i->Widget->IsTransparentForInput();
			i->Widget->OnTapUp(pos);
			i->IsMouseDown = false;
			(*(i->SignalMap[CONST_CLICK_SIGNAL_NAME]))(TSignalParam(pos));
			
			if (! isTransparentForInput)
			{
				WidgetListMutex.unlock();
				return;
			}
		}
	}

	WidgetListMutex.unlock();
}

void TGUIManager::OnMove(vec2 shift)
{
	//Xperimental - need to call widget methods and signals	NOT IN "FOR" LOOP


	TWidgetArr::reverse_iterator i;

	bool moveIsProcessed = false;
	bool moveOutIsProcessed = false;

	TotalShift += shift;

	for (i = WidgetArr.rbegin(); i != WidgetArr.rend(); ++i)
	{
		if (!moveIsProcessed && i->Widget->CheckClick(LastTapPos-TotalShift))
		{
			bool isTransparentForInput = i->Widget->IsTransparentForInput();

			i->Widget->OnMove(shift);
			(*(i->SignalMap[CONST_DRAG_SIGNAL_NAME]))(TSignalParam(shift));

			if (! isTransparentForInput)
			{
				moveIsProcessed = true;
			}
		}
	}

	for (i = WidgetArr.rbegin(); i != WidgetArr.rend(); ++i)
	{
		if (!moveOutIsProcessed && i->Widget->CheckClick(LastTapPos) && !i->Widget->CheckClick(LastTapPos-TotalShift))
		{
			bool isTransparentForInput = i->Widget->IsTransparentForInput();

			i->Widget->OnMoveOut();
			i->IsMouseDown = false;
			if (! isTransparentForInput)
			{
				moveOutIsProcessed = true;
			}
		}
	}
}

void TGUIManager::PrintWidgetList()
{
	TWidgetArr::iterator i;

	for (i = WidgetArr.begin(); i != WidgetArr.end(); ++i)
	{
		std::string widgetRow = i->Name + " (" + tostr(i->Widget->LeftBottomPos.v[0]) + ", " + tostr(i->Widget->LeftBottomPos.v[1]) + ")";
		Console->PrintImmediate(widgetRow);
	}
}


std::shared_ptr<boost::signal<void (TSignalParam)>> TGUIManager::GetOnClickSignal(const std::string& widgetName)
{

	TWidgetArr::iterator i = FindWidgetInArr(widgetName);

	return i->SignalMap[CONST_CLICK_SIGNAL_NAME];
	
}

std::shared_ptr<boost::signal<void (TSignalParam)>> TGUIManager::GetOnHoldSignal(const std::string& widgetName)
{

	TWidgetArr::iterator i = FindWidgetInArr(widgetName);
	
	return i->SignalMap[CONST_HOLD_SIGNAL_NAME];
	
}

std::shared_ptr<boost::signal<void (TSignalParam)>> TGUIManager::GetSignal(const std::string& signalName, const std::string& widgetName)
{

	TWidgetArr::iterator i = FindWidgetInArr(widgetName);
	
	if (i->SignalMap.count(signalName) == 0)
	{
		throw ErrorToLog("Illegal signal: "+signalName);
	}

	return i->SignalMap[signalName];
}

void TGUIManager::SQ_MoveWidget(const SQChar *widgetName, float x, float y)
{
	MoveWidget(std::string(widgetName), vec2(x,y));
}


void TGUIManager::BindFunctions()
{
	ResourceManager->ScriptManager.AddFunction("pwl", 
		TScriptInfo("PrintWidgetList"),
		ResourceManager->GUIManager,
		&TGUIManager::PrintWidgetList);

	ResourceManager->ScriptManager.AddFunction("mw", 
		TScriptInfo("MoveWidget"),
		ResourceManager->GUIManager,
		&TGUIManager::SQ_MoveWidget);
}

} //namespace SE