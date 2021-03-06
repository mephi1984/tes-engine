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

		WidgetParentInterface();
		virtual ~WidgetParentInterface();

		virtual void recalculateInnerWidth() = 0;
		virtual void recalculateInnerHeight() = 0;

		virtual void shareLeftoverWidthBetweenChildren() = 0;
		virtual void shareLeftoverHeightBetweenChildren() = 0;
		virtual void InitChildrenZOrder() = 0;

		virtual void UpdateRenderPair() = 0;

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

		float calculatedLayoutWidth;
		float calculatedLayoutHeight;

		float calculatedInnerWidth;
		float calculatedInnerHeight;

		float zLevel;
		float zLevelAbsolute;

		bool inited;

		childrenHorizontalAlignment childrenHA;
		childrenVerticalAlignment childrenVA;
		
		boost::variant<float, LayoutStyle> layoutWidth;
		boost::variant<float, LayoutStyle> layoutHeight;

		boost::signals2::signal<void(Vector2f, Vector2f, int)> onMoveSignal;
		boost::signals2::signal<void(Vector2f, int)> onMouseUpAfterMoveSignal;
		boost::signals2::signal<void(Vector2f, int)> onMouseDownSignal;
		boost::signals2::signal<void(Vector2f, int)> onMouseUpSignal;
		boost::signals2::signal<void(int)> onMouseCancelSignal;
		boost::signals2::signal<void(int)> OnKeyPressedSignal;

		WidgetParentInterface& parent;

		float paddingTop;
		float paddingBottom;
		float paddingLeft;
		float paddingRight;

		float marginTop;
		float marginBottom;
		float marginLeft;
		float marginRight;

		static bool pointIsInsideView(Vector2f point, std::shared_ptr<WidgetAncestor> widget);
		
		Vector2f extraTranslation;

		std::string name;

		bool focused;

		bool visible;

		Vector4f getBackgroundColor();

		WidgetAncestor(WidgetParentInterface& widgetParent);
		virtual ~WidgetAncestor();
		
		std::vector<Vector3f> MakeVertexCoordVecOfBorders(Vector2f posFrom, Vector2f posTo, float zLevel = 0);
		std::vector<Vector4f> MakeColorVecOfBorders(Vector4f color);
		std::vector<Vector2f> MakeTexCoordVecOfBorders();
		TTriangleList MakeTriangleListOfBorders(Vector2f posFrom, Vector2f posTo, Vector4f color, float zLevel = 0);
			
		virtual void UpdateRenderPair();

		virtual void Draw();

		inline virtual float getContentAreaWidth();
		inline virtual float getContentAreaHeight();

		virtual float getContentAreaLeftoverWidth();
		virtual float getContentAreaLeftoverHeight();

		inline virtual float getDrawWidth();
		inline virtual float getDrawHeight();

		virtual Vector2f getContentTranslate();
		virtual Vector2f getChildTranslate(std::shared_ptr<WidgetAncestor> child);
		inline Vector2f getDrawTranslate();

		inline virtual float getViewWidth();
		inline virtual float getViewHeight();

		inline virtual float getInnerWidth();
		inline virtual float getInnerHeight();

		virtual float calcInnerWidth();
		virtual float calcInnerHeight();

		virtual void recalculateInnerWidth();
		virtual void recalculateInnerHeight();

		virtual void shareLeftoverWidthBetweenChildren();
		virtual void shareLeftoverHeightBetweenChildren();
		virtual void InitChildrenZOrder();

		virtual void setZLevel(float zLevel);
		virtual void setBackground(boost::variant<std::string, Vector4f> background);
		virtual void setBorderColor(Vector4f color);
		virtual void setBorderType(BorderType newBorderType);
		virtual void setVisibility(bool visible);
		virtual void setOwnerDrawBackground(const std::string& backgroundName);

		virtual void setLayoutWidth(boost::variant<float, LayoutStyle> layoutWidth);
		virtual void setLayoutHeight(boost::variant<float, LayoutStyle> layoutHeight);

		virtual void setMargin(float newMarginTop, float newMarginBottom, float newMarginLeft, float newMarginRight);
		virtual void setPadding(float newPaddingTop, float newPaddingBottom, float newPaddingLeft, float newPaddingRight);

		virtual void setChildrenHorizontalAlignment(childrenHorizontalAlignment childrenHA);
		virtual void setChildrenVerticalAlignment(childrenVerticalAlignment childrenHA);

		virtual void setExtraTranslation(float extraTranslationX, float extraTranslationY);

		virtual void Update(size_t dt);

		virtual bool OnMouseDown(Vector2f pos, int touchNumber);

		virtual bool OnMouseUp(Vector2f pos, int touchNumber);

		virtual bool OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual void OnKeyPressed(int key);

		virtual bool OnMouseMove(Vector2f pos);

		virtual void OnMouseMoveOutside();

		virtual void RemoveFocusRecursively();

		
	};


	class ImageView : public WidgetAncestor
	{
	protected:
		TRenderPair focusRenderPair;
		bool selected;
	public:
		
		ImageView(WidgetParentInterface& widgetParent);

		//virtual void setBackground(boost::variant<std::string, Vector4f> background);
		virtual void Draw();
		virtual void UpdateRenderPair();
		void setSelected(bool val);
		bool isSelected();
	};

	class ColorPickerHueSat : public ImageView
	{
	protected:
		TRenderPair rectPointRender;
		Vector2i currentPointUV;
		Vector4f rectColor;
	public:

		ColorPickerHueSat(WidgetParentInterface& widgetParent);

		//virtual void setBackground(boost::variant<std::string, Vector4f> background);
		virtual void Draw();
		virtual void UpdateRenderPair();

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);
		virtual bool OnMouseUp(Vector2f pos, int touchNumber);

		Vector2i getCurrentPointUV();
		void setCurrentPointUV(Vector2i point);
	};

	class ColorPickerLum : public ImageView
	{
	protected:
		TRenderPair rectPointRender;
		TRenderPair colorPaletteRender;
		int currentPointT;
		Vector4f rectColor;
		std::vector<Vector4f> paletteColors;
	public:

		ColorPickerLum(WidgetParentInterface& widgetParent);

		//virtual void setBackground(boost::variant<std::string, Vector4f> background);
		virtual void Draw();
		virtual void UpdateRenderPair();

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);
		virtual bool OnMouseUp(Vector2f pos, int touchNumber);

		int getCurrentPointT();
		void setCurrentPointT(int point);
		void clearColorPalette();
		void addColorToPalette(Vector4f color);
	};

	class VerticalLinearLayout : public WidgetAncestor
	{
	protected:

		
	public:

		float itemSpacing;
		bool touchTransparency;

		bool heightIsFilled;

		VerticalLinearLayout(WidgetParentInterface& widgetParent);

		virtual void shareLeftoverHeightBetweenChildren();
		virtual void InitChildrenZOrder();

		virtual float calcInnerWidth();
		virtual float calcInnerHeight();

		virtual Vector2f getContentTranslate();
		virtual Vector2f getChildTranslate(std::shared_ptr<WidgetAncestor> child);

		void setItemSpacing(float newItemSpacing);
		void setTouchTransparency(bool touchTransparency);

		virtual void UpdateRenderPair();

		virtual void Draw();

		virtual void Update(size_t dt);

		virtual bool OnMouseDown(Vector2f pos, int touchNumber);

		virtual bool OnMouseUp(Vector2f pos, int touchNumber);

		virtual bool OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual void OnKeyPressed(int key);

		virtual bool OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();

		virtual void RemoveFocusRecursively();

	};


	class HorizontalLinearLayout : public WidgetAncestor
	{
	protected:


	public:

		bool widthIsFilled;
		bool touchTransparency;

		float itemSpacing;

		TRenderPair renderPair;

		HorizontalLinearLayout(WidgetParentInterface& widgetParent);

		virtual void shareLeftoverWidthBetweenChildren();
		virtual void InitChildrenZOrder();

		virtual Vector2f getContentTranslate();
		virtual Vector2f getChildTranslate(std::shared_ptr<WidgetAncestor> child);

		virtual float calcInnerWidth();
		virtual float calcInnerHeight();

		void setItemSpacing(float itemSpacing);
		void setTouchTransparency(bool touchTransparency);

		virtual void UpdateRenderPair();

		virtual void Draw();

		virtual void Update(size_t dt);

		virtual bool OnMouseDown(Vector2f pos, int touchNumber);

		virtual bool OnMouseUp(Vector2f pos, int touchNumber);

		virtual bool OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual void OnKeyPressed(int key);

		virtual bool OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();

		virtual void RemoveFocusRecursively();

	};


	class FrameLayout : public WidgetAncestor
	{
	protected:

	public:

		bool touchTransparency;

		FrameLayout(WidgetParentInterface& widgetParent);

		void setTouchTransparency(bool touchTransparency);

		virtual float calcInnerWidth();
		virtual float calcInnerHeight();

		virtual void UpdateRenderPair();

		virtual void Draw();

		virtual void Update(size_t dt);

		virtual bool OnMouseDown(Vector2f pos, int touchNumber);

		virtual bool OnMouseUp(Vector2f pos, int touchNumber);

		virtual bool OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual void OnKeyPressed(int key);

		virtual bool OnMouseMove(Vector2f pos);

		virtual void OnMouseMoveOutside();

		virtual void RemoveFocusRecursively();

	};

	class FlingGestureSupport
	{
	private:
		static const size_t ACCELERATION_AWAITING_MS; // time between OnMouseDown & OnMouseUpAfterMove for acceleration of current speed
		static const float DECELERATION_PER_MS;
		static const float OFFSET_THRESHOLD; // stop, if there was a tap down, not scrolling
		static const float ACCELERATION_RATIO_PER_SPEED_UNIT; // allows to accelerate scrolling infinitely ...
		static const float DECELERATION_RATIO_PER_SPEED_UNIT; // but as scrolling is faster as deceleration is greater
		static const float EVENTS_IGNORING_SPEED_THRESHOLD_PER_MS; // to block tap events if scrollable widget is accelerated too much
		static const size_t TRACK_RECORD_SIZE; // to remember several previous offsets to find average for smoothing
		static const size_t TRACK_RECORD_TIME_MS; // to trunc old OnMove offsets
		static const float BOUNCING_BRAKING_PER_TRESPASSING_UNIT; // as more we outside as braking is greater
		static const float BOUNCING_WALL; // you shall not pass

		float flingOffset;
		size_t flingTimer;
		size_t flingTimerOld;
		bool flingAwaiting;
		bool flingEnabled;

		size_t bottom;
		size_t top;
		float bouncingWall;
		bool bouncingHappened;
		bool bouncingEnabled;

		float flingSpeed; // per millisecond
		bool ignoreEvents;
		bool tappedDown;

		bool recordIsCycled;
		std::list<std::pair<float, float>>::iterator recordIndex;
		std::list<std::pair<float, float>> trackRecord;

		size_t acceleratonAwaiting;
		float deceleration;
		float threshold;
		float accelerationRatio;
		float decelerationRatio;
		float ignoringSpeed;
		size_t recordSize;
		size_t recordTime;
		float bouncingBraking;
		float bouncingWall_const;

		inline float calculateSmoothedFlingSpeed();
		inline void BouncingBottom(size_t dt, float currentScrollPosition);
		inline void BouncingTop(size_t dt, float currentScrollPosition);

	protected:
		FlingGestureSupport(bool flingEnabled, bool bouncingEnabled);

	public:

		inline bool isTapEventsBlockedByFlingerGesture();

		inline void setBounds(size_t bottom, size_t top);
		inline void setBouncingEnabled(bool enabled);
		inline void setFlingEnabled(bool enabled);

		inline float getFlingSpeed();
		inline bool getFlingEnabled();
		inline bool getBouncingEnabled();
		inline float getBouncingThreshold();

		void FlingGestureOnTapDown();
		void FlingGestureOnUpdate(size_t dt, float currentScrollPosition);
		void FlingGestureOnMove(float delta);
		void FlingGestureOnTapUp();

		void FlingGestureSerialize(boost::property_tree::ptree& propertyTree);
	};


	class VerticalScrollLayout : public VerticalLinearLayout, public FlingGestureSupport
	{
	protected:


	public:

		float scroll;

		VerticalScrollLayout(WidgetParentInterface& widgetParent);

		void Draw();

		virtual bool OnMouseDown(Vector2f pos, int touchNumber);

		virtual bool OnMouseUp(Vector2f pos, int touchNumber);

		virtual bool OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual void Update(size_t dt);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);
		virtual void OnMouseMoveOutside();

		virtual bool OnMouseMove(Vector2f pos);

		virtual void UpdateRenderPair();
	};

	class HorizontalScrollLayout : public HorizontalLinearLayout, public FlingGestureSupport
	{
	protected:


	public:

		float scroll;

		HorizontalScrollLayout(WidgetParentInterface& widgetParent);

		void Draw();

		virtual bool OnMouseDown(Vector2f pos, int touchNumber);

		virtual bool OnMouseUp(Vector2f pos, int touchNumber);

		virtual bool OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		virtual bool OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();

		virtual void Update(size_t dt);

		virtual void UpdateRenderPair();
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

		virtual float calcInnerWidth();
		virtual float calcInnerHeight();

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

		float pressedMaxAlpha;
		float hoverMaxAlpha;

		float buttonTimer;
		float hoverButtonTimer;

	public:

		TRenderPair pressedRenderPair;
		TRenderPair hoverRenderPair;

		Button(WidgetParentInterface& widgetParent);

		void setPressedDrawable(boost::variant<std::string, Vector4f> pressedDrawable);
		void setHoverDrawable(boost::variant<std::string, Vector4f> hoverDrawable);

		virtual float calcInnerWidth();
		virtual float calcInnerHeight();

		virtual void UpdateRenderPair();

		void UpdatePressedRenderPair();

		virtual void Draw();

		void Update(size_t dt);

		bool OnMouseDown(Vector2f pos, int touchNumber);

		bool OnMouseUp(Vector2f pos, int touchNumber);

		bool OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		virtual void OnMouseCancel(int touchNumber);

		virtual bool OnMouseMove(Vector2f pos);
		virtual void OnMouseMoveOutside();
	};

	class CheckBox : public WidgetAncestor
	{
	protected:

		bool checked;

		std::string checkedSkin;
		std::string uncheckedSkin;

	public:

		const std::string UNCHECKED_DEFAULT_TEXTURE = "checkboxNormal.png";
		const std::string CHECKED_DEFAULT_TEXTURE = "checkboxPressed.png";

		TRenderPair checkedRenderPair;
		TRenderPair uncheckedRenderPair;

		boost::signals2::signal<void()> onChecked;
		boost::signals2::signal<void()> onUnchecked;

		CheckBox(WidgetParentInterface& widgetParent);

		void setCheckedSkin(const std::string &checkedSkin);
		void setUncheckedSkin(const std::string &uncheckedSkin);
		void setCheckState(bool checked);

		void UpdateRenderPair() override;
		void UpdateCheckedRenderPair();

		void Draw() override;

		bool OnMouseUp(Vector2f pos, int touchNumber) override;
		bool OnMouseUpAfterMove(Vector2f pos, int touchNumber) override;
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

        static const size_t CURSOR_WIDTH;
        static const size_t CURSOR_FLASHING_HALFPERIOD_MS;

		EditText(WidgetParentInterface& widgetParent);

		virtual void Draw();
		virtual void UpdateRenderPair();
		virtual void UpdateCursorRenderPair();

		Vector3f getCursorPos();

		virtual void setSymbolLimit(size_t limit);
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
		~HorizontalSlider();

		void UpdateRenderPair();
		void Draw();

		void changeValue(float t);
		void setPosition(int position);
		void setMinValue(int minValue);
		void setMaxValue(int maxValue);
		void setButtonPadding(float padding);
		void setTrackPadding(float padding);
		void setButtonWidth(float width);
		void setButtonSkin(boost::variant<std::string, Vector4f> buttonSkin);
		void setTrackSkin(boost::variant<std::string, Vector4f> trackSkin);

		inline bool isPointAboveTrack(Vector2f point);
		inline int getTrackPositionFromPoint(Vector2f point);

		bool OnMouseDown(Vector2f pos, int touchNumber);
		bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		void signalValueChange();
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
		~HorizontalDoubleSlider();

		void UpdateRenderPair();
		void Draw();

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

		bool OnMouseDown(Vector2f pos, int touchNumber);
		bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);
		bool OnMouseUp(Vector2f pos, int touchNumber);
		bool OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		

	};

	class NewGuiManager : public WidgetParentInterface
	{
	protected:

		bool inited;

		TRenderPair BackgroundRenderPair;

	public:
		
		const static int UI_RENDERING_ZNEAR;
		const static int UI_RENDERING_ZFAR;
		const static int UI_RENDERING_ZSTART; // allows to draw behind all other widgets, if needed (now is 100)
		
		NewGuiManager();
		~NewGuiManager();

		float getHeight();
		float getWidth();

		void Init();
		void Deinit();

		bool IsInited();

		void Update(size_t dt);

		void Draw();

		bool OnMouseDown(Vector2f pos, int touchNumber);

		bool OnMouseUp(Vector2f pos, int touchNumber);

		bool OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		bool OnMove(Vector2f pos, Vector2f shift, int touchNumber);

		void OnKeyPressed(int key);

		bool OnMouseMove(Vector2f pos);

		virtual void recalculateInnerWidth();
		virtual void recalculateInnerHeight();

		virtual void shareLeftoverWidthBetweenChildren();
		virtual void shareLeftoverHeightBetweenChildren();
		virtual void InitChildrenZOrder();

		virtual void UpdateRenderPair();
		virtual void UpdateOnWindowResize();

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
