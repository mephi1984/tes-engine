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

		Visit(background,
			[this](Vector4f color) {},
			[this](std::string textureName) { ResourceManager->TexList.AddTexture("ui/" + textureName); });


		UpdateRenderPair();
	}

	void WidgetAncestor::UpdateRenderPair()
	{
		if (!inited)
		{
			return;
		}

		Vector2f posFrom(marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop);

		Vector2f posTo(marginLeft + getDrawWidth(), parent.getContentAreaHeight() - marginTop);


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

	void WidgetAncestor::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{

	}

	void WidgetAncestor::OnMouseCancel(int touchNumber)
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

		WidgetAncestor::Draw();

		//Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop + padding(1), 0);
		Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop + paddingBottom, 0);

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

		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

		float diff = getContentAreaHeight();

		for (size_t i = 0; i < children.size(); i++)
		{
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
	}

	void VerticalLinearLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

		float diff = getContentAreaHeight();

		for (size_t i = 0; i < children.size(); i++)
		{
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
	}

	void VerticalLinearLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
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
	}

	void VerticalLinearLayout::OnMouseCancel(int touchNumber)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->OnMouseCancel(touchNumber);
		}
	}

	void VerticalLinearLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			diff += -children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMove(innerRelativePos, shift, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			diff += -itemSpacing;
		}
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

		WidgetAncestor::Draw();

		//Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - margin(1) + padding(1), 0);
		Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop + paddingBottom, 0);

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

		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

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
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

	}

	void HorizontalLinearLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

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
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}
	}

	void HorizontalLinearLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

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
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}
	}

	void HorizontalLinearLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

		for (size_t i = 0; i < children.size(); i++)
		{
			float drawHeight = getContentAreaHeight();

			float childViewHeight = children[i]->getViewHeight();

			float localHeightDiff = drawHeight - childViewHeight;

			Vector2f innerRelativePos = relativePos - Vector2f(0, localHeightDiff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMove(innerRelativePos, shift, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}
	}

	void HorizontalLinearLayout::OnMouseCancel(int touchNumber)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->OnMouseCancel(touchNumber);
		}
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

	//==========================================

	FrameLayout::FrameLayout(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
	{
	}

	float FrameLayout::innerWidth()
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

		WidgetAncestor::Draw();

		//Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop + padding(1), 0);
		Vector3f shift = Vector3f(paddingLeft + marginLeft, parent.getContentAreaHeight() - getDrawHeight() - marginTop + paddingBottom, 0);

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
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

		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
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
	}

	void FrameLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
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
	}

	void FrameLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
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
	}

	void FrameLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		Vector2f relativePos = pos + Vector2f(-paddingLeft - marginLeft, -marginTop - paddingTop);

		float diff = getContentAreaHeight();


		for (size_t i = 0; i < children.size(); i++)
		{
			diff = getContentAreaHeight() - children[i]->getViewHeight();

			Vector2f innerRelativePos = relativePos - Vector2f(0, diff);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMove(innerRelativePos, shift, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

		}
	}

	void FrameLayout::OnMouseCancel(int touchNumber)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->OnMouseCancel(touchNumber);
		}
	}

	void FrameLayout::OnKeyPressed(int key)
	{

		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnKeyPressed, std::placeholders::_1, key));

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
		, buttonTimer(0.f)
	{

	}

	void Button::setPressedDrawable(boost::variant<std::string, Vector4f> pressedDrawable)
	{
		this->pressedDrawable = pressedDrawable;


		Visit(pressedDrawable,
			[this](Vector4f color) {  },
			[this](std::string textureName) { ResourceManager->TexList.AddTexture("ui/" + textureName); });
		

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

	void Button::OnMouseCancel(int touchNumber)
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
		Vector2f relativePos = pos;

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


		}
	}

	void NewGuiManager::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos;

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


		}
	}

	void NewGuiManager::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnMove, std::placeholders::_1, pos, shift, touchNumber));
	}

	void NewGuiManager::OnKeyPressed(int key)
	{
		std::for_each(children.begin(), children.end(), std::bind(&WidgetAncestor::OnKeyPressed, std::placeholders::_1, key));
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
				button->setPressedDrawable(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("pressedDrawable", "")));

				widget = button;
			}
			if (type == "ImageView")
			{
				auto imageView = parentWidget.CreateAndAddChildOfType<ImageView>();

				widget = imageView;
			}

			

			widget->setPadding(pWidgetRecord.second.get<float>("paddingTop", 0.f), pWidgetRecord.second.get<float>("paddingBottom", 0.f), pWidgetRecord.second.get<float>("paddingLeft", 0.f), pWidgetRecord.second.get<float>("paddingRight", 0.f));

			widget->setMargin(pWidgetRecord.second.get<float>("marginTop", 0.f), pWidgetRecord.second.get<float>("marginBottom", 0.f), pWidgetRecord.second.get<float>("marginLeft", 0.f), pWidgetRecord.second.get<float>("marginRight", 0.f));

			widget->setLayoutWidth(layoutDimentionFromConfigValue(pWidgetRecord.second.get<std::string>("width", "wrap_content")));

			widget->setLayoutHeight(layoutDimentionFromConfigValue(pWidgetRecord.second.get<std::string>("height", "wrap_content")));

			widget->setBackground(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("background", "")));


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
