#ifndef NEW_GUI_MANAGER_H_INCLUDED
#define NEW_GUI_MANAGER_H_INCLUDED

#include "include/Render/RenderMisc.h"
#include "include/Utils/Utils.h"

#include "boost/variant.hpp"
#include <thread>

namespace SE
{

	class WidgetAncestor;

	class WidgetParentInterface
	{
	public:
		virtual ~WidgetParentInterface();

		virtual float getContentAreaWidth() = 0;
		virtual float getContentAreaHeight() = 0;

		virtual float getContentAreaLeftoverWidth() = 0;
		virtual float getContentAreaLeftoverHeight() = 0;

		std::vector<std::shared_ptr<WidgetAncestor>> children;

		template <typename T>
		std::shared_ptr<T> CreateAndAddChildOfType()
		{
			std::shared_ptr<T> ptr = std::make_shared<T>(*this);

			children.push_back(ptr);

			return ptr;
		}

		std::shared_ptr<WidgetAncestor> findWidgetByName(const std::string& name);

	};


	
	class WidgetAncestor : public WidgetParentInterface
	{
	public:
		enum LayoutStyle
		{
			LS_FIXED,
			LS_WRAP_CONTENT,
			LS_MATCH_PARENT,
			LS_RELATIVE_SIZE
		};

		enum BorderType
		{
			BT_NONE,
			BT_LINE
		};

		enum childrenHorizontalAlignment
		{
			HA_LEFT,
			HA_CENTER,
			HA_RIGHT
		};


		enum childrenVerticalAlignment
		{
			VA_TOP,
			VA_CENTER,
			VA_BOTTOM
		};

	protected:

		boost::variant<std::string, Vector4f> background;
		
		Vector4f borderColor;
		
		BorderType borderType;

		TRenderPair renderPair, bordersRenderPair;


	public:

		bool inited;

		childrenHorizontalAlignment childrenHA;
		childrenVerticalAlignment childrenVA;
		
		boost::variant<float, LayoutStyle> layoutWidth;
		boost::variant<float, LayoutStyle> layoutHeight;

		boost::signals2::signal<void(Vector2f, Vector2f, int)> onMoveSignal;
		boost::signals2::signal<void(Vector2f, int)> onMouseUpAfterMoveSignal;


		boost::signals2::signal<void(Vector2f, int)> onMouseDownSignal;
		boost::signals2::signal<void(Vector2f, int)> onMouseUpSignal;

		WidgetParentInterface& parent;

		float marginTop;
		float marginBottom;
		float marginLeft;
		float marginRight;

		float paddingTop;
		float paddingBottom;
		float paddingLeft;
		float paddingRight;

		Vector2f extraTranslation;

		std::string name;

		bool focused;

		bool disabled;

		WidgetAncestor(WidgetParentInterface& widgetParent);
		virtual ~WidgetAncestor();

		void setBackground(boost::variant<std::string, Vector4f> background);
		
		std::vector<Vector3f> MakeVertexCoordVecOfBorders(Vector2f posFrom, Vector2f posTo);
		std::vector<Vector4f> MakeColorVecOfBorders(Vector4f color);
		std::vector<Vector2f> MakeTexCoordVecOfBorders();
		TDataTriangleList MakeDataTriangleListOfBorders(Vector2f posFrom, Vector2f posTo, Vector4f color);
			
		virtual void UpdateRenderPair();

		virtual void Draw();

		float calcWidthForLayoutStyle(LayoutStyle layoutStyle);
		float calcHeightForLayoutStyle(LayoutStyle layoutStyle);

		virtual Vector2f getContentStart();
		virtual Vector2f getChildTranslate(std::shared_ptr<WidgetAncestor> child);
		inline Vector2f getDrawTranslate();

		virtual float getContentAreaWidth();
		virtual float getContentAreaHeight();

		virtual float getContentAreaLeftoverWidth();
		virtual float getContentAreaLeftoverHeight();

		virtual float getDrawWidth();
		virtual float getDrawHeight();

		virtual float getViewWidth();
		virtual float getViewHeight();

		virtual float innerWidth();
		virtual float innerHeight();

		virtual void setBorderColor(Vector4f color);
		virtual void setBorderType(BorderType newBorderType);

		virtual void setLayoutWidth(boost::variant<float, LayoutStyle> layoutWidth);
		virtual void setLayoutHeight(boost::variant<float, LayoutStyle> layoutHeight);

		virtual void setMargin(float newMarginTop, float newMarginBottom, float newMarginLeft, float newMarginRight);
		virtual void setPadding(float newPaddingTop, float newPaddingBottom, float newPaddingLeft, float newPaddingRight);

		virtual void setChildrenHorizontalAlignment(childrenHorizontalAlignment childrenHA);
		virtual void setChildrenVerticalAlignment(childrenVerticalAlignment childrenHA);

		virtual void setExtraTranslation(float extraTranslationX, float extraTranslationY);

		virtual void Update(size_t dt);

		virtual void OnMouseDown(Vector2f pos, int touchNumber);

		virtual void OnMouseUp(Vector2f pos, int touchNumber);

		virtual void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual void OnKeyPressed(int key);

		virtual void OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();

		virtual void RemoveFocusRecursively();

		
	};


	class ImageView : public WidgetAncestor
	{
	protected:

	public:
		
		ImageView(WidgetParentInterface& widgetParent);

		virtual void Draw();
	};


	class VerticalLinearLayout : public WidgetAncestor
	{
	protected:

		
	public:

		float itemSpacing;

		VerticalLinearLayout(WidgetParentInterface& widgetParent);

		

		virtual float innerWidth();
		virtual float innerHeight();

		virtual float getContentAreaLeftoverHeight();

		void setItemSpacing(float newItemSpacing);

		virtual void UpdateRenderPair();


		virtual void Draw();

		virtual void Update(size_t dt);

		virtual void OnMouseDown(Vector2f pos, int touchNumber);

		virtual void OnMouseUp(Vector2f pos, int touchNumber);

		virtual void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual void OnKeyPressed(int key);

		virtual void OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();

		virtual void RemoveFocusRecursively();

	};


	class HorizontalLinearLayout : public WidgetAncestor
	{
	protected:


	public:

		float itemSpacing;

		TRenderPair renderPair;

		HorizontalLinearLayout(WidgetParentInterface& widgetParent);

		virtual Vector2f getContentTranslate();
		virtual Vector2f getChildTranslate(std::shared_ptr<WidgetAncestor> child);

		virtual float innerWidth();
		virtual float innerHeight();

		virtual float getContentAreaLeftoverWidth();

		void setItemSpacing(float itemSpacing);

		virtual void UpdateRenderPair();

		virtual void Draw();

		virtual void Update(size_t dt);

		virtual void OnMouseDown(Vector2f pos, int touchNumber);

		virtual void OnMouseUp(Vector2f pos, int touchNumber);

		virtual void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual void OnKeyPressed(int key);

		virtual void OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();

		virtual void RemoveFocusRecursively();

	};


	class FrameLayout : public WidgetAncestor
	{
	protected:


	public:

		FrameLayout(WidgetParentInterface& widgetParent);

		virtual float innerWidth();
		virtual float innerHeight();

		virtual float getContentAreaLeftoverHeight();

		virtual void UpdateRenderPair();

		virtual void Draw();

		virtual void Update(size_t dt);

		virtual void OnMouseDown(Vector2f pos, int touchNumber);

		virtual void OnMouseUp(Vector2f pos, int touchNumber);

		virtual void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual void OnKeyPressed(int key);

		virtual void OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();

		virtual void RemoveFocusRecursively();

	};


	class VerticalScrollLayout : public VerticalLinearLayout
	{
	protected:


	public:

		float scroll;

		VerticalScrollLayout(WidgetParentInterface& widgetParent);

		virtual Vector2f getContentStartPosition();

		void Draw();

		virtual void OnMouseDown(Vector2f pos, int touchNumber);

		virtual void OnMouseUp(Vector2f pos, int touchNumber);

		virtual void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);
		virtual void OnMouseMoveOutside();

		virtual void OnMouseMove(Vector2f pos);
	};

	class HorizontalScrollLayout : public HorizontalLinearLayout
	{
	protected:


	public:

		float scroll;

		HorizontalScrollLayout(WidgetParentInterface& widgetParent);

		virtual Vector2f getContentStartPosition();

		void Draw();

		virtual void OnMouseDown(Vector2f pos, int touchNumber);

		virtual void OnMouseUp(Vector2f pos, int touchNumber);

		virtual void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual void OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();
	};


	class Label : public WidgetAncestor
	{
	protected:

		TRenderPair textRenderPair;
		float textLineHeight;
		std::string wrapped_text;

	public:

		static const int DEFAULT_TEXT_LINE_HEIGHT = 18;

		TTextParams textParams;

		Label(WidgetParentInterface& widgetParent);

		float innerWidth();

		float innerHeight();

		virtual void Draw();

		virtual void setText(const std::string& text);

		virtual void UpdateRenderPair();
		virtual void UpdateTextRenderPair();
	};


	class Button : public Label
	{
	protected:

		boost::variant<std::string, Vector4f> pressedDrawable;
		boost::variant<std::string, Vector4f> hoverDrawable;

		static const float CONST_BUTTON_PRESS_TIME;

		enum ButtonState
		{
			BS_NONE,
			BS_PRESSING,
			BS_PRESSED,
			BS_EASING
		};

		ButtonState buttonState;
		ButtonState hoverButtonState;

		float buttonTimer;
		float hoverButtonTimer;

	public:

		TRenderPair pressedRenderPair;
		TRenderPair hoverRenderPair;

		Button(WidgetParentInterface& widgetParent);

		void setPressedDrawable(boost::variant<std::string, Vector4f> pressedDrawable);
		void setHoverDrawable(boost::variant<std::string, Vector4f> hoverDrawable);

		float innerWidth();

		float innerHeight();

		virtual void UpdateRenderPair();

		void UpdatePressedRenderPair();

		virtual void Draw();

		void Update(size_t dt);

		void OnMouseDown(Vector2f pos, int touchNumber);

		void OnMouseUp(Vector2f pos, int touchNumber);

		void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual void OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();
	};



	class EditText : public Label
	{
	protected:

		TRenderPair cursorRenderPair;
		Vector3f cursorRenderPos;
		size_t editTextTimer;
		size_t symbolLimit;
		bool cursorAppeared;

	public:

		static const size_t CURSOR_WIDTH = 2;
		static const size_t CURSOR_FLASHING_HALFPERIOD_MS = 500;

		EditText(WidgetParentInterface& widgetParent);

		virtual void Draw();
		virtual void UpdateRenderPair();
		virtual void UpdateCursorRenderPair();

		Vector3f getCursorPos();

		void setSymbolLimit(size_t limit);
		virtual void setText(const std::string& text);

		virtual void OnKeyPressed(int key);
		virtual void Update (size_t dt);

	};

	class HorizontalSlider : public WidgetAncestor
	{
	protected:		
		int minValue;
		int maxValue;
		int position;
		float buttonWidth;
		float buttonPadding;
		float trackPadding;
		float sidesPadding;

		TRenderPair buttonRenderPair, trackRenderPair;

		boost::variant<std::string, Vector4f> buttonSkin;
		boost::variant<std::string, Vector4f> trackSkin;

		void UpdateSkinRenderPairs();

	public:

		static const size_t MIN_TRACK_HEIGHT = 4;
		static const size_t MIN_BUTTON_HEIGHT = 10;
		static const size_t MIN_BUTTON_WIDTH = 4;

		boost::signals2::signal<void(float)> onValueChanged;

		HorizontalSlider(WidgetParentInterface& widgetParent);
		virtual ~HorizontalSlider();

		void UpdateRenderPair();
		virtual void Draw();

		void changeValue(float t);
		void setPosition(int position);
		void setMinValue(int minValue);
		void setMaxValue(int maxValue);
		void setButtonPadding(float padding);
		void setTrackPadding(float padding);
		void setButtonWidth(float width);
		void setButtonSkin(boost::variant<std::string, Vector4f> buttonSkin);
		void setTrackSkin(boost::variant<std::string, Vector4f> trackSkin);

		bool isPointAboveTrack(Vector2f point);
		int getTrackPositionFromPoint(Vector2f point);

		void OnMouseDown(Vector2f pos, int touchNumber);
		bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

	};
	
	class HorizontalDoubleSlider : public WidgetAncestor
	{
	protected:
		int minValue;
		int maxValue;
		int position1;
		int position2;
		float buttonWidth;
		float buttonPadding;
		float trackPadding;
		float sidesPadding;
		int movingButton;

		TRenderPair button1RenderPair, button2RenderPair, trackRenderPair;

		boost::variant<std::string, Vector4f> button1Skin;
		boost::variant<std::string, Vector4f> button2Skin;
		boost::variant<std::string, Vector4f> trackSkin;

		void UpdateSkinRenderPairs();

	public:

		static const size_t MIN_TRACK_HEIGHT = 4;
		static const size_t MIN_BUTTON_HEIGHT = 10;
		static const size_t MIN_BUTTON_WIDTH = 4;

		boost::signals2::signal<void(float, float)> onValueChanged;

		HorizontalDoubleSlider(WidgetParentInterface& widgetParent);
		virtual ~HorizontalDoubleSlider();

		void UpdateRenderPair();
		virtual void Draw();

		void changeValue1(float t);
		void changeValue2(float t);
		void setPosition1(int position);
		void setPosition2(int position);
		void setMinValue(int minValue);
		void setMaxValue(int maxValue);
		void setButtonPadding(float padding);
		void setTrackPadding(float padding);
		void setButtonWidth(float width);
		void setButton1Skin(boost::variant<std::string, Vector4f> buttonSkin);
		void setButton2Skin(boost::variant<std::string, Vector4f> buttonSkin);
		void setTrackSkin(boost::variant<std::string, Vector4f> trackSkin);

		inline bool isPointAboveTrack(Vector2f point);
		inline int getTrackPositionFromPoint(Vector2f point);
		inline int getButtonNumberFromPosition(int position);

		void OnMouseDown(Vector2f pos, int touchNumber);
		bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);
		void OnMouseUp(Vector2f pos, int touchNumber);
		void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

	};

	class NewGuiManager : public WidgetParentInterface
	{
	protected:

		bool inited;

		TRenderPair BackgroundRenderPair;

	public:

		NewGuiManager();

		~NewGuiManager();

		void Init();
		void Deinit();

		bool IsInited();

		void Update(size_t dt);

		void Draw();

		void OnMouseDown(Vector2f pos, int touchNumber);

		void OnMouseUp(Vector2f pos, int touchNumber);

		void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		void OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		void OnKeyPressed(int key);

		void OnMouseMove(Vector2f pos);

		virtual float getContentAreaWidth();
		virtual float getContentAreaHeight();

		virtual float getContentAreaLeftoverWidth();
		virtual float getContentAreaLeftoverHeight();

		virtual void UpdateAllRenderPair();

		void LoadFromConfig(const std::string& configFileName);

		void AddWidgetsRecursively(WidgetParentInterface& parentWidget, std::vector<std::shared_ptr<WidgetAncestor>>& widgetArr, boost::property_tree::ptree& ptree);

		boost::variant<float, WidgetAncestor::LayoutStyle> layoutDimentionFromConfigValue(std::string configValue);
		boost::variant<std::string, Vector4f> layoutBackgroundFromConfigValue(std::string configValue);
		Vector4f layoutColorFromConfigValue(std::string configValue);
		WidgetAncestor::childrenHorizontalAlignment layoutHorizontalAlignmentFromConfigValue(std::string configValue);
		WidgetAncestor::childrenVerticalAlignment layoutVerticalAlignmentFromConfigValue(std::string configValue);
		WidgetAncestor::BorderType borderTypeFromConfigValue(std::string configValue);


	};


} //namespace SE




#endif
  //NEW_GUI_MANAGER_H_INCLUDED
