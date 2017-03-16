#ifndef NEW_GUI_MANAGER_H_INCLUDED
#define NEW_GUI_MANAGER_H_INCLUDED

#include "include/Render/RenderMisc.h"
#include "include/Utils/Utils.h"

#include "boost/variant.hpp"
#include "boost/thread.hpp"

namespace SE
{

	class WidgetParentInterface
	{
	public:
		virtual ~WidgetParentInterface();

		virtual float getContentAreaWidth() = 0;
		virtual float getContentAreaHeight() = 0;

		virtual float getContentAreaLeftoverWidth() = 0;
		virtual float getContentAreaLeftoverHeight() = 0;

	};


	class WidgetAncestor : public WidgetParentInterface
	{
	protected:

		boost::variant<std::string, Vector4f> background;

		TRenderPair renderPair;


	public:

		enum LayoutStyle
		{
			LS_FIXED,
			LS_WRAP_CONTENT,
			LS_MATCH_PARENT,
			LS_RELATIVE_SIZE
		};

		

		boost::variant<float, LayoutStyle> layoutWidth;
		boost::variant<float, LayoutStyle> layoutHeight;

		WidgetParentInterface& parent;

		Vector2f margin;
		Vector2f padding;

		//std::vector<std::shared_ptr<WidgetAncestor>> children;

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

		TRenderPair renderPair;

		std::vector<std::shared_ptr<WidgetAncestor>> children;

		VerticalLinearLayout(WidgetParentInterface& widgetParent);

		virtual float innerWidth();
		virtual float innerHeight();

		virtual float getContentAreaLeftoverHeight();


		virtual void Draw();

	};


	class HorizontalLinearLayout : public WidgetAncestor
	{
	protected:


	public:

		float itemSpacing;

		TRenderPair renderPair;

		std::vector<std::shared_ptr<WidgetAncestor>> children;

		HorizontalLinearLayout(WidgetParentInterface& widgetParent);

		virtual float innerWidth();
		virtual float innerHeight();

		virtual float getContentAreaLeftoverWidth();


		virtual void Draw();

	};

	/*
	class TextParams
	{
	public:
		std::string Text;
		std::string FontName;
		TTextBasicAreaParams BasicTextAreaParams;
		TRenderParams RenderParams;

		TextParams()
		{
		}

		TextParams(const std::string& text)
			: Text(text)
		{
		}

		TextParams(const std::string& text, const std::string& fontName, int height, float horizontalPadding, float verticalPadding, TTextHorizontalAlignment textHorizontalAlignment, TTextVerticalAlignment textVerticalAlignment)
			: Text(text)
			, FontName(fontName)
			, BasicTextAreaParams(height, horizontalPadding, verticalPadding, textHorizontalAlignment, textVerticalAlignment)
		{
		}
	};*/



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

		void UpdateTextRenderPair();
	};



	class NewGuiManager : public WidgetParentInterface
	{
	protected:

		TRenderPair BackgroundRenderPair;


		//std::vector<std::shared_ptr<WidgetAncestor>> widgets;

	public:

		std::vector<std::shared_ptr<WidgetAncestor>> widgets;

		NewGuiManager();

		~NewGuiManager();

		void Init();
		void Deinit();

		void Update(size_t dt);

		void Draw();

		void OnMouseDown(Vector2f pos, int touchNumber);

		void OnMouseUp(Vector2f pos, int touchNumber);

		void OnMouseUpAfterMove(Vector2f pos, int touchNumber);

		void OnMove(Vector2f shift, int touchNumber);

		void OnKeyPressed(int key);

		virtual float getContentAreaWidth();
		virtual float getContentAreaHeight();

		virtual float getContentAreaLeftoverWidth();
		virtual float getContentAreaLeftoverHeight();
	};


} //namespace SE




#endif
  //NEW_GUI_MANAGER_H_INCLUDED
