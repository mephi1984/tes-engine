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

		Vector2f posFrom(margin(0), parent.getHeightWithPadding() - getHeight() + margin(1));

		Vector2f posTo(-margin(0) + getWidth(), parent.getHeightWithPadding() - margin(1));

		renderPair.second.Data = MakeDataTriangleList(posFrom, posTo);


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
			return innerWidth();
			break;
		case LS_WRAP_CONTENT:
			return innerWidth();
			break;
		case LS_MATCH_PARENT:
			return parent.getWidthWithPadding() - 2 * margin(0);
			break;
		case LS_RELATIVE_SIZE:
			//Todo: need to fix this
			return parent.getWidthWithPadding() - 2 * margin(0);
			break;
		}

		return 0;
	}
	float WidgetAncestor::calcHeightForLayoutStyle(LayoutStyle layoutStyle)
	{
		switch (layoutStyle)
		{
		case LS_FIXED:
			return innerHeight();
			break;
		case LS_WRAP_CONTENT:
			return innerHeight();
			break;
		case LS_MATCH_PARENT:
			return parent.getHeightWithPadding() - 2 * margin(1);
			break;
		case LS_RELATIVE_SIZE:
			//Todo: need to fix this
			return parent.getHeightWithPadding() - 2 * margin(1);
			break;
		}

		return 0;
	}


	float WidgetAncestor::getWidth()
	{
	
		return Visit(layoutWidth,
			[this](float width) { return width; },
			[this](LayoutStyle layoutStyle) { return this->calcWidthForLayoutStyle(layoutStyle); });

	}

	float WidgetAncestor::getHeight()
	{
		return Visit(layoutHeight,
			[this](float width) { return width; },
			[this](LayoutStyle layoutStyle) { return this->calcHeightForLayoutStyle(layoutStyle); });
	}

	

	float WidgetAncestor::getWidthWithPadding()
	{
		return getWidth() - padding(0) * 2;
	}


	float WidgetAncestor::getHeightWithPadding()
	{
		return getHeight() - padding(1) * 2;
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

		auto layoutStyleIsMatchParent = [](boost::variant<float, LayoutStyle> layoutWidth)
		{
			return Visit(layoutWidth,
				[](float width) { return false; },
				[](LayoutStyle layoutStyle) { return layoutStyle = LS_MATCH_PARENT; });
		};
	
		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!layoutStyleIsMatchParent(children[i]->layoutWidth))
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
		auto layoutStyleIsMatchParent = [](boost::variant<float, LayoutStyle> layoutWidth)
		{
			return Visit(layoutWidth,
				[](float width) { return false; },
				[](LayoutStyle layoutStyle) { return layoutStyle = LS_MATCH_PARENT; });
		};


		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{

			result += children[i]->getHeight();
			/*
			if (!layoutStyleIsMatchParent(children[i]->layoutHeight))
			{
				if (result < children[i]->getHeight())
				{
					result = children[i]->getHeight();
				}
			}*/
		}

		return result;
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
