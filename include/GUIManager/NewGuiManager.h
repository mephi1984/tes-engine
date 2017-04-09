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
	protected:

		boost::variant<std::string, Vector4f> background;

		TRenderPair renderPair;


	public:

		bool inited;

		enum LayoutStyle
		{
			LS_FIXED,
			LS_WRAP_CONTENT,
			LS_MATCH_PARENT,
			LS_RELATIVE_SIZE
		};

		
		boost::variant<float, LayoutStyle> layoutWidth;
		boost::variant<float, LayoutStyle> layoutHeight;

		boost::signals2::signal<void(Vector2f, Vector2f, int)> onMoveSignal;
		boost::signals2::signal<void(Vector2f, int)> onMouseUpAfterMoveSignal;

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

		WidgetAncestor(WidgetParentInterface& widgetParent);
		virtual ~WidgetAncestor();

		void setBackground(boost::variant<std::string, Vector4f> background);

		virtual void UpdateRenderPair();

		virtual void Draw();

		float calcWidthForLayoutStyle(LayoutStyle layoutStyle);
		float calcHeightForLayoutStyle(LayoutStyle layoutStyle);

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

		virtual void setLayoutWidth(boost::variant<float, LayoutStyle> layoutWidth);
		virtual void setLayoutHeight(boost::variant<float, LayoutStyle> layoutHeight);

		virtual void setMargin(float newMarginTop, float newMarginBottom, float newMarginLeft, float newMarginRight);
		virtual void setPadding(float newPaddingTop, float newPaddingBottom, float newPaddingLeft, float newPaddingRight);

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

	public:

		TTextParams textParams;

		Label(WidgetParentInterface& widgetParent);

		float innerWidth();

		float innerHeight();

		virtual void Draw();

		void setText(const std::string& text);

		virtual void UpdateRenderPair();

		void UpdateTextRenderPair();
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

	public:

		EditText(WidgetParentInterface& widgetParent);

		virtual void OnKeyPressed(int key);

	};


	class NewGuiManager : public WidgetParentInterface
	{
	protected:

		TRenderPair BackgroundRenderPair;

	public:

		NewGuiManager();

		~NewGuiManager();

		void Init();
		void Deinit();

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


	};


} //namespace SE




#endif
  //NEW_GUI_MANAGER_H_INCLUDED
