#include "include/Engine.h"


namespace SE
{

	WidgetParentInterface::~WidgetParentInterface()
	{

	}

	WidgetAncestor::WidgetAncestor(WidgetParentInterface& widgetParent)
		: layoutStyleWidth(WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT)
		, layoutStyleHeight(WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT)
		, parent(widgetParent)
		, margin(0, 0)
		, padding(0, 0)
	{
	}

	WidgetAncestor::~WidgetAncestor()
	{

	}

	void WidgetAncestor::Draw()
	{

	}


	float WidgetAncestor::getWidth()
	{
		switch (layoutStyleWidth)
		{
		case LS_FIXED:
			return innerWidth();
			break;
		case LS_WRAP_CONTENT:
			return innerWidth();
			break;
		case LS_MATCH_PARENT:
			return parent.getWidthWithPadding();
			break;
		case LS_RELATIVE_SIZE:
			//Todo: need to fix this
			return parent.getWidthWithPadding();
			break;
		}

		return 0;
	}

	float WidgetAncestor::getHeight()
	{
		switch (layoutStyleHeight)
		{
		case LS_FIXED:
			return innerHeight();
			break;
		case LS_WRAP_CONTENT:
			return innerHeight();
			break;
		case LS_MATCH_PARENT:
			return parent.getHeightWithPadding();
			break;
		case LS_RELATIVE_SIZE:
			//Todo: need to fix this
			return parent.getHeightWithPadding();
			break;
		}

		return 0;
	}

	

	float WidgetAncestor::getWidthWithPadding()
	{
		return getWidth() - padding(0) * 2;
	}


	float WidgetAncestor::getHeightWithPadding()
	{
		return getHeight() - padding(1) * 2;
	}


	void WidgetAncestor::setLayoutStyleWidth(LayoutStyle layoutStyleWidth)
	{
		this->layoutStyleWidth = layoutStyleWidth;

		UpdateRenderPair();
	}

	void WidgetAncestor::setLayoutStyleHeight(LayoutStyle layoutStyleHeight)
	{
		this->layoutStyleHeight = layoutStyleHeight;

		UpdateRenderPair();
	}

	//=======================================

	ImageView::ImageView(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
	{
		UpdateRenderPair();
	}

	void ImageView::UpdateRenderPair()
	{

		Vector2f posFrom(margin(0), parent.getHeightWithPadding() - getHeight() + margin(1));

		Vector2f posTo(-margin(0) + getWidth(), parent.getHeightWithPadding() - margin(1));

		renderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "bt_box_yellow.jpg";
		renderPair.second.Data = MakeDataTriangleList(posFrom, posTo);
		renderPair.second.RefreshBuffer();
	}

	float ImageView::innerWidth()
	{
		return 200;
	}

	float ImageView::innerHeight()
	{
		return 200;
	}

	void ImageView::Draw()
	{
		TRenderParamsSetter render(renderPair.first);

		Renderer->DrawTriangleList(renderPair.second);
	}

	//========================================


	VerticalLinearLayout::VerticalLinearLayout(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
		, itemSpacing(0)
	{
		UpdateRenderPair();
	}

	void VerticalLinearLayout::UpdateRenderPair()
	{

		//Vector2f parentSize(parent.getWidthWithPadding(), parent.getHeightWithPadding());
		//Vector2f size(getWidth(), getHeight());


		//renderPair.second.Data = MakeDataTriangleList(parentSize - size + margin, parentSize - margin);
		//renderPair.second.RefreshBuffer();
	}


	float VerticalLinearLayout::innerWidth()
	{
		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->layoutStyleWidth != LS_MATCH_PARENT)
			{
				if (result < children[i]->getWidth())
				{
					result = children[i]->getWidth();
				}
			}
		}

		return result;
	}

	float VerticalLinearLayout::innerHeight()
	{
		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->layoutStyleHeight != LS_MATCH_PARENT)
			{
				if (result < children[i]->getHeight())
				{
					result = children[i]->getHeight();
				}
			}
		}

		return result;
	}


	void VerticalLinearLayout::Draw()
	{
		
		Vector3f shift = Vector3f(padding(0) + margin(0), padding(1) + margin(1), 0);

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			
			
			children[i]->Draw();

			Renderer->TranslateMatrix(Vector3f(0, -children[i]->getHeight() - itemSpacing, 0));

		}

		Renderer->PopMatrix();

		//TRenderParamsSetter render(renderPair.first);

		//Renderer->DrawTriangleList(renderPair.second);
	}


	//======================================
	
	NewGuiManager::NewGuiManager()
	{

	}

	NewGuiManager::~NewGuiManager()
	{

	}

	void NewGuiManager::Init()
	{
		/*
		BackgroundRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "bt_box_yellow.jpg";
		BackgroundRenderPair.second.Data = MakeDataTriangleList(Vector2f(0, 0), Vector2f(Renderer->GetScreenWidth(), Renderer->GetScreenHeight()*0.5f));
		BackgroundRenderPair.second.RefreshBuffer();*/
	}

	void NewGuiManager::Deinit()
	{

	}

	void NewGuiManager::Update(size_t dt)
	{

		
	}

	void NewGuiManager::Draw()
	{
		//Renderer->PushMatrix();

		//Renderer->TranslateMatrix(Vector3f(5,5,0));

		for (auto widgetPtr : widgets)
		{
			widgetPtr->Draw();
		}

		//Renderer->PopMatrix();
	}

	void NewGuiManager::OnMouseDown(Vector2f pos, int touchNumber)
	{

	}

	void NewGuiManager::OnMouseUp(Vector2f pos, int touchNumber)
	{

	}

	void NewGuiManager::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{

	}

	void NewGuiManager::OnMove(Vector2f shift, int touchNumber)
	{

	}

	void NewGuiManager::OnKeyPressed(int key)
	{

	}


	float NewGuiManager::getWidthWithPadding()
	{
		return Renderer->GetScreenWidth();
	}

	float NewGuiManager::getHeightWithPadding()
	{
		return Renderer->GetScreenHeight();
	}


} //namespace SE
