#include "include/Engine.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"


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
		{
			return true;
		}

		return false;
	}



	//=========================



	WidgetParentInterface::~WidgetParentInterface()
	{

	}

	std::shared_ptr<WidgetAncestor> WidgetParentInterface::findWidgetByName(const std::string& name)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->name == name)
			{
				return children[i];
			}
		}

		for (size_t i = 0; i < children.size(); i++)
		{
			auto result = children[i]->findWidgetByName(name);

			if (result != nullptr)
			{
				return result;
			}
		}

		return nullptr;

	}


	WidgetAncestor::WidgetAncestor(WidgetParentInterface& widgetParent)
		: inited(false)
		, layoutWidth(WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT)
		, layoutHeight(WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT)
		, parent(widgetParent)
		, marginTop(0)
		, marginBottom(0)
		, marginLeft(0)
		, marginRight(0)
		, paddingTop(0)
		, paddingBottom(0)
		, paddingLeft(0)
		, paddingRight(0)
		, focused(false)
		, disabled(false)
	{
		extraTranslation << 0.f, 0.f;

		background = Vector4f(1, 1, 1, 1);
		UpdateRenderPair();
	}

	WidgetAncestor::~WidgetAncestor()
	{

	}

	void WidgetAncestor::setBackground(boost::variant<std::string, Vector4f> background)
	{
		this->background = background;



#ifdef TARGET_WINDOWS_UNIVERSAL
		Visit(background,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture(textureName); });


#else
		Visit(background,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture("ui/" + textureName); });


#endif

		UpdateRenderPair();
	}

	void WidgetAncestor::UpdateRenderPair()
	{
		if (!inited)
		{
			return;
		}

		//Vector2f posFrom(marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop);
		//Vector2f posTo(marginLeft + getDrawWidth(), parent.getContentAreaHeight() - marginTop);
		Vector2f posFrom(0, 0);
		Vector2f posTo(getDrawWidth(), getDrawHeight());

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

	Vector3f WidgetAncestor::getTranslateVector()
	{
		return { marginLeft + extraTranslation(0), parent.getContentAreaHeight() - getDrawHeight() - marginTop + extraTranslation(1), 0.f };
	}

	void WidgetAncestor::Draw()
	{
		if (disabled)
		{
			return;
		}

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(getTranslateVector());

		TRenderParamsSetter render(renderPair.first);

		Renderer->DrawTriangleList(renderPair.second);

		Renderer->PopMatrix();
	}

	float WidgetAncestor::calcWidthForLayoutStyle(LayoutStyle layoutStyle)
	{

		switch (layoutStyle)
		{
		case LS_FIXED:
			return innerWidth() + paddingLeft + paddingRight;
			break;
		case LS_WRAP_CONTENT:
			return innerWidth() + paddingLeft + paddingRight;
			break;
		case LS_MATCH_PARENT:
			return parent.getContentAreaLeftoverWidth() - (marginLeft + marginRight);
			break;
		case LS_RELATIVE_SIZE:
			//Todo: need to fix this
			return parent.getContentAreaLeftoverWidth() - (marginLeft + marginRight);
			break;
		}

		return 0;
	}
	float WidgetAncestor::calcHeightForLayoutStyle(LayoutStyle layoutStyle)
	{

		switch (layoutStyle)
		{
		case LS_FIXED:
			return innerHeight() + (paddingTop + paddingBottom);
			break;
		case LS_WRAP_CONTENT:
			return innerHeight() + (paddingTop + paddingBottom);
			break;
		case LS_MATCH_PARENT:
			return parent.getContentAreaLeftoverHeight() - (marginTop + marginBottom);
			break;
		case LS_RELATIVE_SIZE:
			//Todo: need to fix this
			return parent.getContentAreaLeftoverHeight() - (marginTop + marginBottom);
			break;
		}

		return 0;
	}



	float WidgetAncestor::getContentAreaWidth()
	{
		return getDrawWidth() - (paddingLeft + paddingRight);
	}

	float WidgetAncestor::getContentAreaHeight()
	{

		return getDrawHeight() - (paddingTop + paddingBottom);
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

		return getDrawWidth() + (marginLeft + marginRight);
	}

	float WidgetAncestor::getViewHeight()
	{
		return getDrawHeight() + (marginTop + marginBottom);
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

	void WidgetAncestor::setMargin(float newMarginTop, float newMarginBottom, float newMarginLeft, float newMarginRight)
	{
		marginTop = newMarginTop;
		marginBottom = newMarginBottom;
		marginLeft = newMarginLeft;
		marginRight = newMarginRight;

		UpdateRenderPair();
	}

	void WidgetAncestor::setPadding(float newPaddingTop, float newPaddingBottom, float newPaddingLeft, float newPaddingRight)
	{
		paddingTop = newPaddingTop;
		paddingBottom = newPaddingBottom;
		paddingLeft = newPaddingLeft;
		paddingRight = newPaddingRight;

		UpdateRenderPair();
	}

	void WidgetAncestor::setExtraTranslation(float extraTranslationX, float extraTranslationY)
	{
		extraTranslation << extraTranslationX, extraTranslationY;
	}

	void WidgetAncestor::Update(size_t dt)
	{

	}

	void WidgetAncestor::OnMouseDown(Vector2f pos, int touchNumber)
	{
		focused = true;
		onMouseDownSignal(pos, touchNumber);
	}

	void WidgetAncestor::OnMouseUp(Vector2f pos, int touchNumber)
	{
		onMouseUpSignal(pos, touchNumber);
	}

	void WidgetAncestor::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		onMouseUpAfterMoveSignal(pos, touchNumber);
	}

	bool WidgetAncestor::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		onMoveSignal(pos, shift, touchNumber);
		return false;
		
	}

	void WidgetAncestor::OnMouseCancel(int touchNumber)
	{

	}

	void WidgetAncestor::OnKeyPressed(int key)
	{


	}

	void WidgetAncestor::OnMouseMove(Vector2f pos)
	{
	}

	void WidgetAncestor::OnMouseMoveOutside()
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
		if (disabled)
		{
			return;
		}

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

		int notDisabledChildrenCount = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
	
			notDisabledChildrenCount++;

			if (!layoutStyleIsMatchParent(children[i]->layoutHeight))
			{
				sizeToRemove += children[i]->getViewHeight();

				activeChildrenCount += 1;
			}
		}

		if (notDisabledChildrenCount > 1)
		{
			sizeToRemove += (notDisabledChildrenCount - 1) * itemSpacing;
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
		if (!inited)
		{
			return;
		}

		WidgetAncestor::UpdateRenderPair();

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->UpdateRenderPair();
		}

	}



	void VerticalLinearLayout::Draw()
	{
		if (disabled)
		{
			return;
		}

		WidgetAncestor::Draw();

		//Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop + padding(1), 0);
		Vector3f shift = Vector3f(paddingLeft + marginLeft + extraTranslation(0), parent.getContentAreaHeight() - getDrawHeight() - marginTop + paddingBottom + extraTranslation(0), 0);


		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			
			if (children[i]->disabled)
			{
				continue;
			}
			
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
		if (disabled)
		{
			return;
		}


		Vector2f relativePos = pos + Vector2f(-paddingLeft - extraTranslation(0) - marginLeft, -marginBottom - paddingBottom - extraTranslation(1));

		float diff = getContentAreaHeight();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff += -children[i]->getViewHeight();
			
			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);
			
			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseDown(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			diff += -itemSpacing;
		}

		WidgetAncestor::OnMouseDown(pos, touchNumber);
	}

	void VerticalLinearLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		float diff = getContentAreaHeight();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUp(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			diff += -itemSpacing;
		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);
	}

	void VerticalLinearLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);
		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			diff += -itemSpacing;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);
	}

	void VerticalLinearLayout::OnMouseCancel(int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			children[i]->OnMouseCancel(touchNumber);
		}
	}

	bool VerticalLinearLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		if (disabled)
		{
			return false;
		}


		bool childMoved = false;

		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				childMoved = childMoved | children[i]->OnMove(innerRelativePos, shift, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			diff += -itemSpacing;
		}

		if (!childMoved)
		{
			WidgetAncestor::OnMove(pos, shift, touchNumber);
		}
		return childMoved;
	}

	void VerticalLinearLayout::OnKeyPressed(int key)
	{
		if (disabled)
		{
			return;
		}


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

	void VerticalLinearLayout::OnMouseMove(Vector2f pos)
	{
		if (disabled)
		{
			return;
		}


		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));

		float diff = getContentAreaHeight();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseMove(innerRelativePos);
			}
			else
			{
				children[i]->OnMouseMoveOutside();
			}

			diff += -itemSpacing;
		}

		WidgetAncestor::OnMouseMove(pos);
	}

	void VerticalLinearLayout::OnMouseMoveOutside()
	{
		if (disabled)
		{
			return;
		}


		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnMouseMoveOutside, std::placeholders::_1));
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
		if (!inited)
		{
			return;
		}

		WidgetAncestor::UpdateRenderPair();

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->UpdateRenderPair();
		}

	}


	void HorizontalLinearLayout::Draw()
	{

		if (disabled)
		{
			return;
		}


		WidgetAncestor::Draw();

		//Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - margin(1) + padding(1), 0);
		Vector3f shift = Vector3f(paddingLeft + marginLeft + extraTranslation(0), parent.getContentAreaHeight() - getDrawHeight() - marginTop + paddingBottom + extraTranslation(1), 0);

	
		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

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

		if (disabled)
		{
			return;
		}


		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}


			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;
			
			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);
	
			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseDown(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseDown(pos, touchNumber);

	}

	void HorizontalLinearLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{

		if (disabled)
		{
			return;
		}

		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUp(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);
	}

	void HorizontalLinearLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);
	}

	bool HorizontalLinearLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		if (disabled)
		{
			return false;
		}

		//WidgetAncestor::OnMove(pos, shift, touchNumber);
		bool childMoved = false;

		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				
				childMoved = childMoved | children[i]->OnMove(innerRelativePos, shift, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		if (!childMoved)
		{
			WidgetAncestor::OnMove(pos, shift, touchNumber);
		}
		return childMoved;
	}

	void HorizontalLinearLayout::OnMouseCancel(int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			children[i]->OnMouseCancel(touchNumber);
		}
	}

	void HorizontalLinearLayout::OnKeyPressed(int key)
	{
		if (disabled)
		{
			return;
		}

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnKeyPressed, std::placeholders::_1, key));
	}

	void HorizontalLinearLayout::OnMouseMove(Vector2f pos)
	{
		if (disabled)
		{
			return;
		}


		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{

				children[i]->OnMouseMove(innerRelativePos);
			}
			else
			{
				children[i]->OnMouseMoveOutside();
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseMove(pos);
		
	}

	void HorizontalLinearLayout::OnMouseMoveOutside()
	{
		if (disabled)
		{
			return;
		}

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnMouseMoveOutside, std::placeholders::_1));
	}

	void HorizontalLinearLayout::RemoveFocusRecursively()
	{
		focused = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->focused = false;
		}
	}

	//==========================================

	FrameLayout::FrameLayout(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
	{
	}

	float FrameLayout::innerWidth()
	{
		if (disabled)
		{
			return 0;
		}


		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

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

	float FrameLayout::innerHeight()
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

	float FrameLayout::getContentAreaLeftoverHeight()
	{

		float originalContentAreaHeight = WidgetAncestor::getContentAreaHeight();

		return originalContentAreaHeight;
	}

	void FrameLayout::UpdateRenderPair()
	{
		if (!inited)
		{
			return;
		}

		WidgetAncestor::UpdateRenderPair();

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->UpdateRenderPair();
		}

	}



	void FrameLayout::Draw()
	{

		if (disabled)
		{
			return;
		}


		WidgetAncestor::Draw();

		//Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop + padding(1), 0);
		Vector3f shift = Vector3f(paddingLeft + marginLeft + extraTranslation(0), parent.getContentAreaHeight() - getDrawHeight() - marginTop + paddingBottom + extraTranslation(1), 0);


		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			children[i]->Draw();
		}

		Renderer->PopMatrix();
	}

	void FrameLayout::Update(size_t dt)
	{
		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::Update, std::placeholders::_1, dt));
	}

	void FrameLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{

		if (disabled)
		{
			return;
		}


		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff = getContentAreaHeight() - children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseDown(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

		}

		WidgetAncestor::OnMouseDown(pos, touchNumber);
	}

	void FrameLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}


			diff = getContentAreaHeight() - children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUp(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}


		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);
	}

	void FrameLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{

		if (disabled)
		{
			return;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff = getContentAreaHeight() - children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}


		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);
	}

	bool FrameLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		if (disabled)
		{
			return false;
		}

		bool childMoved = false;

		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff = getContentAreaHeight() - children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				childMoved = childMoved | children[i]->OnMove(innerRelativePos, shift, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

		}

		if (!childMoved)
		{
			WidgetAncestor::OnMove(pos, shift, touchNumber);
		}
		return childMoved;
	}

	void FrameLayout::OnMouseCancel(int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			children[i]->OnMouseCancel(touchNumber);
		}
	}

	void FrameLayout::OnKeyPressed(int key)
	{

		if (disabled)
		{
			return;
		}

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnKeyPressed, std::placeholders::_1, key));

	}

	void FrameLayout::OnMouseMove(Vector2f pos)
	{

		if (disabled)
		{
			return;
		}


		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff = getContentAreaHeight() - children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseMove(innerRelativePos);
			}
			else
			{
				children[i]->OnMouseMoveOutside();
			}

		}

		WidgetAncestor::OnMouseMove(pos);
	}

	void FrameLayout::OnMouseMoveOutside()
	{
		if (disabled)
		{
			return;
		}

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnMouseMoveOutside, std::placeholders::_1));
	}

	void FrameLayout::RemoveFocusRecursively()
	{
		focused = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->focused = false;
		}
	}

	//=======================================

	VerticalScrollLayout::VerticalScrollLayout(WidgetParentInterface& widgetParent)
		: VerticalLinearLayout(widgetParent)
		, scroll(0)
	{
	}
	
	void VerticalScrollLayout::Draw()
	{

		if (disabled)
		{
			return;
		}

		WidgetAncestor::Draw();

		//Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop + padding(1), 0);
		Vector3f shift = Vector3f(paddingLeft + marginLeft + extraTranslation(0), parent.getContentAreaHeight() - getDrawHeight() - marginTop + paddingBottom + scroll + extraTranslation(1), 0);

	
		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{

			if (children[i]->disabled)
			{
				continue;
			}

			children[i]->Draw();

			Renderer->TranslateMatrix(Vector3f(0, -children[i]->getViewHeight() - itemSpacing, 0));

		}

		Renderer->PopMatrix();
	}


	void VerticalScrollLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{

		if (disabled)
		{
			return;
		}

		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - scroll - extraTranslation(1));


		float diff = getContentAreaHeight();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseDown(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			diff += -itemSpacing;
		}

		WidgetAncestor::OnMouseDown(pos, touchNumber);
	}

	void VerticalScrollLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}


		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - scroll - extraTranslation(1));

		float diff = getContentAreaHeight();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUp(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			diff += -itemSpacing;
		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);
	}

	void VerticalScrollLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		//Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop - scroll);
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - scroll - extraTranslation(1));

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			diff += -itemSpacing;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);
	}


	bool VerticalScrollLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		if (disabled)
		{
			return false;
		}

		
		WidgetAncestor::OnMove(pos, shift, touchNumber);

		float viewHeight = getContentAreaHeight();
		float contentHeight = innerHeight();

		if (contentHeight > viewHeight)
		{
			scroll -= shift(1);

			if (scroll < 0)
			{
				scroll = 0;
			}

			if (scroll > contentHeight - viewHeight)
			{
				scroll = contentHeight - viewHeight;
			}
		}

		if (std::abs(shift(0)) < std::abs(shift(1)))
		{
			return true;
		}
		else
		{
			return false;
		}

		
	}

	void VerticalScrollLayout::OnMouseMove(Vector2f pos)
	{
		if (disabled)
		{
			return;
		}


		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft - extraTranslation(0), -marginBottom - paddingBottom - scroll - extraTranslation(1));

		float diff = getContentAreaHeight();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseMove(innerRelativePos);
			}
			else
			{
				children[i]->OnMouseMoveOutside();
			}

			diff += -itemSpacing;
		}

	}

	void VerticalScrollLayout::OnMouseMoveOutside()
	{
		if (disabled)
		{
			return;
		}

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnMouseMoveOutside, std::placeholders::_1));
	}

	//========================================


	HorizontalScrollLayout::HorizontalScrollLayout(WidgetParentInterface& widgetParent)
		: HorizontalLinearLayout(widgetParent)
		, scroll(0)
	{
	}

	void HorizontalScrollLayout::Draw()
	{

		if (disabled)
		{
			return;
		}


		WidgetAncestor::Draw();

		Vector3f shift = Vector3f(paddingLeft + marginLeft - scroll + extraTranslation(0), parent.getContentAreaHeight() - getDrawHeight() - marginTop + paddingBottom + extraTranslation(1), 0);

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			children[i]->Draw();

			Renderer->TranslateMatrix(Vector3f(children[i]->getViewWidth() + itemSpacing, 0, 0));

		}

		Renderer->PopMatrix();

	}


	void HorizontalScrollLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{

		if (disabled)
		{
			return;
		}


		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft + scroll - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));
	
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseDown(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseDown(pos, touchNumber);

	}

	void HorizontalScrollLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}


		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft + scroll - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));


		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUp(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);
	}

	void HorizontalScrollLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft + scroll - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);
	}

	bool HorizontalScrollLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		if (disabled)
		{
			return false;
		}

		WidgetAncestor::OnMove(pos, shift, touchNumber);

		float viewWidth = getContentAreaWidth();
		float contentWidth = innerWidth();

		if (contentWidth > viewWidth)
		{
			scroll += shift(0);

			if (scroll < 0)
			{
				scroll = 0;
			}

			if (scroll > contentWidth - viewWidth)
			{
				scroll = contentWidth - viewWidth;
			}
		}

		/*
		if (std::abs(shift(0)) > std::abs(shift(1)))
		{
			return true;
		}
		else
		{
			return false;
		}*/

		return true;
	}

	void HorizontalScrollLayout::OnMouseMove(Vector2f pos)
	{

		if (disabled)
		{
			return;
		}

		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft + scroll - extraTranslation(0), -marginBottom - paddingBottom - extraTranslation(1));

		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->disabled)
			{
				continue;
			}

			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseMove(innerRelativePos);
			}
			else
			{
				children[i]->OnMouseMoveOutside();
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

	}


	void HorizontalScrollLayout::OnMouseMoveOutside()
	{
		if (disabled)
		{
			return;
		}

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnMouseMoveOutside, std::placeholders::_1));
	}


	//=======================================

	Label::Label(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
	{
		textParams.BasicTextAreaParams.HorizontalPadding = 0;
		textParams.BasicTextAreaParams.VerticalPadding = 0;

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
		if (disabled)
		{
			return;
		}

		WidgetAncestor::Draw();


		//+extraTranslation(0)


		Renderer->PushMatrix();
		Renderer->TranslateMatrix(Vector3f(extraTranslation(0), extraTranslation(1), 0.f));

		
		TRenderParamsSetter render(textRenderPair.first);

		Renderer->DrawTriangleList(textRenderPair.second);

		Renderer->PopMatrix();
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
		if (!inited)
		{
			return;
		}
		//TTriangleList triangleList;

		if (textParams.FontName == "")
		{
			textParams.FontName = ResourceManager->FontManager.GetCurrentFontName();
		}

		ResourceManager->FontManager.PushFont(textParams.FontName);

		Vector2f posFrom(marginLeft + paddingLeft, parent.getContentAreaHeight() - getContentAreaHeight() - marginTop - paddingTop);

		Vector2f posTo(marginLeft + paddingLeft + getContentAreaWidth(), parent.getContentAreaHeight() - marginTop - paddingTop);

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
		, hoverButtonState(BS_NONE)
		, buttonTimer(0.f)
		, hoverButtonTimer(0.f)
	{

	}

	void Button::setPressedDrawable(boost::variant<std::string, Vector4f> pressedDrawable)
	{
		this->pressedDrawable = pressedDrawable;


#ifdef TARGET_WINDOWS_UNIVERSAL
		Visit(pressedDrawable,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture(textureName); });
#else
		Visit(pressedDrawable,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture("ui/" + textureName); });
#endif

		UpdateRenderPair();
	}

	void Button::setHoverDrawable(boost::variant<std::string, Vector4f> hoverDrawable)
	{
		this->hoverDrawable = hoverDrawable;


#ifdef TARGET_WINDOWS_UNIVERSAL
		Visit(hoverDrawable,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture(textureName); });
#else
		Visit(hoverDrawable,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture("ui/" + textureName); });
#endif



		UpdateRenderPair();
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
		if (!inited)
		{
			return;
		}

		Label::UpdateRenderPair();
		UpdatePressedRenderPair();

	}

	void Button::UpdatePressedRenderPair()
	{
		Vector2f posFrom(marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop);

		Vector2f posTo(marginLeft + getDrawWidth(), parent.getContentAreaHeight() - marginTop);


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



		std::string hoverTextureName = Visit(hoverDrawable,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		Vector4f hoverColor = Visit(hoverDrawable,
			[this](Vector4f color) { return  color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 0); });


		hoverRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = hoverTextureName;
		hoverRenderPair.second.Data = MakeDataTriangleList(posFrom, posTo);

		for (auto& colorVec : hoverRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = hoverColor;
		}

		hoverRenderPair.second.RefreshBuffer();
	}

	void Button::Draw()
	{

		if (disabled)
		{
			return;
		}

		WidgetAncestor::Draw();
		

		Renderer->PushMatrix();
		Renderer->TranslateMatrix(Vector3f(extraTranslation(0), extraTranslation(1), 0.f));

		TRenderParamsSetter render2(hoverRenderPair.first);

		Renderer->DrawTriangleList(hoverRenderPair.second);


		TRenderParamsSetter render1(pressedRenderPair.first);

		Renderer->DrawTriangleList(pressedRenderPair.second);


		TRenderParamsSetter render3(textRenderPair.first);

		Renderer->DrawTriangleList(textRenderPair.second);


		Renderer->PopMatrix();

	}

	void Button::Update(size_t dt)
	{

		float oldButtonTimer = buttonTimer;
		float oldHoverButtonTimer = hoverButtonTimer;

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


		if (hoverButtonState == ButtonState::BS_PRESSING)
		{
			hoverButtonTimer += dt / 1000.f;

			if (hoverButtonTimer >= CONST_BUTTON_PRESS_TIME)
			{
				hoverButtonTimer = CONST_BUTTON_PRESS_TIME;
				hoverButtonState = ButtonState::BS_PRESSED;
			}
		}

		if (hoverButtonState == ButtonState::BS_EASING)
		{
			hoverButtonTimer -= dt / 1000.f;

			if (hoverButtonTimer <= 0)
			{
				hoverButtonTimer = 0;
				hoverButtonState = ButtonState::BS_NONE;
			}
		}

		for (auto& color : pressedRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			color(3) = buttonTimer / CONST_BUTTON_PRESS_TIME;
		}

		for (auto& color : hoverRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			color(3) = hoverButtonTimer * (1.f - buttonTimer / CONST_BUTTON_PRESS_TIME) / (CONST_BUTTON_PRESS_TIME);
		}

		if (oldHoverButtonTimer != hoverButtonTimer || oldButtonTimer != buttonTimer)
		{
			hoverRenderPair.second.RefreshBuffer();
		}

		if (oldButtonTimer != buttonTimer)
		{
			pressedRenderPair.second.RefreshBuffer();
		}
		

	}

	void Button::OnMouseDown(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		if (buttonState == ButtonState::BS_NONE || buttonState == ButtonState::BS_EASING)
		{
			buttonState = BS_PRESSING;
		}
			
		WidgetAncestor::OnMouseDown(pos, touchNumber);
	}

	void Button::OnMouseUp(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}


		if (buttonState == ButtonState::BS_PRESSING || buttonState == ButtonState::BS_PRESSED)
		{
			buttonState = BS_EASING;
		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);
	}

	void Button::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		if (buttonState == ButtonState::BS_PRESSING || buttonState == ButtonState::BS_PRESSED)
		{
			buttonState = BS_EASING;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);
	}

	void Button::OnMouseCancel(int touchNumber)
	{
		if (disabled)
		{
			return;
		}

		if (buttonState == ButtonState::BS_PRESSING || buttonState == ButtonState::BS_PRESSED)
		{
			buttonState = BS_EASING;
		}
	}

	void Button::OnMouseMove(Vector2f pos)
	{
		if (disabled)
		{
			return;
		}

		if (hoverButtonState == ButtonState::BS_NONE || hoverButtonState == ButtonState::BS_EASING)
		{
			hoverButtonState = BS_PRESSING;
		}
	}

	void Button::OnMouseMoveOutside()
	{
		if (disabled)
		{
			return;
		}

		if (hoverButtonState == ButtonState::BS_PRESSING || hoverButtonState == ButtonState::BS_PRESSED)
		{
			hoverButtonState = BS_EASING;
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
		if (disabled)
		{
			return;
		}

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

	HorizontalSlider::HorizontalSlider(WidgetParentInterface& widgetParent) :
		WidgetAncestor(widgetParent)
	{ }

	HorizontalSlider::~HorizontalSlider() {}

	void HorizontalSlider::UpdateRenderPair()
	{
		if (!inited)
		{
			return;
		}

		WidgetAncestor::UpdateRenderPair();
		UpdateSkinRenderPairs();
	}

	void HorizontalSlider::UpdateSkinRenderPairs()
	{

		bool isTexture = Visit(trackSkin,
			[this](Vector4f color) { return false; },
			[this](std::string textureName) { return true; });

		Vector4f color = Visit(trackSkin,
			[this](Vector4f color) { return color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 1); });

		std::string textureName = Visit(trackSkin,
			[this](Vector4f color) { return "white.png"; },
			[this](std::string textureName) { return textureName; });

		Vector2f
			from_point(paddingLeft + sidesPadding, paddingBottom + trackPadding),
			to_point(getDrawWidth() - paddingRight - sidesPadding, getDrawHeight() - paddingTop - trackPadding);

		if (from_point(0) > to_point(0)) from_point(0) = to_point(0);
		if (!isTexture)// || to_point(1) - from_point(1) + 1 < MIN_TRACK_HEIGHT)
		{
			from_point(1) = paddingBottom + getContentAreaHeight() / 2.f - MIN_TRACK_HEIGHT / 2.f;
			to_point(1) = paddingBottom + getContentAreaHeight() / 2.f + MIN_TRACK_HEIGHT / 2.f;
		}
				
		trackRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		trackRenderPair.second.Data = MakeDataTriangleList(from_point, to_point);

		for (auto& colorVec : trackRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = color;
		}

		trackRenderPair.second.RefreshBuffer();

		isTexture = Visit(trackSkin,
			[this](Vector4f color) { return false; },
			[this](std::string textureName) { return true; });

		textureName = Visit(buttonSkin,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		color = Visit(buttonSkin,
			[this](Vector4f color) { return  color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 1); });

		from_point = { paddingLeft + sidesPadding, paddingBottom + buttonPadding };
		to_point = { buttonWidth - 1 + paddingLeft + sidesPadding, getDrawHeight() - paddingTop - buttonPadding };

		if (!isTexture)// || to_point(0) - from_point(0) + 1 < MIN_BUTTON_WIDTH)
		{
			from_point(0) = paddingLeft + getContentAreaWidth() / 2.f - MIN_BUTTON_WIDTH / 2.f;
			to_point(0) = paddingLeft + getContentAreaWidth() / 2.f + MIN_BUTTON_WIDTH / 2.f;
		}
		if (!isTexture) //|| to_point(1) - from_point(1) + 1 < MIN_BUTTON_HEIGHT)
		{
			from_point(1) = paddingBottom + getContentAreaHeight() / 2.f - MIN_BUTTON_HEIGHT / 2.f;
			to_point(1) = paddingBottom + getContentAreaHeight() / 2.f + MIN_BUTTON_HEIGHT / 2.f;
		}

		buttonRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		buttonRenderPair.second.Data = MakeDataTriangleList(from_point, to_point);

		for (auto& colorVec : buttonRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = color;
		}

		buttonRenderPair.second.RefreshBuffer();
	}

	void HorizontalSlider::setPosition(int position)
	{
		if (position < minValue) position = minValue;
		if (position > maxValue) position = maxValue;
		if (this->position != position)
		{
			this->position = position;
			onValueChanged((position - minValue) / (float)(maxValue- minValue));
			Draw();
		}
	}

	void HorizontalSlider::setMinValue(int minValue)
	{
		if (minValue > maxValue) minValue = maxValue;
		this->minValue = minValue;
		setPosition(position);
	}

	void HorizontalSlider::setMaxValue(int maxValue)
	{
		if (maxValue < minValue) maxValue = minValue;
		this->maxValue = maxValue;
		setPosition(position);
	}

	void HorizontalSlider::setButtonWidth(float width)
	{
		if (width < MIN_BUTTON_WIDTH) width = MIN_BUTTON_WIDTH;
		buttonWidth = width;
		sidesPadding = width / 2.f;
	}

	void HorizontalSlider::setButtonPadding(float padding)
	{
		if (getContentAreaHeight() - padding * 2 < MIN_BUTTON_HEIGHT) padding = (getContentAreaHeight() - MIN_BUTTON_HEIGHT) / 2.f;
		buttonPadding = padding;
	}

	void HorizontalSlider::setTrackPadding(float padding)
	{
		if (getContentAreaHeight() - padding * 2 < MIN_TRACK_HEIGHT) padding = (getContentAreaHeight() - MIN_TRACK_HEIGHT) / 2.f;
		trackPadding = padding;
	}

	void HorizontalSlider::setButtonSkin(boost::variant<std::string, Vector4f> buttonSkin)
	{
		this->buttonSkin = buttonSkin;
#ifdef TARGET_WINDOWS_UNIVERSAL
		Visit(buttonSkin,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture(textureName); });
#else
		Visit(buttonSkin,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture("ui/" + textureName); });
#endif

		UpdateRenderPair();
	}

	void HorizontalSlider::setTrackSkin(boost::variant<std::string, Vector4f> trackSkin)
	{
		this->trackSkin = trackSkin;
#ifdef TARGET_WINDOWS_UNIVERSAL
		Visit(trackSkin,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture(textureName); });
#else
		Visit(trackSkin,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture("ui/" + textureName); });
#endif

		UpdateRenderPair();
	}

	void HorizontalSlider::Draw()
	{
		if (disabled)
		{
			return;
		}

		WidgetAncestor::Draw();

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(getTranslateVector());
		
		TRenderParamsSetter render1(trackRenderPair.first);
		Renderer->DrawTriangleList(trackRenderPair.second);

		Renderer->TranslateMatrix(Vector3f( (position - minValue) / (float)(maxValue - minValue) * (getContentAreaWidth() - 2 * sidesPadding) - buttonWidth / 2.f, 0, 0));
		TRenderParamsSetter render2(buttonRenderPair.first);
		Renderer->DrawTriangleList(buttonRenderPair.second);
				
		Renderer->PopMatrix();
	}

	bool HorizontalSlider::isPointAboveTrack(Vector2f point)
	{
		return (point(0) >= -sidesPadding && point(0) < getDrawWidth() - marginRight + sidesPadding &&
			point(1) >= 0 && point(1) <  getDrawHeight() - paddingTop);
	}

	int HorizontalSlider::getTrackPositionFromPoint(Vector2f point)
	{
		return (int)(point(0) / (getContentAreaWidth() - 2 * sidesPadding) * (maxValue - minValue)) + minValue;
	}

	void HorizontalSlider::OnMouseDown(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}
		pos -= Vector2f(marginLeft + paddingLeft + sidesPadding, marginBottom + paddingBottom);
		if (!isPointAboveTrack(pos)) return;
		isTouched = true;
		setPosition(getTrackPositionFromPoint(pos));
		WidgetAncestor::OnMouseDown(pos, touchNumber);
	}

	bool HorizontalSlider::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		if (disabled)
		{
			return false;
		}
		if (isTouched)
		{
			pos -= Vector2f(marginLeft + paddingLeft + sidesPadding, marginBottom + paddingBottom);
			setPosition(getTrackPositionFromPoint(pos));
		}
		return WidgetAncestor::OnMove(pos, shift, touchNumber);
	}

	void HorizontalSlider::OnMouseUp(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}
		isTouched = false;
		WidgetAncestor::OnMouseUp(pos, touchNumber);
	}

	void HorizontalSlider::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		if (disabled)
		{
			return;
		}
		isTouched = false;
		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);
	}

	void HorizontalSlider::OnMouseCancel(int touchNumber)
	{
		if (disabled)
		{
			return;
		}
	}

	void HorizontalSlider::OnMouseMoveOutside()
	{
		if (disabled)
		{
			return;
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

	}

	void NewGuiManager::Deinit()
	{

	}

	void NewGuiManager::Update(size_t dt)
	{

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::Update, std::placeholders::_1, dt));
	}

	void NewGuiManager::Draw()
	{

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::Draw, std::placeholders::_1));
	}

	void NewGuiManager::OnMouseDown(Vector2f pos, int touchNumber)
	{

		pos(0) = pos(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		pos(1) = pos(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();
		Vector2f relativePos = pos;

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->RemoveFocusRecursively();

			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);
		
			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseDown(innerRelativePos, touchNumber);
			}


		}


	}

	void NewGuiManager::OnMouseUp(Vector2f pos, int touchNumber)
	{
		pos(0) = pos(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		pos(1) = pos(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();

		Vector2f relativePos = pos;

		for (size_t i = 0; i < children.size(); i++)
		{
			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			//if (pointIsInsideView(innerRelativePos, children[i]))
			//{
				children[i]->OnMouseUp(innerRelativePos, touchNumber);
			//}


		}

	}

	void NewGuiManager::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		pos(0) = pos(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		pos(1) = pos(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();

		Vector2f relativePos = pos;

		for (size_t i = 0; i < children.size(); i++)
		{
			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			//if (pointIsInsideView(innerRelativePos, children[i]))
			//{
				children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber);
			//}


		}

	}

	void NewGuiManager::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		pos(0) = pos(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		pos(1) = pos(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();

		shift(0) = shift(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		shift(1) = shift(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnMove, std::placeholders::_1, pos, shift, touchNumber));
	}

	void NewGuiManager::OnKeyPressed(int key)
	{
		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnKeyPressed, std::placeholders::_1, key));
	}

	void NewGuiManager::OnMouseMove(Vector2f pos)
	{
		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnMouseMove, std::placeholders::_1, pos));
	}

	float NewGuiManager::getContentAreaWidth()
	{
		//return Renderer->GetScreenWidth();
		//return 480;
		return Renderer->GetMatrixWidth();
	}

	float NewGuiManager::getContentAreaHeight()
	{
		//return Renderer->GetScreenHeight();
		//return 320;
		return Renderer->GetMatrixHeight();
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
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->UpdateRenderPair();
		}

	}

	void NewGuiManager::LoadFromConfig(const std::string& configFileName)
	{
		size_t count;
		boost::shared_array<char> data = CreateMemFromFile<char>(ST::PathToResources + configFileName, count);


		std::stringstream ss;
		ss.write(&data[0], count);

		boost::property_tree::ptree ptree;

		boost::property_tree::read_json(ss, ptree);

		AddWidgetsRecursively(*this, children, ptree.get_child("widgets"));

		UpdateAllRenderPair();

	}

	void NewGuiManager::AddWidgetsRecursively(WidgetParentInterface& parentWidget, std::vector<std::shared_ptr<WidgetAncestor>>& widgetArr, boost::property_tree::ptree& ptree)
	{
		for (auto& pWidgetRecord : ptree)
		{
			std::string type = pWidgetRecord.second.get<std::string>("type");

			std::shared_ptr<WidgetAncestor> widget;

			if (type == "VerticalLinearLayout")
			{
				auto verticalLinearLayout = parentWidget.CreateAndAddChildOfType<VerticalLinearLayout>();

				verticalLinearLayout->setItemSpacing(pWidgetRecord.second.get<float>("itemSpacing", 0.f));
				
				auto child = pWidgetRecord.second.get_child_optional("children");

				if (child)
				{
					AddWidgetsRecursively(*verticalLinearLayout, verticalLinearLayout->children, *child);
				}

				widget = verticalLinearLayout;

			}
			if (type == "HorizontalLinearLayout")
			{
				auto horizontalLinearLayout = parentWidget.CreateAndAddChildOfType<HorizontalLinearLayout>();

				horizontalLinearLayout->setItemSpacing(pWidgetRecord.second.get<float>("itemSpacing", 0.f));

				auto child = pWidgetRecord.second.get_child_optional("children");

				if (child)
				{
					AddWidgetsRecursively(*horizontalLinearLayout, horizontalLinearLayout->children, *child);
				}

				widget = horizontalLinearLayout;

			}
			
			if (type == "FrameLayout")
			{
				auto frameLayout = parentWidget.CreateAndAddChildOfType<FrameLayout>();

				auto child = pWidgetRecord.second.get_child_optional("children");

				if (child)
				{
					AddWidgetsRecursively(*frameLayout, frameLayout->children, *child);
				}

				widget = frameLayout;

			}
			if (type == "VerticalScrollLayout")
			{
				auto verticalScrollLayout = parentWidget.CreateAndAddChildOfType<VerticalScrollLayout>();

				verticalScrollLayout->setItemSpacing(pWidgetRecord.second.get<float>("itemSpacing", 0.f));

				auto child = pWidgetRecord.second.get_child_optional("children");

				if (child)
				{
					AddWidgetsRecursively(*verticalScrollLayout, verticalScrollLayout->children, *child);
				}

				widget = verticalScrollLayout;

			}
			if (type == "HorizontalScrollLayout")
			{
				auto horizontalScrollLayout = parentWidget.CreateAndAddChildOfType<HorizontalScrollLayout>();

				horizontalScrollLayout->setItemSpacing(pWidgetRecord.second.get<float>("itemSpacing", 0.f));

				auto child = pWidgetRecord.second.get_child_optional("children");

				if (child)
				{
					AddWidgetsRecursively(*horizontalScrollLayout, horizontalScrollLayout->children, *child);
				}

				widget = horizontalScrollLayout;

			}
			if (type == "Label")
			{
				auto label = parentWidget.CreateAndAddChildOfType<Label>();

				label->setText(pWidgetRecord.second.get<std::string>("text", ""));

				widget = label;
			}
			if (type == "EditText")
			{
				auto editText = parentWidget.CreateAndAddChildOfType<EditText>();

				editText->setText(pWidgetRecord.second.get<std::string>("text", ""));

				widget = editText;
			}
			if (type == "Button")
			{
				auto button = parentWidget.CreateAndAddChildOfType<Button>();

				button->setText(pWidgetRecord.second.get<std::string>("text", ""));
				button->setPressedDrawable(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("pressedDrawable", "#00000000")));
				button->setHoverDrawable(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("hoverDrawable", "#00000000")));

				widget = button;
			}
			if (type == "ImageView")
			{
				auto imageView = parentWidget.CreateAndAddChildOfType<ImageView>();

				widget = imageView;
			}
			if (type == "HorizontalSlider")
			{
				auto slider = parentWidget.CreateAndAddChildOfType<HorizontalSlider>();

				float width = slider->getContentAreaWidth(), height = slider->getContentAreaHeight();

				slider->setMaxValue(pWidgetRecord.second.get<int>("maxValue", 100));
				slider->setMinValue(pWidgetRecord.second.get<int>("minValue", 0));
				slider->setPosition(pWidgetRecord.second.get<int>("position", 50));
				slider->setButtonWidth(pWidgetRecord.second.get<int>("buttonWidth", 40));
				slider->setButtonPadding(pWidgetRecord.second.get<int>("buttonPadding", 10));
				slider->setTrackPadding(pWidgetRecord.second.get<int>("trackPadding", 20));
				slider->setButtonSkin(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("buttonSkin", "#000000FF")));
				slider->setTrackSkin(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("trackSkin", "#C0C0C0FF")));

				widget = slider;
			}
			
			widget->name = pWidgetRecord.second.get<std::string>("name", "");

			widget->setPadding(pWidgetRecord.second.get<float>("paddingTop", 0.f), pWidgetRecord.second.get<float>("paddingBottom", 0.f), pWidgetRecord.second.get<float>("paddingLeft", 0.f), pWidgetRecord.second.get<float>("paddingRight", 0.f));

			widget->setMargin(pWidgetRecord.second.get<float>("marginTop", 0.f), pWidgetRecord.second.get<float>("marginBottom", 0.f), pWidgetRecord.second.get<float>("marginLeft", 0.f), pWidgetRecord.second.get<float>("marginRight", 0.f));

			widget->setLayoutWidth(layoutDimentionFromConfigValue(pWidgetRecord.second.get<std::string>("width", "wrap_content")));

			widget->setLayoutHeight(layoutDimentionFromConfigValue(pWidgetRecord.second.get<std::string>("height", "wrap_content")));

			widget->setBackground(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("background", "#00000000")));

			widget->setExtraTranslation(pWidgetRecord.second.get<float>("extraTranslationX", 0.f), pWidgetRecord.second.get<float>("extraTranslationY", 0.f));
			
			widget->disabled = pWidgetRecord.second.get<int>("disabled", 0) == 0 ? false : true;
			
			widget->inited = true;			

		}


	}

	boost::variant<float, WidgetAncestor::LayoutStyle> NewGuiManager::layoutDimentionFromConfigValue(std::string configValue)
	{
		boost::to_lower(configValue);

		if (configValue == "match_parent")
		{
			return WidgetAncestor::LayoutStyle::LS_MATCH_PARENT;
		}
		else if(configValue == "wrap_content")
		{
			return WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT;
		}
		else
		{
			try
			{
				return boost::lexical_cast<float>(configValue);
			}
			catch (boost::bad_lexical_cast& castError)
			{
				return WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT;
			}
		}

	}


	boost::variant<std::string, Vector4f> NewGuiManager::layoutBackgroundFromConfigValue(std::string configValue)
	{

		if (boost::starts_with(configValue, "#"))
		{
			configValue.erase(configValue.begin(), configValue.begin() + 1);

			unsigned int color;
			std::stringstream ss;
			ss << std::hex << configValue;
			ss >> color;

			Vector4f result;

			result(3) = (color % 256) / 255.f;

			result(2) = ((color >> 8) % 256) / 255.f;

			result(1) = ((color >> 16) % 256) / 255.f;

			result(0) = ((color >> 24) % 256) / 255.f;

			return result;
		}
		else
		{
			return configValue;
		}

	}

	



} //namespace SE
