#ifndef GUIMANAGER_H_INCLUDED
#define GUIMANAGER_H_INCLUDED

#include "include/Render/RenderMisc.h"
#include "include/GUIManager/ButtonWidget.h"
#include "include/GUIManager/WidgetXmlParsers.h"
#include "include/Utils/Utils.h"
#include "include/ScriptManager/ScriptManager.h"

#include "boost/variant.hpp"
#include "boost/thread.hpp"

namespace SE
{

extern const std::string CONST_HOLD_SIGNAL_NAME;
extern const std::string CONST_CLICK_SIGNAL_NAME;
extern const std::string CONST_DRAG_SIGNAL_NAME;


typedef boost::variant<cardinal, vec2> TSignalParam;

struct TWidgetStruct
{
	std::string Name;
	std::string GroupName;
	int Order;

	bool IsMouseDown;

	std::shared_ptr<TInstancingWidgetAncestor> Widget;

	std::map<std::string, std::shared_ptr<boost::signal<void (TSignalParam)>>> SignalMap;
	
	TWidgetStruct(const std::string& name, const std::string& groupName, std::shared_ptr<TInstancingWidgetAncestor> widget, int order = 0)
		: Name(name)
		, GroupName(groupName)
		, Order(order)
		, IsMouseDown(false)
		, Widget(widget)
	{
		SignalMap[CONST_HOLD_SIGNAL_NAME] = std::shared_ptr<boost::signal<void (TSignalParam)>>(new boost::signal<void (TSignalParam)>());
		SignalMap[CONST_CLICK_SIGNAL_NAME] = std::shared_ptr<boost::signal<void (TSignalParam)>>(new boost::signal<void (TSignalParam)>());
		SignalMap[CONST_DRAG_SIGNAL_NAME] = std::shared_ptr<boost::signal<void (TSignalParam)>>(new boost::signal<void (TSignalParam)>());
	}

	TWidgetStruct(const TWidgetStruct& widgetStruct)
	{
		Name = widgetStruct.Name;
		Widget = widgetStruct.Widget;
		GroupName = widgetStruct.GroupName;
		IsMouseDown = widgetStruct.IsMouseDown;
		Order = widgetStruct.Order;
		SignalMap = widgetStruct.SignalMap;
	
	}

	TWidgetStruct& operator=(const TWidgetStruct& widgetStruct)
	{
		//Self-assignment is ok
		Name = widgetStruct.Name;
		Widget = widgetStruct.Widget;
		GroupName = widgetStruct.GroupName;
		IsMouseDown = widgetStruct.IsMouseDown;
		Order = widgetStruct.Order;
		SignalMap = widgetStruct.SignalMap;
		
		return *this;
	}

	~TWidgetStruct()
	{
	}

	//For sorting
	bool operator>(const TWidgetStruct& widget) const
	{
		return Order > widget.Order;
	}

	bool operator<(const TWidgetStruct& widget) const
	{
		return Order < widget.Order;
	}

	bool operator==(const TWidgetStruct& widget) const
	{
		return Order == widget.Order;
	}


};


struct TWidgetTransformTask
{
	std::string GroupName;
	vec2 PosTo;
	int CurrentTime;
	int TotalTime;

	enum TMoveStyle
	{
		MS_LINEAR,
		MS_SQR,
		MS_SQRT
	} MoveStyle;

	TWidgetTransformTask(const std::string& groupName, vec2 posTo, int totalTime, int currentTime, TMoveStyle moveStyle)
		: GroupName(groupName)
		, PosTo(posTo)
		, TotalTime(totalTime)
		, CurrentTime(currentTime)
		, MoveStyle(moveStyle)
	{
	}
};

typedef std::list<TWidgetTransformTask> TWidgetTrasfromTaskList;

typedef std::vector<TWidgetStruct> TWidgetArr;

class TGUIManager : public TFunctionBinderInterface
{
protected:
	
	TWidgetArr WidgetArr;

	TWidgetTrasfromTaskList WidgetTrasfromTaskList;

	void MoveWidgetByIterator(TWidgetArr::iterator widget, vec2 shift);

	TWidgetArr::iterator FindWidgetInArr(const std::string& widgetName);

	vec2 LastTapPos;
	vec2 TotalShift;

	boost::mutex WidgetListMutex;

public:

	boost::signal<void(int)> KeyPressedSignal;
	boost::signal<void()> PostUpdateSignal;

	~TGUIManager();
	
	void AddWidget(std::shared_ptr<TInstancingWidgetAncestor> widgetAncestor, const std::string& name, const std::string& groupName, int order = 0);

	void DeleteWidget(std::string name);
	void DeleteWidgetGroup(std::string groupName);

	void DeleteWidgetOnUpdate(const std::string& name);
	void DeleteWidgetGroupOnUpdate(const std::string& groupName);

	void Update(cardinal dt);
	void Draw();

	void MoveWidget(const std::string& widgetName, vec2 shift);
	void MoveWidgetGroup(const std::string& widgetGroupName, const std::string& exceptWidget, vec2 shift);

	void AddWidgetTransformTask(TWidgetTransformTask widgetTransformTask);

	void OnMouseDown(vec2 pos);

	void OnMouseUp(vec2 pos);

	void OnMove(vec2 shift);

	void PrintWidgetList();

	std::shared_ptr<boost::signal<void (TSignalParam)>> GetOnClickSignal(const std::string& widgetName);
	std::shared_ptr<boost::signal<void (TSignalParam)>> GetOnHoldSignal(const std::string& widgetName);
	std::shared_ptr<boost::signal<void (TSignalParam)>> GetSignal(const std::string& signalName, const std::string& widgetName);

	//TFunctionBinderInterface implementation:
	void SQ_MoveWidget(const SQChar *widgetName, float x, float y);
	virtual void BindFunctions();

};

} //namespace SE


#endif