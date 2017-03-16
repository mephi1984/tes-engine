#include "include/Engine.h"


namespace SE
{


	//==== Simple visitor solution from Habrahabr's user 0xd34df00d =======

	namespace detail
	{
		template<typename... Args>
		struct VisitorBase : Args...
		{
			VisitorBase(Args&&... args)
				: Args{ std::forward<Args>(args) }...
			{
			}
		};

		template<typename R, typename... Args>
		struct Visitor : boost::static_visitor<R>, VisitorBase<Args...>
		{
			using VisitorBase<Args...>::VisitorBase;
		};
	}

	template<typename HeadVar, typename... TailVars, typename... Args>
	auto Visit(const boost::variant<HeadVar, TailVars...>& v, Args&&... args)
	{
		using R_t = decltype (detail::VisitorBase<Args...> { std::forward<Args>(args)... } (std::declval<HeadVar>()));

		return boost::apply_visitor(detail::Visitor<R_t, Args...> { std::forward<Args>(args)... }, v);
	}


	//==============================================



	bool layoutStyleIsMatchParent(boost::variant<float, WidgetAncestor::LayoutStyle> layoutWidth)
	{
		return Visit(layoutWidth,
			[](float width) { return false; },
			[](WidgetAncestor::LayoutStyle layoutStyle) { return layoutStyle == WidgetAncestor::LayoutStyle::LS_MATCH_PARENT; });
	};


	//=========================



	WidgetParentInterface::~WidgetParentInterface()
	{

	}

	WidgetAncestor::WidgetAncestor(WidgetParentInterface& widgetParent)
		: layoutWidth(WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT)
		, layoutHeight(WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT)
		, parent(widgetParent)
		, margin(0, 0)
		, padding(0, 0)
	{
		background = Vector4f(1, 1, 1, 1);
		UpdateRenderPair();
	}

	WidgetAncestor::~WidgetAncestor()
	{

	}

	void WidgetAncestor::setBackground(boost::variant<std::string, Vector4f> background)
	{
		this->background = background;
		UpdateRenderPair();
	}

	void WidgetAncestor::UpdateRenderPair()
	{

		Vector2f posFrom(margin(0), parent.getContentAreaHeight() - getDrawHeight() - margin(1));

		Vector2f posTo(margin(0) + getDrawWidth(), parent.getContentAreaHeight() - margin(1));


		std::string textureName = Visit(background,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		Vector4f color = Visit(background,
			[this](Vector4f color) { return  color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 1); });


		renderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		renderPair.second.Data = MakeDataTriangleList(posFrom, posTo);

		for (auto& colorVec : renderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = color;
		}

		renderPair.second.RefreshBuffer();
	}

	float WidgetAncestor::innerWidth()
	{
		return Visit(background,
			[this](Vector4f color) { return 0.f; },
			[this](std::string textureName) { return ResourceManager->TexList.GetTextureWidth(textureName); });
	}

	float WidgetAncestor::innerHeight()
	{
		return Visit(background,
			[this](Vector4f color) { return 0.f; },
			[this](std::string textureName) { return ResourceManager->TexList.GetTextureHeight(textureName); });
	}

	void WidgetAncestor::Draw()
	{
		TRenderParamsSetter render(renderPair.first);

		Renderer->DrawTriangleList(renderPair.second);
	}

	float WidgetAncestor::calcWidthForLayoutStyle(LayoutStyle layoutStyle)
	{
		switch (layoutStyle)
		{
		case LS_FIXED:
			return innerWidth() + 2.f*padding(0);
			break;
		case LS_WRAP_CONTENT:
			return innerWidth() + 2.f*padding(0);
			break;
		case LS_MATCH_PARENT:
			return parent.getContentAreaLeftoverWidth() - 2 * margin(0);
			break;
		case LS_RELATIVE_SIZE:
			//Todo: need to fix this
			return parent.getContentAreaLeftoverWidth() - 2 * margin(0);
			break;
		}

		return 0;
	}
	float WidgetAncestor::calcHeightForLayoutStyle(LayoutStyle layoutStyle)
	{
		switch (layoutStyle)
		{
		case LS_FIXED:
			return innerHeight() + 2.f*padding(1);
			break;
		case LS_WRAP_CONTENT:
			return innerHeight() + 2.f*padding(1);
			break;
		case LS_MATCH_PARENT:
			return parent.getContentAreaLeftoverHeight() - 2 * margin(1);
			break;
		case LS_RELATIVE_SIZE:
			//Todo: need to fix this
			return parent.getContentAreaLeftoverHeight() - 2 * margin(1);
			break;
		}

		return 0;
	}



	float WidgetAncestor::getContentAreaWidth()
	{
		return getDrawWidth() - padding(0) * 2;
	}

	float WidgetAncestor::getContentAreaHeight()
	{
		return getDrawHeight() - padding(1) * 2;
	}

	float WidgetAncestor::getContentAreaLeftoverWidth()
	{
		return getContentAreaWidth();
	}

	float WidgetAncestor::getContentAreaLeftoverHeight()
	{
		return getContentAreaHeight();
	}

	float WidgetAncestor::getDrawWidth()
	{
		return Visit(layoutWidth,
			[this](float width) { return width; },
			[this](LayoutStyle layoutStyle) { return this->calcWidthForLayoutStyle(layoutStyle); });

	}

	float WidgetAncestor::getDrawHeight()
	{
		return Visit(layoutHeight,
			[this](float width) { return width; },
			[this](LayoutStyle layoutStyle) { return this->calcHeightForLayoutStyle(layoutStyle); });
	}

	float WidgetAncestor::getViewWidth()
	{
		return getDrawWidth() + 2*margin(0);
	}

	float WidgetAncestor::getViewHeight()
	{
		return getDrawHeight() + 2 * margin(1);
	}



	void WidgetAncestor::setLayoutWidth(boost::variant<float, LayoutStyle> layoutWidth)
	{
		this->layoutWidth = layoutWidth;

		UpdateRenderPair();
	}

	void WidgetAncestor::setLayoutHeight(boost::variant<float, LayoutStyle> layoutHeight)
	{
		this->layoutHeight = layoutHeight;

		UpdateRenderPair();
	}

	//=======================================

	ImageView::ImageView(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
	{

	}



	void ImageView::Draw()
	{
		WidgetAncestor::Draw();
	}

	//========================================


	VerticalLinearLayout::VerticalLinearLayout(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
		, itemSpacing(0)
	{
		UpdateRenderPair();
	}

	float VerticalLinearLayout::innerWidth()
	{

		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!layoutStyleIsMatchParent(children[i]->layoutWidth))
			{
				if (result < children[i]->getViewWidth())
				{
					result = children[i]->getViewWidth();
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

			result += children[i]->getViewHeight();
			
			if (i > 0)
			{
				result += itemSpacing;
			}
		}


		return result;
	}

	float VerticalLinearLayout::getContentAreaLeftoverHeight()
	{
		float originalContentAreaHeight = WidgetAncestor::getContentAreaHeight();



		float sizeToRemove = 0;

		int activeChildrenCount = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
		if (!layoutStyleIsMatchParent(children[i]->layoutHeight))
		{
		sizeToRemove += children[i]->getViewHeight();

		activeChildrenCount += 1;
		}
		}

		if (children.size() > 1)
		{
		sizeToRemove += (children.size() - 1) * itemSpacing;
		}

		return originalContentAreaHeight - sizeToRemove;
	}



	void VerticalLinearLayout::Draw()
	{

		WidgetAncestor::Draw();
		
		Vector3f shift = Vector3f(padding(0) + margin(0), padding(1) + margin(1), 0);

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			
			
			children[i]->Draw();

			Renderer->TranslateMatrix(Vector3f(0, -children[i]->getViewHeight() - itemSpacing, 0));

		}

		Renderer->PopMatrix();
	}

	//========================================


	HorizontalLinearLayout::HorizontalLinearLayout(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
		, itemSpacing(0)
	{
		UpdateRenderPair();
	}

	float HorizontalLinearLayout::innerHeight()
	{

		

		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!layoutStyleIsMatchParent(children[i]->layoutHeight))
			{
				if (result < children[i]->getViewHeight())
				{
					result = children[i]->getViewHeight();
				}
			}
		}

		return result;
	}

	float HorizontalLinearLayout::innerWidth()
	{

		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{

			result += children[i]->getViewWidth();

			if (i > 0)
			{
				result += itemSpacing;
			}
		}


		return result;
	}

	float HorizontalLinearLayout::getContentAreaLeftoverWidth()
	{
		float originalContentAreaWidth = WidgetAncestor::getContentAreaWidth();

		float sizeToRemove = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!layoutStyleIsMatchParent(children[i]->layoutWidth))
			{
				sizeToRemove += children[i]->getViewWidth();

			}
		}

		if (children.size() > 1)
		{
			sizeToRemove += (children.size() - 1) * itemSpacing;
		}

		return originalContentAreaWidth - sizeToRemove;
	}



	void HorizontalLinearLayout::Draw()
	{

		WidgetAncestor::Draw();

		Vector3f shift = Vector3f(padding(0) + margin(0), parent.getContentAreaHeight() - getDrawHeight() - margin(1) + padding(1), 0);


		//Vector2f posFrom(margin(0), parent.getContentAreaHeight() - getDrawHeight() - margin(1));

		//Vector2f posTo(margin(0) + getDrawWidth(), parent.getContentAreaHeight() - margin(1));


		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{

			children[i]->Draw();

			Renderer->TranslateMatrix(Vector3f(children[i]->getViewHeight() + itemSpacing, 0, 0));

		}

		Renderer->PopMatrix();

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

	float NewGuiManager::getContentAreaWidth()
	{
		return Renderer->GetScreenWidth();
	}

	float NewGuiManager::getContentAreaHeight()
	{
		return Renderer->GetScreenHeight();
	}

	float NewGuiManager::getContentAreaLeftoverWidth()
	{
		return getContentAreaWidth();
	}

	float NewGuiManager::getContentAreaLeftoverHeight()
	{
		return getContentAreaHeight();
	}


} //namespace SE
