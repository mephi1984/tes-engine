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

		virtual float getWidthWithPadding() = 0;
		virtual float getHeightWithPadding() = 0;

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

		LayoutStyle layoutStyleWidth;
		LayoutStyle layoutStyleHeight;
		

		Vector2f layoutStyleContentSize;

		WidgetParentInterface& parent;

		Vector2f margin;
		Vector2f padding;

		//std::vector<std::shared_ptr<WidgetAncestor>> children;

		WidgetAncestor(WidgetParentInterface& widgetParent);
		virtual ~WidgetAncestor();

		virtual void UpdateRenderPair() = 0;

		virtual void Draw();

		virtual float getWidth();
		virtual float getHeight();

		virtual float getWidthWithPadding();		
		virtual float getHeightWithPadding();

		virtual float innerWidth() = 0;
		virtual float innerHeight() = 0;

		virtual void setLayoutStyleWidth(LayoutStyle layoutStyleWidth);
		virtual void setLayoutStyleHeight(LayoutStyle layoutStyleHeight);

	};

	class ImageView : public WidgetAncestor
	{
	protected:

		

	public:
		TRenderPair renderPair;

		ImageView(WidgetParentInterface& widgetParent);

		virtual void UpdateRenderPair();

		virtual float innerWidth();
		virtual float innerHeight();

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

		virtual void UpdateRenderPair();

		virtual float innerWidth();
		virtual float innerHeight();


		virtual void Draw();

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

		virtual float getWidthWithPadding();
		virtual float getHeightWithPadding();

	};


} //namespace SE




#endif
  //NEW_GUI_MANAGER_H_INCLUDED
