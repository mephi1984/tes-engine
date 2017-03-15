#ifndef GUIMANAGER_H_INCLUDED
#define GUIMANAGER_H_INCLUDED

#include "include/Render/RenderMisc.h"
#include "include/GUIManager/ButtonWidget.h"
#include "include/GUIManager/KeyboardWidget.h"
#include "include/GUIManager/WidgetXmlParsers.h"
#include "include/Utils/Utils.h"

#include "boost/variant.hpp"
#include "boost/thread.hpp"

namespace SE
{

extern const std::string CONST_HOLD_SIGNAL_NAME;
extern const std::string CONST_CLICK_SIGNAL_NAME;
extern const std::string CONST_DRAG_SIGNAL_NAME;
extern const std::string CONST_TAPDOWN_SIGNAL_NAME;


typedef boost::variant<size_t, Vector2f> TSignalParam;

struct TWidgetStruct
{
	std::string Name;
	std::string GroupName;
	int Order;

	bool IsMouseDown;

	std::shared_ptr<TInstancingWidgetAncestor> Widget;

	std::map<std::string, std::shared_ptr<boost::signals2::signal<void (TSignalParam)>>> SignalMap;
	
	TWidgetStruct(const std::string& name, const std::string& groupName, std::shared_ptr<TInstancingWidgetAncestor> widget, int order = 0)
		: Name(name)
		, GroupName(groupName)
		, Order(order)
		, IsMouseDown(false)
		, Widget(widget)
	{
		SignalMap[CONST_HOLD_SIGNAL_NAME] = std::shared_ptr<boost::signals2::signal<void (TSignalParam)>>(new boost::signals2::signal<void (TSignalParam)>());
		SignalMap[CONST_CLICK_SIGNAL_NAME] = std::shared_ptr<boost::signals2::signal<void (TSignalParam)>>(new boost::signals2::signal<void (TSignalParam)>());
        SignalMap[CONST_TAPDOWN_SIGNAL_NAME] = std::shared_ptr<boost::signals2::signal<void (TSignalParam)>>(new boost::signals2::signal<void (TSignalParam)>());
		SignalMap[CONST_DRAG_SIGNAL_NAME] = std::shared_ptr<boost::signals2::signal<void (TSignalParam)>>(new boost::signals2::signal<void (TSignalParam)>());
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
	Vector2f PosTo;
	int CurrentTime;
	int TotalTime;

	enum TMoveStyle
	{
		MS_LINEAR,
		MS_SQR,
		MS_SQRT
	} MoveStyle;

	TWidgetTransformTask(const std::string& groupName, Vector2f posTo, int totalTime, int currentTime, TMoveStyle moveStyle)
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

class TGUIManager
{
protected:
	
	TWidgetArr WidgetArr;

	TWidgetTrasfromTaskList WidgetTrasfromTaskList;

	void MoveWidgetByIterator(TWidgetArr::iterator widget, Vector2f shift);

	TWidgetArr::iterator FindWidgetInArr(const std::string& widgetName);

    std::map<int, Vector2f> LastTapPos;
	std::map<int, Vector2f> TotalShift;

	boost::mutex WidgetListMutex;
public:
    bool KeyboardIsOnScreen;

	boost::signals2::signal<void(int)> KeyPressedSignal;
    boost::signals2::signal<void(std::string)> SetTextSignal;
	boost::signals2::signal<void()> OnKeyboardShowSignal;
	boost::signals2::signal<void()> OnKeyboardHideSignal;

	TGUIManager();

	~TGUIManager();
	
	void AddWidget(std::shared_ptr<TInstancingWidgetAncestor> widgetAncestor, const std::string& name, const std::string& groupName, int order = 0);

	void DeleteWidget(std::string name);
	void DeleteWidgetGroup(std::string groupName);

	void DeleteWidgetOnUpdate(const std::string& name);
	void DeleteWidgetGroupOnUpdate(const std::string& groupName);
	void DeleteWidgetLaterOnUpdate(const std::string& name);
	void DeleteWidgetGroupLaterOnUpdate(const std::string& groupName);

	void Update(size_t dt);
	void Draw();

	void MoveWidget(const std::string& widgetName, Vector2f shift);
	void MoveWidgetGroup(const std::string& widgetGroupName, const std::string& exceptWidget, Vector2f shift);

	void AddWidgetTransformTask(TWidgetTransformTask widgetTransformTask);

	void OnMouseDown(Vector2f pos, int touchNumber);

	void OnMouseUp(Vector2f pos, int touchNumber);
    
    void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

	void OnMove(Vector2f shift, int touchNumber);

	void ShowKeyboard(const std::string text = "");
	void HideKeyboard();

	bool IsKeyboardOnScreen();


	void PrintWidgetList();

	std::shared_ptr<boost::signals2::signal<void (TSignalParam)>> GetOnClickSignal(const std::string& widgetName);
	std::shared_ptr<boost::signals2::signal<void (TSignalParam)>> GetOnHoldSignal(const std::string& widgetName);
	std::shared_ptr<boost::signals2::signal<void (TSignalParam)>> GetSignal(const std::string& signalName, const std::string& widgetName);
    std::shared_ptr<boost::signals2::signal<void (TSignalParam)>> GetOnTapDownSignal(const std::string& widgetName);

	virtual void BindFunctions();

};

} //namespace SE


#endif
