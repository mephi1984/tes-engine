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

	bool pointIsInsideView(Vector2f point, std::shared_ptr<WidgetAncestor> widget)
	{
		float viewWidth = widget->getViewWidth();
		float viewHeight = widget->getViewHeight();
		if ((point(0) >= 0) && (point(1) >= 0) && (point(0) <= viewWidth) && (point(1) <= viewHeight))
		//if ((point(0) >= 0) && (point(0) <= viewWidth))
		{
			return true;
		}

		return false;
	}



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
		, focused(false)
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

	void WidgetAncestor::setMargin(Vector2f newMargin)
	{
		margin = newMargin;
		UpdateRenderPair();
	}

	void WidgetAncestor::setPadding(Vector2f newPadding)
	{
		padding = newPadding;
		UpdateRenderPair();
	}

	void WidgetAncestor::Update(size_t dt)
	{

	}

	void WidgetAncestor::OnMouseDown(Vector2f pos, int touchNumber)
	{
		focused = true;
	}

	void WidgetAncestor::OnMouseUp(Vector2f pos, int touchNumber)
	{

	}

	void WidgetAncestor::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{

	}

	void WidgetAncestor::OnMove(Vector2f shift, int touchNumber)
	{

	}

	void WidgetAncestor::OnKeyPressed(int key)
	{

	}


	void WidgetAncestor::RemoveFocusRecursively()
	{
		focused = false;
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

	void VerticalLinearLayout::setItemSpacing(float newItemSpacing)
	{
		itemSpacing = newItemSpacing;

		UpdateRenderPair();
	}

	void VerticalLinearLayout::UpdateRenderPair()
	{
		WidgetAncestor::UpdateRenderPair();

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->UpdateRenderPair();
		}

	}



	void VerticalLinearLayout::Draw()
	{

		WidgetAncestor::Draw();

		Vector3f shift = Vector3f(padding(0) + margin(0), parent.getContentAreaHeight() - getDrawHeight() - margin(1) + padding(1), 0);
		
		//Vector3f shift = Vector3f(padding(0) + margin(0), padding(1) + margin(1), 0);

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			
			
			children[i]->Draw();

			Renderer->TranslateMatrix(Vector3f(0, -children[i]->getViewHeight() - itemSpacing, 0));

		}

		Renderer->PopMatrix();
	}

	void VerticalLinearLayout::Update(size_t dt)
	{
		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::Update, std::placeholders::_1, dt));
	}

	void VerticalLinearLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{

		Vector2f relativePos = pos + Vector2f(-padding(0) - margin(0), -margin(1) - padding(1));

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			diff += -children[i]->getViewHeight();
			
			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);
			
			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseDown(innerRelativePos, touchNumber);
			}

			diff += -itemSpacing;
		}
	}

	void VerticalLinearLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-padding(0) - margin(0), -margin(1) - padding(1));

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUp(innerRelativePos, touchNumber);
			}

			diff += -itemSpacing;
		}
	}

	void VerticalLinearLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-padding(0) - margin(0), -margin(1) - padding(1));

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber);
			}

			diff += -itemSpacing;
		}
	}

	void VerticalLinearLayout::OnMove(Vector2f shift, int touchNumber)
	{
	
	}

	void VerticalLinearLayout::OnKeyPressed(int key)
	{

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnKeyPressed, std::placeholders::_1, key));

	}

	void VerticalLinearLayout::RemoveFocusRecursively()
	{
		focused = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->focused = false;
		}
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

	void HorizontalLinearLayout::setItemSpacing(float newItemSpacing)
	{
		itemSpacing = newItemSpacing;

		UpdateRenderPair();
	}

	void HorizontalLinearLayout::UpdateRenderPair()
	{
		WidgetAncestor::UpdateRenderPair();

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->UpdateRenderPair();
		}

	}


	void HorizontalLinearLayout::Draw()
	{

		WidgetAncestor::Draw();

		Vector3f shift = Vector3f(padding(0) + margin(0), parent.getContentAreaHeight() - getDrawHeight() - margin(1) + padding(1), 0);

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{

			children[i]->Draw();

			Renderer->TranslateMatrix(Vector3f(children[i]->getViewWidth() + itemSpacing, 0, 0));

		}

		Renderer->PopMatrix();

	}


	void HorizontalLinearLayout::Update(size_t dt)
	{
		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::Update, std::placeholders::_1, dt));
	}

	void HorizontalLinearLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{

		Vector2f relativePos = pos + Vector2f(-padding(0) - margin(0), -margin(1) - padding(1));

		for (size_t i = 0; i < children.size(); i++)
		{
			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;
			
			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);
	
			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseDown(innerRelativePos, touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

	}

	void HorizontalLinearLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-padding(0) - margin(0), -margin(1) - padding(1));

		for (size_t i = 0; i < children.size(); i++)
		{
			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUp(innerRelativePos, touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}
	}

	void HorizontalLinearLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-padding(0) - margin(0), -margin(1) - padding(1));

		for (size_t i = 0; i < children.size(); i++)
		{
			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}
	}

	void HorizontalLinearLayout::OnMove(Vector2f shift, int touchNumber)
	{

	}

	void HorizontalLinearLayout::OnKeyPressed(int key)
	{
		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnKeyPressed, std::placeholders::_1, key));
	}

	void HorizontalLinearLayout::RemoveFocusRecursively()
	{
		focused = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->focused = false;
		}
	}

	//=======================================

	Label::Label(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
	{
		textParams.BasicTextAreaParams.HorizontalPadding = 0;
		textParams.BasicTextAreaParams.VerticalPadding = 0;
		//textParams.BasicTextAreaParams.TextVerticalAlignment = TVA_CENTER;
		textParams.BasicTextAreaParams.Height = 18;

		textParams.BasicTextAreaParams.TextHorizontalAlignment = THA_CENTER;


	}


	float Label::innerWidth()
	{
		//return ResourceManager->FontManager.GetTextAdvance(textParams.Text);
		return ResourceManager->FontManager.GetTextAdvance(textParams.Text) + 2; //To prevent wrong word wrap
	}

	float Label::innerHeight()
	{
		return textParams.BasicTextAreaParams.Height;
	}


	void Label::Draw()
	{
		WidgetAncestor::Draw();

		
		TRenderParamsSetter render(textRenderPair.first);

		Renderer->DrawTriangleList(textRenderPair.second);
	}


	void Label::setText(const std::string& text)
	{
		textParams.Text = text;

		UpdateTextRenderPair();
	}
	
	void Label::UpdateRenderPair()
	{
		WidgetAncestor::UpdateRenderPair();

		UpdateTextRenderPair();
	}

	void Label::UpdateTextRenderPair()
	{
		//TTriangleList triangleList;

		if (textParams.FontName == "")
		{
			textParams.FontName = ResourceManager->FontManager.GetCurrentFontName();
		}

		ResourceManager->FontManager.PushFont(textParams.FontName);

		Vector2f posFrom(margin(0) + padding(0), parent.getContentAreaHeight() - getContentAreaHeight() - margin(1)- padding(1));

		Vector2f posTo(margin(0) + padding(0) + getContentAreaWidth(), parent.getContentAreaHeight() - margin(1) - padding(1));

		textRenderPair.second = ResourceManager->FontManager.DrawTextInBoxToVBO(posFrom, posTo, textParams.BasicTextAreaParams, textParams.Text, true);

		textRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();

		
		textRenderPair.second.RefreshBuffer();

		ResourceManager->FontManager.PopFont();
	}


	//=========================================

	const float Button::CONST_BUTTON_PRESS_TIME = 0.1f;

	Button::Button(WidgetParentInterface& widgetParent)
		: Label(widgetParent)
		, buttonState(BS_NONE)
		, buttonTimer(0.f)
	{

	}

	void Button::setPressedDrawable(boost::variant<std::string, Vector4f> pressedDrawable)
	{
		this->pressedDrawable = pressedDrawable;
		UpdatePressedRenderPair();
	}

	float Button::innerWidth()
	{
		float backgroundWidth = WidgetAncestor::innerWidth();
		float textWidth = Label::innerWidth();

		return backgroundWidth > textWidth ? backgroundWidth : textWidth;
	}

	float Button::innerHeight()
	{
		float backgroundHeight = WidgetAncestor::innerHeight();
		float textHeight = Label::innerHeight();

		return backgroundHeight > textHeight ? backgroundHeight : textHeight;
	}

	void Button::UpdateRenderPair()
	{
		Label::UpdateRenderPair();
		UpdatePressedRenderPair();

	}

	void Button::UpdatePressedRenderPair()
	{
		Vector2f posFrom(margin(0), parent.getContentAreaHeight() - getDrawHeight() - margin(1));

		Vector2f posTo(margin(0) + getDrawWidth(), parent.getContentAreaHeight() - margin(1));


		std::string textureName = Visit(pressedDrawable,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		Vector4f color = Visit(pressedDrawable,
			[this](Vector4f color) { return  color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 0); });


		pressedRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		pressedRenderPair.second.Data = MakeDataTriangleList(posFrom, posTo);

		for (auto& colorVec : pressedRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = color;
		}

		pressedRenderPair.second.RefreshBuffer();
	}

	void Button::Draw()
	{

		WidgetAncestor::Draw();
		
		
		TRenderParamsSetter render1(pressedRenderPair.first);

		Renderer->DrawTriangleList(pressedRenderPair.second);


		TRenderParamsSetter render2(textRenderPair.first);

		Renderer->DrawTriangleList(textRenderPair.second);
	}

	void Button::Update(size_t dt)
	{

		if (buttonState == ButtonState::BS_PRESSING)
		{
			buttonTimer += dt / 1000.f;

			if (buttonTimer >= CONST_BUTTON_PRESS_TIME)
			{
				buttonTimer = CONST_BUTTON_PRESS_TIME;
				buttonState = ButtonState::BS_PRESSED;
			}
		}

		if (buttonState == ButtonState::BS_EASING)
		{
			buttonTimer -= dt / 1000.f;

			if (buttonTimer <= 0)
			{
				buttonTimer = 0;
				buttonState = ButtonState::BS_NONE;
			}
		}

		for (auto& color : pressedRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			color(3) = buttonTimer / CONST_BUTTON_PRESS_TIME;
		}
		
		pressedRenderPair.second.RefreshBuffer();

	}

	void Button::OnMouseDown(Vector2f pos, int touchNumber)
	{
		if (buttonState == ButtonState::BS_NONE || buttonState == ButtonState::BS_EASING)
		{
			buttonState = BS_PRESSING;
		}
			
		
	}

	void Button::OnMouseUp(Vector2f pos, int touchNumber)
	{
		if (buttonState == ButtonState::BS_PRESSING || buttonState == ButtonState::BS_PRESSED)
		{
			buttonState = BS_EASING;
		}
	}

	void Button::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		if (buttonState == ButtonState::BS_PRESSING || buttonState == ButtonState::BS_PRESSED)
		{
			buttonState = BS_EASING;
		}
	}

	//=======================================

	EditText::EditText(WidgetParentInterface& widgetParent)
		: Label(widgetParent)
	{
		textParams.BasicTextAreaParams.TextHorizontalAlignment = THA_LEFT;
	}

	void EditText::OnKeyPressed(int key)
	{
		if (focused)
		{
			if (key == 8) //Backspace
			{
				if (textParams.Text.size() > 0)
				{
					textParams.Text.erase(textParams.Text.end() - 1);
				}
			}
			else if (key >= 32 && key <= 255) //ASCII
			{
				textParams.Text += static_cast<char>(key);
			}

			UpdateRenderPair();
		}
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

		std::for_each(widgets.begin(), widgets.end(), std::bind(&WidgetAncestor::Update, std::placeholders::_1, dt));
	}

	void NewGuiManager::Draw()
	{

		std::for_each(widgets.begin(), widgets.end(), std::bind(&WidgetAncestor::Draw, std::placeholders::_1));
	}

	void NewGuiManager::OnMouseDown(Vector2f pos, int touchNumber)
	{



		Vector2f relativePos = pos;

		for (size_t i = 0; i < widgets.size(); i++)
		{
			widgets[i]->RemoveFocusRecursively();

			float drawHeight = getContentAreaHeight();

			float childViewHeight = widgets[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);
		
			if (pointIsInsideView(innerRelativePos, widgets[i]))
			{
				widgets[i]->OnMouseDown(innerRelativePos, touchNumber);
			}


		}

	}

	void NewGuiManager::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos;

		for (size_t i = 0; i < widgets.size(); i++)
		{
			float drawHeight = getContentAreaHeight();

			float childViewHeight = widgets[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, widgets[i]))
			{
				widgets[i]->OnMouseUp(innerRelativePos, touchNumber);
			}


		}
	}

	void NewGuiManager::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos;

		for (size_t i = 0; i < widgets.size(); i++)
		{
			float drawHeight = getContentAreaHeight();

			float childViewHeight = widgets[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, widgets[i]))
			{
				widgets[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber);
			}


		}
	}

	void NewGuiManager::OnMove(Vector2f shift, int touchNumber)
	{
		std::for_each(widgets.begin(), widgets.end(), std::bind(&WidgetAncestor::OnMove, std::placeholders::_1, shift, touchNumber));
	}

	void NewGuiManager::OnKeyPressed(int key)
	{
		std::for_each(widgets.begin(), widgets.end(), std::bind(&WidgetAncestor::OnKeyPressed, std::placeholders::_1, key));
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

	void NewGuiManager::UpdateAllRenderPair()
	{
		for (size_t i = 0; i < widgets.size(); i++)
		{
			widgets[i]->UpdateRenderPair();
		}

	}



} //namespace SE
