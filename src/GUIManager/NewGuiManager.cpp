#include "include/Engine.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"


namespace SE
{

    
    const size_t EditText::CURSOR_WIDTH = 2;
    const size_t EditText::CURSOR_FLASHING_HALFPERIOD_MS = 500;
    

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

	inline WidgetAncestor::LayoutStyle getLayoutStyle(boost::variant<float, WidgetAncestor::LayoutStyle> layoutStyle)
	{
		return Visit(layoutStyle,
			[](float width) { return WidgetAncestor::LayoutStyle::LS_FIXED; },
			[](WidgetAncestor::LayoutStyle layoutStyle) { return layoutStyle; });
	};

	inline bool pointIsInsideView(Vector2f point, std::shared_ptr<WidgetAncestor> widget)
	{
		point -= widget->extraTranslation;
		float viewWidth = widget->getViewWidth();
		float viewHeight = widget->getViewHeight();
		return (point(0) >= 0) && (point(1) >= 0) && (point(0) <= viewWidth) && (point(1) <= viewHeight);
	}

	//===================================

	WidgetParentInterface::WidgetParentInterface()
	{ }

	WidgetParentInterface::~WidgetParentInterface() { }

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

	//====================================

	WidgetAncestor::WidgetAncestor(WidgetParentInterface& widgetParent)
		: inited(false)
		, layoutWidth(WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT)
		, layoutHeight(WidgetAncestor::LayoutStyle::LS_WRAP_CONTENT)
		, parent(widgetParent)
		, marginTop(0)
		, marginBottom(0)
		, marginLeft(0)
		, marginRight(0)
		, paddingLeft(0)
		, paddingRight(0)
		, paddingBottom(0)
		, paddingTop(0)
		, calculatedLayoutWidth(0)
		, calculatedLayoutHeight(0)
		, calculatedInnerWidth(0)
		, calculatedInnerHeight(0)
		, focused(false)
		, visible(true)
		, childrenHA(HA_LEFT)
		, childrenVA(VA_TOP)
	{
		extraTranslation << 0.f, 0.f;
		background = Vector4f(1, 1, 1, 1);

		UpdateRenderPair();
	}

	WidgetAncestor::~WidgetAncestor()
	{

	}

	void WidgetAncestor::setVisibility(bool visible)
	{
		this->visible = visible; // temporary
		UpdateRenderPair();
		parent.recalculateInnerWidth();
		parent.recalculateInnerHeight();
	}

	void WidgetAncestor::setBorderColor(Vector4f color)
	{
		borderColor = color;

		UpdateRenderPair();
	}

	void WidgetAncestor::setBorderType(BorderType newBorderType)
	{
		borderType = newBorderType;

		UpdateRenderPair();
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

	std::vector<Vector3f> WidgetAncestor::MakeVertexCoordVecOfBorders(Vector2f posFrom, Vector2f posTo)
	{
		std::vector<Vector3f> result(8);

		Vector2f pos1 = posFrom;
		Vector2f pos2 = Vector2f(posFrom(0), posTo(1));
		Vector2f pos3 = posTo;
		Vector2f pos4 = Vector2f(posTo(0), posFrom(1));

		result[0] << pos1, 0;
		result[1] << pos2, 0;
		result[2] << pos2, 0;
		result[3] << pos3, 0;
		result[4] << pos3, 0;
		result[5] << pos4, 0;
		result[6] << pos4, 0;
		result[7] << pos1, 0;

		return result;
	}

	std::vector<Vector4f> WidgetAncestor::MakeColorVecOfBorders(Vector4f color)
	{
		return std::vector<Vector4f> (8, color);
	}

	std::vector<Vector2f> WidgetAncestor::MakeTexCoordVecOfBorders()
	{
		return std::vector<Vector2f>(8, Vector2f(0,0));
	}

	TDataTriangleList WidgetAncestor::MakeDataTriangleListOfBorders(Vector2f posFrom, Vector2f posTo, Vector4f color)
	{
		TDataTriangleList triangleList;

		triangleList.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB] = MakeColorVecOfBorders(color);
		triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB] = MakeVertexCoordVecOfBorders(posFrom, posTo);
		triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB] = MakeTexCoordVecOfBorders();

		return triangleList;
	}

	void WidgetAncestor::UpdateRenderPair()
	{
		if (!inited)
		{
			return;
		}

		Vector2f shift = getDrawTranslate();
		Vector2f posFrom = shift;
		Vector2f posTo = shift + Vector2f(getDrawWidth(), getDrawHeight());

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

		bordersRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "white.bmp";
		bordersRenderPair.second.Data = MakeDataTriangleListOfBorders(posFrom, posTo, borderColor);
		
		bordersRenderPair.second.RefreshBuffer();
	}

	void WidgetAncestor::Draw()
	{
		TRenderParamsSetter render1(renderPair.first);
		Renderer->DrawTriangleList(renderPair.second);

		CheckGlError();
		
		if (borderType == BorderType::BT_LINE)
		{
			TRenderParamsSetter render2(bordersRenderPair.first);
			Renderer->DrawTriangleList(bordersRenderPair.second, GL_LINES);

			CheckGlError();
		}
	}
	
	float WidgetAncestor::getContentAreaLeftoverWidth()
	{
		float occupiedWidth = 0.f;
		LayoutStyle style;
		
		for (auto &child : children)
		{
			style = getLayoutStyle(child->layoutWidth);
			if (style != LS_MATCH_PARENT)
			{
				occupiedWidth += child->calculatedLayoutWidth;
			}
		}

		return getContentAreaWidth() - occupiedWidth;
	}

	float WidgetAncestor::getContentAreaLeftoverHeight()
	{
		float occupiedHeight = 0.f;
		LayoutStyle style;

		for (auto &child : children)
		{
			style = getLayoutStyle(child->layoutHeight);
			if (style != LS_MATCH_PARENT)
			{
				occupiedHeight += child->calculatedLayoutHeight;
			}
		}

		return getContentAreaHeight() - occupiedHeight;
	}
	
	void WidgetAncestor::shareLeftoverWidthBetweenChildren()
	{
		if (children.size() == 0) return;

		for (auto &child : children)
		{
			if (getLayoutStyle(child->layoutWidth) == WidgetAncestor::LayoutStyle::LS_MATCH_PARENT)
			{
				child->calculatedLayoutWidth = getContentAreaWidth();
				child->calculatedInnerWidth = child->calcInnerWidth();
			}
			child->shareLeftoverWidthBetweenChildren();
		}
	}

	void WidgetAncestor::shareLeftoverHeightBetweenChildren()
	{
		if (children.size() == 0) return;

		for (auto &child : children)
		{
			if (getLayoutStyle(child->layoutHeight) == WidgetAncestor::LayoutStyle::LS_MATCH_PARENT)
			{
				child->calculatedLayoutHeight = getContentAreaHeight();
				child->calculatedInnerHeight = child->calcInnerHeight();
			}
			child->shareLeftoverHeightBetweenChildren();
		}
	}

	void WidgetAncestor::recalculateInnerWidth()
	{
		calculatedInnerWidth = calcInnerWidth();

		LayoutStyle style = getLayoutStyle(layoutWidth);

		calculatedLayoutWidth = Visit(layoutWidth,
			[this](float width) { return width; },
			[this, style](LayoutStyle layoutStyle)
		{
			return calculatedInnerWidth - marginLeft - marginRight;
		});

		parent.recalculateInnerWidth();
	}

	void WidgetAncestor::recalculateInnerHeight()
	{
		calculatedInnerHeight = calcInnerHeight();

		LayoutStyle style = getLayoutStyle(layoutHeight);

		calculatedLayoutHeight = Visit(layoutHeight,
			[this](float height) { return height; },
			[this, style](LayoutStyle layoutStyle)
		{
			return calculatedInnerHeight - marginBottom - marginTop;
		});

		parent.recalculateInnerHeight();
	}

	float WidgetAncestor::calcInnerWidth()
	{
		return Visit(background,
			[this](Vector4f color) { return 0.f; },
			[this](std::string textureName) { return ResourceManager->TexList.GetTextureWidth(textureName); }) +
			paddingLeft + paddingRight + marginLeft + marginRight;
	}

	float WidgetAncestor::calcInnerHeight()
	{
		return Visit(background,
			[this](Vector4f color) { return 0.f; },
			[this](std::string textureName) { return ResourceManager->TexList.GetTextureHeight(textureName); }) +
			paddingBottom + paddingTop + marginTop + marginBottom;
	}

	float WidgetAncestor::getInnerWidth()
	{
		return calculatedInnerWidth;
	}

	float WidgetAncestor::getInnerHeight()
	{
		return calculatedInnerHeight;
	}

	float WidgetAncestor::getContentAreaWidth()
	{
		return getDrawWidth() - (paddingLeft + paddingRight);
	}

	float WidgetAncestor::getContentAreaHeight()
	{
		return getDrawHeight() - (paddingTop + paddingBottom);
	}

	Vector2f WidgetAncestor::getDrawTranslate()
	{
		return Vector2f(extraTranslation(0) + marginLeft, extraTranslation(1) + marginBottom);
	}

	Vector2f WidgetAncestor::getContentTranslate()
	{
		Vector2f result;
		if (childrenHA == HA_LEFT)
		{
			result(0) = paddingLeft;
		}
		else if (childrenHA == HA_CENTER)
		{
			result(0) = paddingLeft + getContentAreaWidth() / 2.f;
		}
		else
		{
			result(0) = getDrawWidth() - paddingRight;
		}
		
		if (childrenVA == VA_TOP)
		{
			result(1) = getDrawHeight() - paddingTop;
		}
		else if (childrenHA == VA_CENTER)
		{
			result(1) = getContentAreaHeight() / 2.f + paddingBottom;
		}
		else
		{
			result(1) = paddingBottom;
		}

		return result;
	}

	Vector2f WidgetAncestor::getChildTranslate(std::shared_ptr<WidgetAncestor> child)
	{
		float width = getContentAreaWidth();
		float height = getContentAreaHeight();

		Vector2f result;

		if (childrenHA == HA_LEFT)
		{
			result(0) = 0;
		}
		else if (childrenHA == HA_CENTER)
		{
			result(0) = child->getViewWidth() <= width ? -child->getViewWidth() / 2.f : -width / 2.f;
		}
		else
		{
			result(0) = -child->getViewWidth();
			if (result(0) < -width) result(0) = -width;
		}

		if (childrenVA == VA_TOP)
		{
			result(1) = -child->getViewHeight();
		}
		else if (childrenVA == VA_CENTER)
		{
			result(1) = child->getViewHeight() <= height ? result(1) = -child->getViewHeight() / 2.f
				: height / 2.f - child->getViewHeight();
		}
		else
		{
			result(1) = child->getViewHeight() <= height ? 0 : height - child->getViewHeight();
		}

		return result;
	}

	float WidgetAncestor::getDrawWidth()
	{
		return calculatedLayoutWidth;
	}

	float WidgetAncestor::getDrawHeight()
	{
		return calculatedLayoutHeight;
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
		LayoutStyle style = getLayoutStyle(layoutWidth);

		calculatedInnerWidth = calcInnerWidth();

		calculatedLayoutWidth = Visit(layoutWidth,
			[this](float width) { return width; },
			[this, style](LayoutStyle layoutStyle)
		{
			return calculatedInnerWidth - marginLeft - marginRight;
			/*if (style == LS_WRAP_CONTENT)
			{
				return calculatedInnerWidth - marginLeft - marginRight;
			}
			if (style == LS_MATCH_PARENT)
			{
				return 0.f;
			}*/
		});

		UpdateRenderPair();
	}

	void WidgetAncestor::setLayoutHeight(boost::variant<float, LayoutStyle> layoutHeight)
	{
		this->layoutHeight = layoutHeight;
		LayoutStyle style = getLayoutStyle(layoutHeight);

		calculatedInnerHeight = calcInnerHeight();

		calculatedLayoutHeight = Visit(layoutHeight,
			[this](float height) { return height; },
			[this, style](LayoutStyle layoutStyle)
		{
			return calculatedInnerHeight - marginBottom - marginTop;
			/*if (style == LS_WRAP_CONTENT)
			{
				return calculatedInnerHeight - marginBottom - marginTop;
			}
			if (style == LS_MATCH_PARENT)
			{
				return 0.f;
			}*/
		});

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

	void WidgetAncestor::setMargin(float newMarginTop, float newMarginBottom, float newMarginLeft, float newMarginRight)
	{
		marginTop = newMarginTop;
		marginBottom = newMarginBottom;
		marginLeft = newMarginLeft;
		marginRight = newMarginRight;
	}

	void WidgetAncestor::setChildrenHorizontalAlignment(childrenHorizontalAlignment childrenHA)
	{
		this->childrenHA = childrenHA;
		UpdateRenderPair();
	}

	void WidgetAncestor::setChildrenVerticalAlignment(childrenVerticalAlignment childrenVA)
	{
		this->childrenVA = childrenVA;
		UpdateRenderPair();
	}

	void WidgetAncestor::setExtraTranslation(float extraTranslationX, float extraTranslationY)
	{
		extraTranslation << extraTranslationX, extraTranslationY;
	}

	void WidgetAncestor::Update(size_t dt)
	{

	}

	bool WidgetAncestor::OnMouseDown(Vector2f pos, int touchNumber)
	{
		focused = true;
		onMouseDownSignal(pos, touchNumber);
		return true;
	}

	bool WidgetAncestor::OnMouseUp(Vector2f pos, int touchNumber)
	{
		onMouseUpSignal(pos, touchNumber);
		return true;
	}

	bool WidgetAncestor::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		onMouseUpAfterMoveSignal(pos, touchNumber);
		return true;
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
		WidgetAncestor::Draw();
	}

	//========================================


	VerticalLinearLayout::VerticalLinearLayout(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
		, itemSpacing(0)
	{
		UpdateRenderPair();
	}

	void VerticalLinearLayout::shareLeftoverHeightBetweenChildren()
	{
		if (children.size() == 0) return;

		int matchCount = 0;
		std::list<std::shared_ptr<WidgetAncestor>> matchChildren;

		for (auto &child : children)
		{
			if (getLayoutStyle(child->layoutHeight) == WidgetAncestor::LayoutStyle::LS_MATCH_PARENT)
			{
				++matchCount;
				matchChildren.push_back(child);
			}
		}

		if (matchCount > 0)
		{
			heightIsFilled = true;
			float childHeight = getContentAreaLeftoverHeight() / matchCount;
			for (auto &child : matchChildren)
			{
				child->calculatedLayoutHeight = childHeight;
				child->calculatedInnerHeight = child->calcInnerHeight();
			}
		}
		else
		{
			heightIsFilled = false;
		}

		for (auto &child : children)
		{
			child->shareLeftoverHeightBetweenChildren();
		}
	}

	float VerticalLinearLayout::calcInnerWidth()
	{
		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			if (getLayoutStyle(children[i]->layoutWidth) != WidgetAncestor::LayoutStyle::LS_MATCH_PARENT)
			{
				if (result < children[i]->getViewWidth())
				{
					result = children[i]->getViewWidth();
				}
			}
		}

		return result + paddingLeft + paddingRight + marginLeft + marginRight;
	}

	float VerticalLinearLayout::calcInnerHeight()
	{

		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			result += children[i]->getViewHeight();
			
			if (i > 0)
			{
				result += itemSpacing;
			}
		}


		return result + paddingBottom + paddingTop + marginBottom + marginTop;
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

	Vector2f VerticalLinearLayout::getContentTranslate()
	{
		Vector2f result;

		if (childrenHA == HA_LEFT)
		{
			result(0) = paddingLeft;
		}
		else if (childrenHA == HA_CENTER)
		{
			result(0) = paddingLeft + getContentAreaWidth() / 2.f;
		}
		else
		{
			result(0) = getDrawWidth() - paddingRight;
		}

		float diff = getContentAreaHeight() - getInnerHeight();

		if (childrenVA == VA_TOP || heightIsFilled)
		{
			result(1) = getDrawHeight() - paddingTop;
		}
		else if (childrenVA == VA_CENTER)
		{
			result(1) = (diff > 0 ? (getContentAreaHeight() + getInnerHeight()) / 2.f : getContentAreaHeight()) + paddingBottom;
		}
		else
		{
			result(1) = (diff > 0 ? getInnerHeight() : getContentAreaHeight()) + paddingBottom;
		}

		return result;
	}

	Vector2f VerticalLinearLayout::getChildTranslate(std::shared_ptr<WidgetAncestor> child)
	{
		float width = getContentAreaWidth();

		Vector2f result;
		result(1) = 0;

		if (childrenHA == HA_LEFT)
		{
			result(0) = 0;
		}
		else if (childrenHA == HA_CENTER)
		{
			result(0) = child->getViewWidth() <= width ? -child->getViewWidth() / 2.f : -width / 2.f;
		}
		else
		{
			result(0) =  child->getViewWidth() <= width ? -child->getViewWidth() : result(0) = -width;
		}

		return result;
	}

	void VerticalLinearLayout::Draw()
	{
		WidgetAncestor::Draw();

		Vector3f shift, childShift;
		shift << getContentTranslate() + getDrawTranslate(), 0;
		shift(1) += itemSpacing;

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Renderer->TranslateMatrix(Vector3f(0, -children[i]->getViewHeight() - itemSpacing, 0));
			
			childShift << getChildTranslate(children[i]), 0;

			Renderer->PushMatrix();
			Renderer->TranslateMatrix(childShift);

			children[i]->Draw();

			Renderer->PopMatrix();

		}

		Renderer->PopMatrix();

		CheckGlError();
	}

	void VerticalLinearLayout::Update(size_t dt)
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->Update(dt);
		}
	}

	bool VerticalLinearLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;
			
			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);
			
			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseDown(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}
		}

		WidgetAncestor::OnMouseDown(pos, touchNumber);

		return handled;
	}

	bool VerticalLinearLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUp(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}
		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);

		return handled;
	}

	bool VerticalLinearLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		return handled;
	}

	void VerticalLinearLayout::OnMouseCancel(int touchNumber)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			children[i]->OnMouseCancel(touchNumber);
		}
	}

	bool VerticalLinearLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		bool childMoved = false;

		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

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

	void VerticalLinearLayout::OnKeyPressed(int key)
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnKeyPressed(key);
		}
	}

	void VerticalLinearLayout::RemoveFocusRecursively()
	{
		focused = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->RemoveFocusRecursively();;
		}
	}

	void VerticalLinearLayout::OnMouseMove(Vector2f pos)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

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

	void VerticalLinearLayout::OnMouseMoveOutside()
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnMouseMoveOutside();
		}
	}

	//========================================


	HorizontalLinearLayout::HorizontalLinearLayout(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
		, itemSpacing(0)
	{
		UpdateRenderPair();
	}

	void HorizontalLinearLayout::shareLeftoverWidthBetweenChildren()
	{
		if (children.size() == 0) return;

		int matchCount = 0;
		std::list<std::shared_ptr<WidgetAncestor>> matchChildren;

		for (auto &child : children)
		{
			if (getLayoutStyle(child->layoutWidth) == WidgetAncestor::LayoutStyle::LS_MATCH_PARENT)
			{
				++matchCount;
				matchChildren.push_back(child);
			}
		}

		if (matchCount > 0)
		{
			widthIsFilled = true;
			float childWidth = getContentAreaLeftoverWidth() / matchCount;
			for (auto &child : matchChildren)
			{
				child->calculatedLayoutWidth = childWidth;
				child->calculatedInnerWidth = child->calcInnerWidth();
			}
		}
		else
		{
			widthIsFilled = false;
		}

		for (auto &child : children)
		{
			child->shareLeftoverWidthBetweenChildren();
		}
	}

	float HorizontalLinearLayout::calcInnerHeight()
	{

		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			if (getLayoutStyle(children[i]->layoutHeight) != WidgetAncestor::LayoutStyle::LS_MATCH_PARENT)
			{
				if (result < children[i]->getViewHeight())
				{
					result = children[i]->getViewHeight();
				}
			}
		}

		return result + paddingBottom + paddingTop + marginBottom + marginTop;
	}

	float HorizontalLinearLayout::calcInnerWidth()
	{
		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			result += children[i]->getViewWidth();

			if (i > 0)
			{
				result += itemSpacing;
			}
		}


		return result + paddingLeft + paddingRight + marginLeft + marginRight;
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

	Vector2f HorizontalLinearLayout::getContentTranslate()
	{
		Vector2f result;

		float diff = getContentAreaWidth() - getInnerWidth();

		if (childrenHA == HA_LEFT || widthIsFilled)
		{
			result(0) = paddingLeft;
		}
		else if (childrenHA == HA_CENTER)
		{
			result(0) = paddingLeft + (diff > 0 ? diff / 2.f : 0);
		}
		else
		{
			result(0) = paddingLeft + (diff > 0 ? diff : 0);
		}

		if (childrenVA == VA_TOP)
		{
			result(1) = getDrawHeight() - paddingTop;
		}
		else if (childrenVA == VA_CENTER)
		{
			result(1) = getContentAreaHeight() / 2.f + paddingBottom;
		}
		else
		{
			result(1) = paddingBottom;
		}

		return result;
	}

	Vector2f HorizontalLinearLayout::getChildTranslate(std::shared_ptr<WidgetAncestor> child)
	{
		float height = getContentAreaHeight();

		Vector2f result;
		result(0) = 0;

		if (childrenVA == VA_TOP)
		{
			result(1) = -child->getViewHeight();
		}
		else if (childrenVA == VA_CENTER)
		{
			result(1) = child->getViewHeight() <= height ? result(1) = -child->getViewHeight() / 2.f
				: height / 2.f - child->getViewHeight();
		}
		else
		{
			result(1) = child->getViewHeight() <= height ? 0 : height - child->getViewHeight();
		}

		return result;
	}

	void HorizontalLinearLayout::Draw()
	{
		WidgetAncestor::Draw();

		Vector3f shift, childShift;
		shift << getContentTranslate() + getDrawTranslate(), 0;
	
		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}
			
			childShift << getChildTranslate(children[i]), 0;

			Renderer->PushMatrix();
			Renderer->TranslateMatrix(childShift);

			children[i]->Draw();

			Renderer->PopMatrix();
			Renderer->TranslateMatrix(Vector3f(children[i]->getViewWidth() + itemSpacing, 0, 0));

		}

		CheckGlError();

		Renderer->PopMatrix();

	}


	void HorizontalLinearLayout::Update(size_t dt)
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->Update(dt);
		}
	}

	bool HorizontalLinearLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}
			
			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);
	
			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseDown(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseDown(pos, touchNumber);

		return handled;
	}

	bool HorizontalLinearLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

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

		return handled;
	}

	bool HorizontalLinearLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		return handled;
	}

	bool HorizontalLinearLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		bool childMoved = false;

		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

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
		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			children[i]->OnMouseCancel(touchNumber);
		}
	}

	void HorizontalLinearLayout::OnKeyPressed(int key)
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnKeyPressed(key);
		}
	}

	void HorizontalLinearLayout::OnMouseMove(Vector2f pos)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

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
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnMouseMoveOutside();
		}
	}

	void HorizontalLinearLayout::RemoveFocusRecursively()
	{
		focused = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->RemoveFocusRecursively();;
		}
	}

	//==========================================

	FrameLayout::FrameLayout(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
	{
	}

	float FrameLayout::calcInnerWidth()
	{
		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			LayoutStyle style = getLayoutStyle(children[i]->layoutWidth);

			if (style == LS_FIXED || style == LS_WRAP_CONTENT)
			{
				if (result < children[i]->getViewWidth())
				{
					result = children[i]->getViewWidth();
				}
			}
		}

		return result + paddingLeft + paddingRight + marginLeft + marginRight;
	}

	float FrameLayout::calcInnerHeight()
	{
		float result = 0;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			LayoutStyle style = getLayoutStyle(children[i]->layoutHeight);

			if (style == LS_FIXED || style == LS_WRAP_CONTENT)
			{
				if (result < children[i]->getViewHeight())
				{
					result = children[i]->getViewHeight();
				}
			}
		}

		return result + paddingBottom + paddingTop + marginBottom + marginTop;
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

		Vector3f shift, childShift;
		shift << getContentTranslate() + getDrawTranslate(), 0;

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			childShift << getChildTranslate(children[i]), 0;

			Renderer->PushMatrix();
			Renderer->TranslateMatrix(childShift);

			children[i]->Draw();

			Renderer->PopMatrix();
		}

		Renderer->PopMatrix();

		CheckGlError();

	}

	void FrameLayout::Update(size_t dt)
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->Update(dt);
		}
	}

	bool FrameLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseDown(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

		}

		WidgetAncestor::OnMouseDown(pos, touchNumber);

		return handled;
	}

	bool FrameLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUp(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);

		return handled;
	}

	bool FrameLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		return handled;
	}

	bool FrameLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		bool childMoved = false;

		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

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
		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			children[i]->OnMouseCancel(touchNumber);
		}
	}

	void FrameLayout::OnKeyPressed(int key)
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnKeyPressed(key);
		}
	}

	void FrameLayout::OnMouseMove(Vector2f pos)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

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
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnMouseMoveOutside();
		}
	}

	void FrameLayout::RemoveFocusRecursively()
	{
		focused = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->RemoveFocusRecursively();;
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
		WidgetAncestor::Draw();

		Vector3f shift, shift_child;
		shift << getContentTranslate() + getDrawTranslate(), 0;
		shift(1) += scroll + itemSpacing;
	
		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Renderer->TranslateMatrix(Vector3f(0, -children[i]->getViewHeight() - itemSpacing, 0));

			shift_child << getChildTranslate(children[i]), 0;

			Renderer->PushMatrix();
			Renderer->TranslateMatrix(shift_child);

			children[i]->Draw();

			Renderer->PopMatrix();

		}

		Renderer->PopMatrix();

		CheckGlError();

	}

	bool VerticalScrollLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing + scroll;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseDown(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}
		}
		WidgetAncestor::OnMouseDown(pos, touchNumber);

		return handled;
	}

	bool VerticalScrollLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing + scroll;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUp(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}
		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);

		return handled;
	}

	bool VerticalScrollLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing + scroll;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		return handled;
	}


	bool VerticalScrollLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		bool childMoved = false;

		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing + scroll;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				childMoved = childMoved | children[i]->OnMove(innerRelativePos, shift, touchNumber);
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}
		}

		if (childMoved) return true;
		
		WidgetAncestor::OnMove(pos, shift, touchNumber);

		float viewHeight = getContentAreaHeight();
		float contentHeight = getInnerHeight();

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
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(1) -= itemSpacing + scroll;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			relativePos(1) += children[i]->getViewHeight() + itemSpacing;

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				children[i]->OnMouseMove(innerRelativePos);
			}
			else
			{
				children[i]->OnMouseMoveOutside();
			}
		}

	}

	void VerticalScrollLayout::OnMouseMoveOutside()
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnMouseMoveOutside();
		}
	}

	//========================================


	HorizontalScrollLayout::HorizontalScrollLayout(WidgetParentInterface& widgetParent)
		: HorizontalLinearLayout(widgetParent)
		, scroll(0)
	{
	}

	void HorizontalScrollLayout::Draw()
	{
		WidgetAncestor::Draw();

		Vector3f shift, shift_child;
		shift << getContentTranslate() + getDrawTranslate(), 0;
		shift(0) -= scroll;

		Renderer->PushMatrix();

		Renderer->TranslateMatrix(shift);

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			shift_child << getChildTranslate(children[i]), 0;

			Renderer->PushMatrix();
			Renderer->TranslateMatrix(shift_child);

			children[i]->Draw();

			Renderer->PopMatrix();
			Renderer->TranslateMatrix(Vector3f(children[i]->getViewWidth() + itemSpacing, 0, 0));

		}

		Renderer->PopMatrix();

		CheckGlError();


	}

	bool HorizontalScrollLayout::OnMouseDown(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(0) += scroll;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseDown(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseDown(pos, touchNumber);

		return handled;
	}

	bool HorizontalScrollLayout::OnMouseUp(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(0) += scroll;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUp(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);

		return handled;
	}

	bool HorizontalScrollLayout::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(0) += scroll;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
			else
			{
				children[i]->OnMouseCancel(touchNumber);
			}

			relativePos(0) -= children[i]->getViewWidth() + itemSpacing;

		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		return handled;
	}

	bool HorizontalScrollLayout::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		bool childMoved = false;

		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(0) += scroll;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

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

		if (childMoved) return true;

		WidgetAncestor::OnMove(pos, shift, touchNumber);

		float viewWidth = getContentAreaWidth();
		float contentWidth = getInnerWidth();

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

		if (std::abs(shift(0)) > std::abs(shift(1)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void HorizontalScrollLayout::OnMouseMove(Vector2f pos)
	{
		Vector2f relativePos = pos - getContentTranslate() - getDrawTranslate();
		relativePos(0) += scroll;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = relativePos - getChildTranslate(children[i]);

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
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnMouseMoveOutside();
		}
	}


	//=======================================

	Label::Label(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
	{
		textParams.BasicTextAreaParams.HorizontalPadding = 0;
		textParams.BasicTextAreaParams.VerticalPadding = 0;

		textParams.BasicTextAreaParams.Height = DEFAULT_TEXT_LINE_HEIGHT;

		textParams.BasicTextAreaParams.TextHorizontalAlignment = THA_CENTER;


	}


	float Label::calcInnerWidth()
	{
		return ResourceManager->FontManager.GetTextAdvance(textParams.Text, textParams.FontName)
			+ 2 * textParams.BasicTextAreaParams.HorizontalPadding + 1 + //To prevent wrong word wrap
			+ paddingLeft + paddingRight + marginLeft + marginRight;
	}

	float Label::calcInnerHeight()
	{
		return textParams.BasicTextAreaParams.Height + 2 * textParams.BasicTextAreaParams.VerticalPadding +
			paddingBottom + paddingTop + marginBottom + marginTop;
	}


	void Label::Draw()
	{
		WidgetAncestor::Draw();

		TRenderParamsSetter render(textRenderPair.first);
		Renderer->DrawTriangleList(textRenderPair.second);

		CheckGlError();
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

		if (textParams.FontName == "")
		{
			textParams.FontName = ResourceManager->FontManager.GetCurrentFontName();
		}

		ResourceManager->FontManager.PushFont(textParams.FontName);

		Vector2f shift = getDrawTranslate();
		Vector2f posFrom = shift + Vector2f(paddingLeft, paddingBottom);
		Vector2f posTo = shift + Vector2f(getDrawWidth() - paddingRight, getDrawHeight() - paddingTop);

		textRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = ResourceManager->FontManager.GetCurrentFontTextureName();

		wrapped_text = textParams.Text;

		Vector2f realPosFrom =
			ResourceManager->FontManager.FitStringToBoxWithWordWrap(posFrom, posTo, textParams.BasicTextAreaParams, wrapped_text, textParams.FontName);

		textRenderPair.second =
			ResourceManager->FontManager.DrawStringToVBO(realPosFrom, textParams.BasicTextAreaParams, wrapped_text);

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

	float Button::calcInnerWidth()
	{
		float backgroundWidth = WidgetAncestor::calcInnerWidth();
		float textWidth = Label::calcInnerWidth();

		return backgroundWidth > textWidth ? backgroundWidth : textWidth;
	}

	float Button::calcInnerHeight()
	{
		float backgroundHeight = WidgetAncestor::calcInnerHeight();
		float textHeight = Label::calcInnerHeight();

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
		Vector2f shift = getDrawTranslate();
		Vector2f posFrom = shift;
		Vector2f posTo = shift + Vector2f(getDrawWidth(), getDrawHeight());

		bool isTexture = Visit(pressedDrawable,
			[this](Vector4f color) { return false; },
			[this](std::string textureName) { return true; });

		std::string textureName = Visit(pressedDrawable,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		Vector4f color = Visit(pressedDrawable,
			[this](Vector4f color) { return  color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 0); });

		pressedMaxAlpha = isTexture ? 1.f : color(3);

		pressedRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		pressedRenderPair.second.Data = MakeDataTriangleList(posFrom, posTo);

		for (auto& colorVec : pressedRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec << color(0), color(1), color(2), 0;
		}

		pressedRenderPair.second.RefreshBuffer();

		isTexture = Visit(hoverDrawable,
			[this](Vector4f color) { return false; },
			[this](std::string textureName) { return true; });

		textureName = Visit(hoverDrawable,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		color = Visit(hoverDrawable,
			[this](Vector4f color) { return  color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 0); });

		hoverMaxAlpha = isTexture ? 1.f : color(3);

		hoverRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		hoverRenderPair.second.Data = MakeDataTriangleList(posFrom, posTo);

		for (auto& colorVec : hoverRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec << color(0), color(1), color(2), 0;
		}

		hoverRenderPair.second.RefreshBuffer();
	}

	void Button::Draw()
	{
		WidgetAncestor::Draw();

		TRenderParamsSetter render2(hoverRenderPair.first);
		Renderer->DrawTriangleList(hoverRenderPair.second);

		CheckGlError();


		TRenderParamsSetter render1(pressedRenderPair.first);

		Renderer->DrawTriangleList(pressedRenderPair.second);

		CheckGlError();


		TRenderParamsSetter render3(textRenderPair.first);

		Renderer->DrawTriangleList(textRenderPair.second);

		CheckGlError();
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
			color(3) = buttonTimer / CONST_BUTTON_PRESS_TIME * pressedMaxAlpha;
		}

		for (auto& color : hoverRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			color(3) = hoverButtonTimer * (1.f - buttonTimer / CONST_BUTTON_PRESS_TIME) / 
				CONST_BUTTON_PRESS_TIME * hoverMaxAlpha;
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

	bool Button::OnMouseDown(Vector2f pos, int touchNumber)
	{
		if (buttonState == ButtonState::BS_NONE || buttonState == ButtonState::BS_EASING)
		{
			buttonState = BS_PRESSING;
		}
			
		WidgetAncestor::OnMouseDown(pos, touchNumber);

		return true;
	}

	bool Button::OnMouseUp(Vector2f pos, int touchNumber)
	{
		if (buttonState == ButtonState::BS_PRESSING || buttonState == ButtonState::BS_PRESSED)
		{
			buttonState = BS_EASING;
		}

		WidgetAncestor::OnMouseUp(pos, touchNumber);

		return true;
	}

	bool Button::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		if (buttonState == ButtonState::BS_PRESSING || buttonState == ButtonState::BS_PRESSED)
		{
			buttonState = BS_EASING;
		}

		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);

		return true;
	}

	void Button::OnMouseCancel(int touchNumber)
	{
		if (buttonState == ButtonState::BS_PRESSING || buttonState == ButtonState::BS_PRESSED)
		{
			buttonState = BS_EASING;
		}
	}

	void Button::OnMouseMove(Vector2f pos)
	{
		if (hoverButtonState == ButtonState::BS_NONE || hoverButtonState == ButtonState::BS_EASING)
		{
			hoverButtonState = BS_PRESSING;
		}
	}

	void Button::OnMouseMoveOutside()
	{
		if (hoverButtonState == ButtonState::BS_PRESSING || hoverButtonState == ButtonState::BS_PRESSED)
		{
			hoverButtonState = BS_EASING;
		}
	}

	//=======================================

	CheckBox::CheckBox(WidgetParentInterface& widgetParent)
		: WidgetAncestor(widgetParent)
		, checked(false)
	{ }

	void CheckBox::setCheckState(bool checked)
	{
		this->checked = checked;
		if (checked)
		{
			onChecked();
		}
		else
		{
			onUnchecked();
		}
	}

	void CheckBox::setCheckedSkin(const std::string &checkedSkin)
	{
		this->checkedSkin = checkedSkin.empty() ? CHECKED_DEFAULT_TEXTURE : checkedSkin;

#ifdef TARGET_WINDOWS_UNIVERSAL
		ResourceManager->TexList.AddTexture(this->checkedSkin);
#else
		ResourceManager->TexList.AddTexture("ui/" + this->checkedSkin);
#endif

		UpdateCheckedRenderPair();
	}

	void CheckBox::setUncheckedSkin(const std::string &uncheckedSkin)
	{
		this->uncheckedSkin = uncheckedSkin.empty() ? UNCHECKED_DEFAULT_TEXTURE : uncheckedSkin;

#ifdef TARGET_WINDOWS_UNIVERSAL
		ResourceManager->TexList.AddTexture(this->uncheckedSkin);
#else
		ResourceManager->TexList.AddTexture("ui/" + this->uncheckedSkin);
#endif

		UpdateCheckedRenderPair();
	}

	void CheckBox::UpdateRenderPair()
	{
		if (!inited)
		{
			return;
		}

		WidgetAncestor::UpdateRenderPair();
		UpdateCheckedRenderPair();
	}

	void CheckBox::UpdateCheckedRenderPair()
	{
		Vector2f shift = getDrawTranslate();
		Vector2f posFrom = shift;
		Vector2f posTo = shift + Vector2f(getDrawWidth(), getDrawHeight());

		checkedRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = checkedSkin;
		checkedRenderPair.second.Data = MakeDataTriangleList(posFrom, posTo);

		for (auto& colorVec : checkedRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = { 1, 1, 1, 1 };
		}

		checkedRenderPair.second.RefreshBuffer();

		uncheckedRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = uncheckedSkin;
		uncheckedRenderPair.second.Data = MakeDataTriangleList(posFrom, posTo);

		for (auto& colorVec : uncheckedRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = { 1, 1, 1, 1 };
		}

		uncheckedRenderPair.second.RefreshBuffer();
	}

	void CheckBox::Draw()
	{
		WidgetAncestor::Draw();

		if (checked)
		{
			TRenderParamsSetter render(checkedRenderPair.first);
			Renderer->DrawTriangleList(checkedRenderPair.second);
		}
		else
		{
			TRenderParamsSetter render(uncheckedRenderPair.first);
			Renderer->DrawTriangleList(uncheckedRenderPair.second);
		}

		CheckGlError();
	}

	bool CheckBox::OnMouseUp(Vector2f pos, int touchNumber)
	{
		setCheckState(!checked);
		return true;
	}

	bool CheckBox::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		setCheckState(!checked);
		return true;
	}


	//=======================================

	EditText::EditText(WidgetParentInterface& widgetParent)
		: Label(widgetParent),
		editTextTimer(0),
		symbolLimit(0),
		cursorAppeared(false)
	{
		textParams.BasicTextAreaParams.TextHorizontalAlignment = THA_LEFT;
	}

	void EditText::UpdateRenderPair()
	{
		Label::UpdateRenderPair();
		UpdateCursorRenderPair();
	}
	
	void EditText::UpdateCursorRenderPair()
	{
		if (!inited)
		{
			return;
		}

		cursorRenderPos = getCursorPos();

		Vector2f shift = getDrawTranslate() + Vector2f(paddingLeft, paddingBottom);

		Vector2f posFrom = shift;
		Vector2f posTo = shift + Vector2f(CURSOR_WIDTH, textParams.BasicTextAreaParams.Height);

		cursorRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = "white.bmp";

		cursorRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB] = MakeColorCoordVec(Vector4f(0, 0, 0, 1));
		cursorRenderPair.second.Data.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB] = MakeVertexCoordVec(posFrom, posTo);
		cursorRenderPair.second.Data.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB] = MakeTexCoordVec(Vector2f(0,0), Vector2f(1, 1));

		cursorRenderPair.second.RefreshBuffer();
	}

	void EditText::Draw()
	{
		Label::Draw();

		if (focused && cursorAppeared)
		{
			Renderer->PushMatrix();

			Renderer->TranslateMatrix(cursorRenderPos);

			TRenderParamsSetter render(cursorRenderPair.first);
			Renderer->DrawTriangleList(cursorRenderPair.second);

			Renderer->PopMatrix();

			CheckGlError();
		}
	}

	void EditText::setText(const std::string& text)
	{
		Label::setText(text);
	
		UpdateCursorRenderPair();
	}

	void EditText::setSymbolLimit(size_t limit)
	{
		symbolLimit = limit;
	}

	Vector3f EditText::getCursorPos()
	{
		Vector3f result;

		if (textParams.FontName != "")
		{
			ResourceManager->FontManager.PushFont(textParams.FontName);
		}
		else
		{
			ResourceManager->FontManager.PushFont(ResourceManager->FontManager.GetCurrentFontName());
		}

		if (textParams.BasicTextAreaParams.TextHorizontalAlignment == THA_RIGHT)
		{
			result(0) = getContentAreaWidth() - textParams.BasicTextAreaParams.HorizontalPadding;
		}
		else
		{
			float advance = 0;
			if (wrapped_text.length() > 0)
			{
				size_t i = wrapped_text.length() - 1;
				while (i != UINT32_MAX && wrapped_text[i] != '\n')
				{
					advance += ResourceManager->FontManager.GetCharAdvance(wrapped_text[i--], textParams.FontName);
				}
			}

			if (textParams.BasicTextAreaParams.TextHorizontalAlignment == THA_LEFT)
			{
				result(0) = textParams.BasicTextAreaParams.HorizontalPadding + advance;
			}
			else
			{
				result(0) = textParams.BasicTextAreaParams.HorizontalPadding + (getContentAreaWidth() + advance) / 2.f;
			}
		}

		if (textParams.BasicTextAreaParams.TextVerticalAlignment == TVA_BOTTOM)
		{
			result(1) = textParams.BasicTextAreaParams.VerticalPadding;
		}
		else
		{
			int lines = std::count(wrapped_text.begin(), wrapped_text.end(), '\n') + 1;

			if (textParams.BasicTextAreaParams.TextVerticalAlignment == TVA_TOP)
			{
				result(1) = textParams.BasicTextAreaParams.TextVerticalAlignment + getContentAreaHeight() -
					textParams.BasicTextAreaParams.Height * lines;
			}
			else
			{
				result(1) = textParams.BasicTextAreaParams.TextVerticalAlignment + (getContentAreaHeight() -
					textParams.BasicTextAreaParams.Height * lines) / 2;
			}
		}

		ResourceManager->FontManager.PopFont();

		result(2) = 0;
		return result;
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
				if (symbolLimit == 0 || textParams.Text.length() < symbolLimit)
				{
					textParams.Text += static_cast<char>(key);
				}
			}

			UpdateRenderPair();
		}
	}

	void EditText::Update(size_t dt)
	{
		editTextTimer += dt;
		while (editTextTimer / 500 > 1)
		{
			editTextTimer -= 500;
			cursorAppeared = !cursorAppeared;
		}
	}

	//--------------------------------------------------------------------------

	HorizontalSlider::HorizontalSlider(WidgetParentInterface& widgetParent) :
		WidgetAncestor(widgetParent),
		minValue(0),
		maxValue(100),
		position(0),
		buttonWidth(MIN_BUTTON_WIDTH),
		buttonPadding(0),
		trackPadding(0)
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

		std::string textureName = Visit(trackSkin,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		Vector4f color = Visit(trackSkin,
			[this](Vector4f color) { return color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 1); });

		Vector2f
			shift = getDrawTranslate(),
			from_point(paddingLeft + sidesPadding, paddingBottom + trackPadding),
			to_point(getDrawWidth() - paddingRight - sidesPadding, getDrawHeight() - paddingTop - trackPadding);

		if (from_point(0) > to_point(0)) from_point(0) = to_point(0);
		if (!isTexture || to_point(1) - from_point(1) + 1 < MIN_TRACK_HEIGHT)
		{
			from_point(1) = paddingBottom + getContentAreaHeight() / 2.f - MIN_TRACK_HEIGHT / 2.f;
			to_point(1) = paddingBottom + getContentAreaHeight() / 2.f + MIN_TRACK_HEIGHT / 2.f;
		}
		
		from_point += shift;
		to_point += shift;

		trackRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		trackRenderPair.second.Data = MakeDataTriangleList(from_point, to_point);

		for (auto& colorVec : trackRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = color;
		}

		trackRenderPair.second.RefreshBuffer();

		isTexture = Visit(buttonSkin,
			[this](Vector4f color) { return false; },
			[this](std::string textureName) { return true; });

		textureName = Visit(buttonSkin,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		color = Visit(buttonSkin,
			[this](Vector4f color) { return  color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 1); });

		from_point = { paddingLeft, paddingBottom + buttonPadding };
		to_point = { buttonWidth + paddingLeft, getDrawHeight() - paddingTop - buttonPadding };

		if (!isTexture || to_point(0) - from_point(0) + 1 < MIN_BUTTON_WIDTH)
		{
			from_point(0) = paddingLeft;
			to_point(0) = paddingLeft + MIN_BUTTON_WIDTH;
		}
		if (!isTexture || to_point(1) - from_point(1) + 1 < MIN_BUTTON_HEIGHT)
		{
			from_point(1) = paddingBottom + getContentAreaHeight() / 2.f - MIN_BUTTON_HEIGHT / 2.f;
			to_point(1) = paddingBottom + getContentAreaHeight() / 2.f + MIN_BUTTON_HEIGHT / 2.f;
		}

		from_point += shift;
		to_point += shift;

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
			if (maxValue != minValue)
			{
				onValueChanged(maxValue != minValue ? (position - minValue) / (float)(maxValue- minValue) : 0);
			}
		}
	}

	void HorizontalSlider::changeValue(float t)
	{
		if (t < 0)
		{
			t = 0;
		}

		if (t > 1)
		{
			t = 1;
		}

		int newPosition = minValue + t * (maxValue - minValue) + 0.5f;

		if (newPosition != this->position)
		{
			this->position = newPosition;
			onValueChanged(t);
		}
	}

	void HorizontalSlider::setMinValue(int minValue)
	{
		if (minValue > maxValue)
		{
			minValue = maxValue;
		}
		this->minValue = minValue;
		setPosition(position);
	}

	void HorizontalSlider::setMaxValue(int maxValue)
	{
		if (maxValue < minValue)
		{
			maxValue = minValue;
		}
		this->maxValue = maxValue;
		setPosition(position);
	}

	void HorizontalSlider::setButtonWidth(float width)
	{
		if (width < MIN_BUTTON_WIDTH)
		{
			width = MIN_BUTTON_WIDTH;
		}
		buttonWidth = width;
		sidesPadding = width / 2.f;
	}

	void HorizontalSlider::setButtonPadding(float padding)
	{
		buttonPadding = padding;
	}

	void HorizontalSlider::setTrackPadding(float padding)
	{
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
		WidgetAncestor::Draw();

		Renderer->PushMatrix();

		TRenderParamsSetter render1(trackRenderPair.first);
		Renderer->DrawTriangleList(trackRenderPair.second);

		CheckGlError();


		if (maxValue != minValue)
		{
			Renderer->TranslateMatrix(Vector3f((position - minValue) / (float)(maxValue - minValue) * (getContentAreaWidth() - buttonWidth), 0, 0));
		}
		else
		{
			Renderer->TranslateMatrix(Vector3f(0.5f * (getContentAreaWidth() - buttonWidth), 0, 0));
		}

		TRenderParamsSetter render2(buttonRenderPair.first);
		Renderer->DrawTriangleList(buttonRenderPair.second);

		Renderer->PopMatrix();

		CheckGlError();

	}

	bool HorizontalSlider::isPointAboveTrack(Vector2f point)
	{
		return (point(0) >= -sidesPadding && point(0) < getContentAreaWidth() - sidesPadding &&
			point(1) >= 0 && point(1) < getContentAreaHeight());
	}

	int HorizontalSlider::getTrackPositionFromPoint(Vector2f point)
	{
		return (int)(point(0) / (getContentAreaWidth() - buttonWidth) * (maxValue - minValue) + 0.5f) + minValue;
	}

	bool HorizontalSlider::OnMouseDown(Vector2f pos, int touchNumber)
	{
		pos -= Vector2f(paddingLeft + sidesPadding, paddingBottom) + getDrawTranslate();
		if (!isPointAboveTrack(pos)) return false;
		setPosition(getTrackPositionFromPoint(pos));
		WidgetAncestor::OnMouseDown(pos, touchNumber);
		return true;
	}

	bool HorizontalSlider::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		WidgetAncestor::OnMove(pos, shift, touchNumber);

		pos -= Vector2f(paddingLeft + sidesPadding, paddingBottom) + getDrawTranslate();
		setPosition(getTrackPositionFromPoint(pos));
		
		return true;
	}

	//---------------------------------------------------------------------

	HorizontalDoubleSlider::HorizontalDoubleSlider(WidgetParentInterface& widgetParent) :
		WidgetAncestor(widgetParent),
		minValue(0),
		maxValue(100),
		position1(0),
		position2(0),
		buttonWidth(MIN_BUTTON_WIDTH),
		buttonPadding(0),
		trackPadding(0),
		movingButton(0)
	{}

	HorizontalDoubleSlider::~HorizontalDoubleSlider() { }

	void HorizontalDoubleSlider::UpdateRenderPair()
	{
		if (!inited)
		{
			return;
		}

		WidgetAncestor::UpdateRenderPair();
		UpdateSkinRenderPairs();
	}

	void HorizontalDoubleSlider::UpdateSkinRenderPairs()
	{

		bool isTexture = Visit(trackSkin,
			[this](Vector4f color) { return false; },
			[this](std::string textureName) { return true; });

		std::string textureName = Visit(trackSkin,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		Vector4f color = Visit(trackSkin,
			[this](Vector4f color) { return color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 1); });

		Vector2f
			shift = getDrawTranslate(),
			from_point(paddingLeft + buttonWidth, paddingBottom + trackPadding),
			to_point(getDrawWidth() - paddingRight - buttonWidth, getDrawHeight() - paddingTop - trackPadding);

		if (from_point(0) > to_point(0)) from_point(0) = to_point(0);
		if (!isTexture || to_point(1) - from_point(1) + 1 < MIN_TRACK_HEIGHT)
		{
			from_point(1) = paddingBottom + getContentAreaHeight() / 2.f - MIN_TRACK_HEIGHT / 2.f;
			to_point(1) = paddingBottom + getContentAreaHeight() / 2.f + MIN_TRACK_HEIGHT / 2.f;
		}

		from_point += shift;
		to_point += shift;

		trackRenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		trackRenderPair.second.Data = MakeDataTriangleList(from_point, to_point);

		for (auto& colorVec : trackRenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = color;
		}

		trackRenderPair.second.RefreshBuffer();

		isTexture = Visit(button1Skin,
			[this](Vector4f color) { return false; },
			[this](std::string textureName) { return true; });

		textureName = Visit(button1Skin,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		color = Visit(button1Skin,
			[this](Vector4f color) { return  color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 1); });

		from_point = { paddingLeft, paddingBottom + buttonPadding };
		to_point = { paddingLeft + buttonWidth, getDrawHeight() - paddingTop - buttonPadding };

		if (!isTexture || to_point(0) - from_point(0) + 1 < MIN_BUTTON_WIDTH)
		{
			from_point(0) = paddingLeft;
			to_point(0) = paddingLeft + MIN_BUTTON_WIDTH;
		}
		if (!isTexture || to_point(1) - from_point(1) + 1 < MIN_BUTTON_HEIGHT)
		{
			from_point(1) = paddingBottom + (getContentAreaHeight() - MIN_BUTTON_HEIGHT) / 2.f;
			to_point(1) = paddingBottom + (getContentAreaHeight() + MIN_BUTTON_HEIGHT) / 2.f;
		}

		from_point += shift;
		to_point += shift;

		button1RenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		button1RenderPair.second.Data = MakeDataTriangleList(from_point, to_point);

		for (auto& colorVec : button1RenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = color;
		}

		button1RenderPair.second.RefreshBuffer();

		isTexture = Visit(button2Skin,
			[this](Vector4f color) { return false; },
			[this](std::string textureName) { return true; });

		textureName = Visit(button2Skin,
			[this](Vector4f color) { return "white.bmp"; },
			[this](std::string textureName) { return textureName; });

		color = Visit(button2Skin,
			[this](Vector4f color) { return  color; },
			[this](std::string textureName) { return Vector4f(1, 1, 1, 1); });

		from_point = { paddingLeft + buttonWidth, paddingBottom + buttonPadding };
		to_point = { paddingLeft + buttonWidth * 2, getDrawHeight() - paddingTop - buttonPadding };

		if (!isTexture || to_point(0) - from_point(0) + 1 < MIN_BUTTON_WIDTH)
		{
			from_point(0) = paddingLeft + MIN_BUTTON_WIDTH;
			to_point(0) = paddingLeft + 2 * MIN_BUTTON_WIDTH;
		}
		if (!isTexture || to_point(1) - from_point(1) + 1 < MIN_BUTTON_HEIGHT)
		{
			from_point(1) = paddingBottom + (getContentAreaHeight() - MIN_BUTTON_HEIGHT) / 2.f;
			to_point(1) = paddingBottom + (getContentAreaHeight() + MIN_BUTTON_HEIGHT) / 2.f;
		}

		from_point += shift;
		to_point += shift;

		button2RenderPair.first.SamplerMap[CONST_STRING_TEXTURE_UNIFORM] = textureName;
		button2RenderPair.second.Data = MakeDataTriangleList(from_point, to_point);

		for (auto& colorVec : button2RenderPair.second.Data.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB])
		{
			colorVec = color;
		}

		button2RenderPair.second.RefreshBuffer();
	}

	void HorizontalDoubleSlider::Draw()
	{
		WidgetAncestor::Draw();

		TRenderParamsSetter render1(trackRenderPair.first);
		Renderer->DrawTriangleList(trackRenderPair.second);

		CheckGlError();

		Renderer->PushMatrix();

		if (maxValue != minValue)
		{
			Renderer->TranslateMatrix(Vector3f((position1 - minValue) / (float)(maxValue - minValue) * (getContentAreaWidth() - 2 * buttonWidth), 0, 0));
		}
		else
		{
			Renderer->TranslateMatrix(Vector3f(0.5f * (getContentAreaWidth() - 2 * buttonWidth), 0, 0));
		}

		TRenderParamsSetter render2(button1RenderPair.first);
		Renderer->DrawTriangleList(button1RenderPair.second);

		Renderer->PopMatrix();
		Renderer->PushMatrix();

		if (maxValue != minValue)
		{
			Renderer->TranslateMatrix(Vector3f((position2 - minValue) / (float)(maxValue - minValue) * (getContentAreaWidth() - 2 * buttonWidth), 0, 0));
		}
		else
		{
			Renderer->TranslateMatrix(Vector3f(0.5f * (getContentAreaWidth() - 2 * buttonWidth), 0, 0));
		}

		TRenderParamsSetter render3(button2RenderPair.first);
		Renderer->DrawTriangleList(button2RenderPair.second);

		Renderer->PopMatrix();

		CheckGlError();
	}

	void HorizontalDoubleSlider::changeValue1(float t)
	{
		if (t < 0)
		{
			t = 0;
		}

		if (t > 1)
		{
			t = 1;
		}

		int newPosition = minValue + t * (maxValue - minValue) + 0.5f;

		if (newPosition != this->position2)
		{
			this->position2 = newPosition;
			onValueChanged(t, (position2 - minValue) / float(maxValue - minValue));
		}
	}

	void HorizontalDoubleSlider::changeValue2(float t)
	{
		if (t < 0)
		{
			t = 0;
		}

		if (t > 1)
		{
			t = 1;
		}

		int newPosition = minValue + t * (maxValue - minValue) + 0.5f;

		if (newPosition != this->position2)
		{
			this->position2 = newPosition;
			onValueChanged((position1 - minValue) / float(maxValue - minValue), t);
		}
	}

	void HorizontalDoubleSlider::setPosition1(int position)
	{
		if (position < minValue) position = minValue;
		if (position > maxValue) position = maxValue;
		if (position > position2) position = position2;
		if (this->position1 != position)
		{
			this->position1 = position;
			if (maxValue != minValue)
			{
				onValueChanged((position1 - minValue) / (float)(maxValue - minValue),
					(position2 - minValue) / (float)(maxValue - minValue));
			}
			else
			{
				onValueChanged(0, 0);
			}
		}
	}

	void HorizontalDoubleSlider::setPosition2(int position)
	{
		if (position < minValue) position = minValue;
		if (position > maxValue) position = maxValue;
		if (position < position1) position = position1;
		if (this->position2 != position)
		{
			this->position2 = position;
			if (maxValue != minValue)
			{
				onValueChanged((position1 - minValue) / (float)(maxValue - minValue),
					(position2 - minValue) / (float)(maxValue - minValue));
			}
			else
			{
				onValueChanged(0, 0);
			}
		}
	}


	void HorizontalDoubleSlider::setMinValue(int minValue)
	{
		if (minValue > maxValue)
		{
			minValue = maxValue;
		}
		this->minValue = minValue;
		setPosition1(position1);
		setPosition2(position2);
	}

	void HorizontalDoubleSlider::setMaxValue(int maxValue)
	{
		if (maxValue < minValue)
		{
			maxValue = minValue;
		}
		this->maxValue = maxValue;
		setPosition1(position1);
		setPosition2(position2);
	}

	void HorizontalDoubleSlider::setButtonWidth(float width)
	{
		if (width < MIN_BUTTON_WIDTH)
		{
			width = MIN_BUTTON_WIDTH;
		}
		buttonWidth = width;
	}

	void HorizontalDoubleSlider::setButtonPadding(float padding)
	{
		buttonPadding = padding;
	}

	void HorizontalDoubleSlider::setTrackPadding(float padding)
	{
		trackPadding = padding;
	}

	void HorizontalDoubleSlider::setButton1Skin(boost::variant<std::string, Vector4f> buttonSkin)
	{
		this->button1Skin = buttonSkin;
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

	void HorizontalDoubleSlider::setButton2Skin(boost::variant<std::string, Vector4f> buttonSkin)
	{
		this->button2Skin = buttonSkin;
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

	void HorizontalDoubleSlider::setTrackSkin(boost::variant<std::string, Vector4f> trackSkin)
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

	int HorizontalDoubleSlider::getTrackPositionFromPoint(Vector2f point)
	{
		return (int)(point(0) / (getContentAreaWidth() - 2 * buttonWidth) * (maxValue - minValue) + 0.5f) + minValue;
	}
	
	bool HorizontalDoubleSlider::isPointAboveTrack(Vector2f point)
	{
		return (point(0) >= -buttonWidth && point(0) < getContentAreaWidth() - buttonWidth &&
			point(1) >= 0 && point(1) < getContentAreaHeight());
	}

	int HorizontalDoubleSlider::getButtonNumberFromPosition(int position)
	{
		float middle = (position1 + position2) / 2.f;
		return position < middle ? 1 : 2;
	}

	bool HorizontalDoubleSlider::OnMouseDown(Vector2f pos, int touchNumber)
	{
		pos -= Vector2f(paddingLeft + buttonWidth, paddingBottom) + getDrawTranslate();
		if (!isPointAboveTrack(pos)) return false;
		int position = getTrackPositionFromPoint(pos);
		if ((movingButton = getButtonNumberFromPosition(position)) > 0)
		{
			if (movingButton == 1)
			{
				setPosition1(position);
			}
			else
			{
				setPosition2(position);
			}
		}
		WidgetAncestor::OnMouseDown(pos, touchNumber);
		return true;
	}

	bool HorizontalDoubleSlider::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		WidgetAncestor::OnMove(pos, shift, touchNumber);
		if (movingButton == 0) return false;
		pos -= Vector2f(paddingLeft + buttonWidth, paddingBottom) + getDrawTranslate();
		int position = getTrackPositionFromPoint(pos);
		if (movingButton == 1)
		{
			setPosition1(position);
		}
		else
		{
			setPosition2(position);
		}
		return true;
	}
	
	bool HorizontalDoubleSlider::OnMouseUp(Vector2f pos, int touchNumber)
	{
		movingButton = 0;
		WidgetAncestor::OnMouseUp(pos, touchNumber);
		return true;
	}

	bool HorizontalDoubleSlider::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		movingButton = 0;
		WidgetAncestor::OnMouseUpAfterMove(pos, touchNumber);
		return true;
	}

	//======================================
	
	NewGuiManager::NewGuiManager()
		: inited(false)
	{
	}

	NewGuiManager::~NewGuiManager()
	{

	}

	float NewGuiManager::getHeight()
	{
		return Renderer->GetMatrixHeight();
	}


	float NewGuiManager::getWidth()
	{
		return Renderer->GetMatrixWidth();
	}


	void NewGuiManager::Init()
	{
		inited = true;
	}

	void NewGuiManager::Deinit()
	{

	}

	bool NewGuiManager::IsInited()
	{
		return inited;
	}

	void NewGuiManager::Update(size_t dt)
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->Update(dt);
		}
	}

	void NewGuiManager::Draw()
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->Draw();
		}
	}

	bool NewGuiManager::OnMouseDown(Vector2f pos, int touchNumber)
	{
		pos(0) = pos(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		pos(1) = pos(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			children[i]->RemoveFocusRecursively();

			Vector2f innerRelativePos = pos;
			innerRelativePos(1) -= getHeight() - children[i]->getViewHeight();
		
			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseDown(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
		}

		return handled;
	}

	bool NewGuiManager::OnMouseUp(Vector2f pos, int touchNumber)
	{
		pos(0) = pos(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		pos(1) = pos(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();

		Vector2f relativePos = pos;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = pos;
			innerRelativePos(1) -= getHeight() - children[i]->getViewHeight();

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUp(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
		}

		return handled;
	}

	bool NewGuiManager::OnMouseUpAfterMove(Vector2f pos, int touchNumber)
	{
		pos(0) = pos(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		pos(1) = pos(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();

		Vector2f relativePos = pos;

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = pos;
			innerRelativePos(1) -= getHeight() - children[i]->getViewHeight();

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMouseUpAfterMove(innerRelativePos, touchNumber))
				{
					handled = true;
				}
			}
		}

		return handled;
	}

	bool NewGuiManager::OnMove(Vector2f pos, Vector2f shift, int touchNumber)
	{
		pos(0) = pos(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		pos(1) = pos(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();

		shift(0) = shift(0) * Renderer->GetMatrixWidth() / Renderer->GetScreenWidth();
		shift(1) = shift(1) * Renderer->GetMatrixHeight() / Renderer->GetScreenHeight();

		bool handled = false;

		for (size_t i = 0; i < children.size(); i++)
		{
			if (!children[i]->visible)
			{
				continue;
			}

			Vector2f innerRelativePos = pos;
			innerRelativePos(1) -= getHeight() - children[i]->getViewHeight();

			if (pointIsInsideView(innerRelativePos, children[i]))
			{
				if (children[i]->OnMove(innerRelativePos, shift, touchNumber))
				{
					handled = true;
				}
			}
		}

		return handled;
	}

	void NewGuiManager::OnKeyPressed(int key)
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnKeyPressed(key);
		}
	}

	void NewGuiManager::OnMouseMove(Vector2f pos)
	{
		for (auto &child : children)
		{
			if (!child->visible)
			{
				continue;
			}
			child->OnMouseMove(pos);
		}
	}

	void NewGuiManager::shareLeftoverWidthBetweenChildren()
	{
		if (children.size() == 0) return;

		for (auto &child : children)
		{
			if (getLayoutStyle(child->layoutWidth) == WidgetAncestor::LayoutStyle::LS_MATCH_PARENT)
			{
				child->calculatedLayoutWidth = getWidth();
				child->calculatedInnerWidth = child->calcInnerWidth();
			}
			child->shareLeftoverWidthBetweenChildren();
		}
	}

	void NewGuiManager::recalculateInnerWidth()
	{
		shareLeftoverWidthBetweenChildren();
	}

	void NewGuiManager::recalculateInnerHeight()
	{
		shareLeftoverHeightBetweenChildren();
	}

	void NewGuiManager::shareLeftoverHeightBetweenChildren()
	{
		if (children.size() == 0) return;

		for (auto &child : children)
		{
			if (getLayoutStyle(child->layoutHeight) == WidgetAncestor::LayoutStyle::LS_MATCH_PARENT)
			{
				child->calculatedLayoutHeight = getHeight();
				child->calculatedInnerHeight = child->calcInnerHeight();
			}
			child->shareLeftoverHeightBetweenChildren();
		}
	}

	void NewGuiManager::UpdateOnWindowResize()
	{
		shareLeftoverWidthBetweenChildren();
		shareLeftoverHeightBetweenChildren();

		UpdateRenderPair();
	}

	void NewGuiManager::UpdateRenderPair()
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

		UpdateOnWindowResize();

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

				if (pWidgetRecord.second.count("textParams") != 0)
				{
					label->textParams.Serialize(pWidgetRecord.second.find("textParams")->second);
				}
				else
				{
					label->textParams = TTextParams();
				}

				widget = label;
			}
			if (type == "EditText")
			{
				auto editText = parentWidget.CreateAndAddChildOfType<EditText>();

				editText->setSymbolLimit(pWidgetRecord.second.get<size_t>("symbolLimit", 0));

				if (pWidgetRecord.second.count("textParams") != 0)
				{
					editText->textParams.Serialize(pWidgetRecord.second.find("textParams")->second);
				}
				else
				{
					editText->textParams = TTextParams();
				}

				widget = editText;
			}
			if (type == "CheckBox")
			{
				auto checkBox = parentWidget.CreateAndAddChildOfType<CheckBox>();

				checkBox->setCheckedSkin(pWidgetRecord.second.get<std::string>("pressedDrawable", ""));
				checkBox->setUncheckedSkin(pWidgetRecord.second.get<std::string>("hoverDrawable", ""));
				checkBox->setCheckState(pWidgetRecord.second.get<int>("checked", 0));

				widget = checkBox;
			}
			if (type == "Button")
			{
				auto button = parentWidget.CreateAndAddChildOfType<Button>();

				if (pWidgetRecord.second.count("textParams") != 0)
				{
					button->textParams.Serialize(pWidgetRecord.second.find("textParams")->second);
				}
				else
				{
					button->textParams = TTextParams();
				}

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

				slider->setMaxValue(pWidgetRecord.second.get<int>("maxValue", 100));
				slider->setMinValue(pWidgetRecord.second.get<int>("minValue", 0));
				slider->setPosition(pWidgetRecord.second.get<int>("position", 0));
				slider->setButtonWidth(pWidgetRecord.second.get<int>("buttonWidth", HorizontalSlider::MIN_BUTTON_WIDTH));
				slider->setButtonPadding(pWidgetRecord.second.get<int>("buttonPadding", 0));
				slider->setTrackPadding(pWidgetRecord.second.get<int>("trackPadding", 0));
				slider->setButtonSkin(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("buttonSkin", "#000000FF")));
				slider->setTrackSkin(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("trackSkin", "#000000FF")));

				widget = slider;
			}
			if (type == "HorizontalDoubleSlider")
			{
				auto slider = parentWidget.CreateAndAddChildOfType<HorizontalDoubleSlider>();

				slider->setMaxValue(pWidgetRecord.second.get<int>("maxValue", 100));
				slider->setMinValue(pWidgetRecord.second.get<int>("minValue", 0));
				slider->setPosition1(pWidgetRecord.second.get<int>("position1", 0));
				slider->setPosition2(pWidgetRecord.second.get<int>("position2", 0));
				slider->setButtonWidth(pWidgetRecord.second.get<int>("buttonWidth", HorizontalDoubleSlider::MIN_BUTTON_WIDTH));
				slider->setButtonPadding(pWidgetRecord.second.get<int>("buttonPadding", 0));
				slider->setTrackPadding(pWidgetRecord.second.get<int>("trackPadding", 0));
				slider->setButton1Skin(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("button1Skin", "#000000FF")));
				slider->setButton2Skin(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("button2Skin", "#000000FF")));
				slider->setTrackSkin(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("trackSkin", "#000000FF")));

				widget = slider;
			}

			widget->name = pWidgetRecord.second.get<std::string>("name", "");

			widget->setPadding(pWidgetRecord.second.get<float>("paddingTop", 0.f), pWidgetRecord.second.get<float>("paddingBottom", 0.f), pWidgetRecord.second.get<float>("paddingLeft", 0.f), pWidgetRecord.second.get<float>("paddingRight", 0.f));

			widget->setMargin(pWidgetRecord.second.get<float>("marginTop", 0.f), pWidgetRecord.second.get<float>("marginBottom", 0.f), pWidgetRecord.second.get<float>("marginLeft", 0.f), pWidgetRecord.second.get<float>("marginRight", 0.f));
			
			widget->setExtraTranslation(pWidgetRecord.second.get<float>("extraTranslationX", 0.f), pWidgetRecord.second.get<float>("extraTranslationY", 0.f));

			widget->setBackground(layoutBackgroundFromConfigValue(pWidgetRecord.second.get<std::string>("background", "#00000000")));

			widget->setLayoutWidth(layoutDimentionFromConfigValue(pWidgetRecord.second.get<std::string>("width", "wrap_content")));

			widget->setLayoutHeight(layoutDimentionFromConfigValue(pWidgetRecord.second.get<std::string>("height", "wrap_content")));

			widget->setBorderColor(layoutColorFromConfigValue(pWidgetRecord.second.get<std::string>("borderColor", "#000000FF")));
			
			widget->setBorderType(borderTypeFromConfigValue(pWidgetRecord.second.get<std::string>("borderType", "none")));

			widget->setChildrenHorizontalAlignment(layoutHorizontalAlignmentFromConfigValue(pWidgetRecord.second.get<std::string>("horizontalAlignment", "HA_LEFT")));

			widget->setChildrenVerticalAlignment(layoutVerticalAlignmentFromConfigValue(pWidgetRecord.second.get<std::string>("verticalAlignment", "VA_TOP")));
			
			widget->visible = pWidgetRecord.second.get<int>("visible", 1);
			
			widget->inited = true;			

		}


	}

	WidgetAncestor::childrenHorizontalAlignment NewGuiManager::layoutHorizontalAlignmentFromConfigValue(std::string configValue)
	{
		boost::to_upper(configValue);

		if (configValue == "HA_RIGHT")
		{
			return WidgetAncestor::childrenHorizontalAlignment::HA_RIGHT;
		}
		else if (configValue == "HA_CENTER")
		{
			return WidgetAncestor::childrenHorizontalAlignment::HA_CENTER;
		}
		else
		{
			return WidgetAncestor::childrenHorizontalAlignment::HA_LEFT;
		}
	}

	WidgetAncestor::childrenVerticalAlignment NewGuiManager::layoutVerticalAlignmentFromConfigValue(std::string configValue)
	{
		boost::to_upper(configValue);

		if (configValue == "VA_BOTTOM")
		{
			return WidgetAncestor::childrenVerticalAlignment::VA_BOTTOM;
		}
		else if (configValue == "VA_CENTER")
		{
			return WidgetAncestor::childrenVerticalAlignment::VA_CENTER;
		}
		else
		{
			return WidgetAncestor::childrenVerticalAlignment::VA_TOP;
		}
	}

	boost::variant<float, WidgetAncestor::LayoutStyle> NewGuiManager::layoutDimentionFromConfigValue(std::string configValue)
	{
		boost::to_lower(configValue);

		if (configValue == "match_parent")
		{
			return WidgetAncestor::LayoutStyle::LS_MATCH_PARENT;
		}
		else if (configValue == "wrap_content")
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

	Vector4f NewGuiManager::layoutColorFromConfigValue(std::string configValue)
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

	WidgetAncestor::BorderType NewGuiManager::borderTypeFromConfigValue(std::string configValue)
	{
		if (configValue == "line")
		{
			return WidgetAncestor::BorderType::BT_LINE;
		}
		else
		{
			return WidgetAncestor::BorderType::BT_NONE;
		}
	}

	boost::variant<std::string, Vector4f> NewGuiManager::layoutBackgroundFromConfigValue(std::string configValue)
	{

		if (boost::starts_with(configValue, "#"))
		{
			return layoutColorFromConfigValue(configValue);
		}
		else
		{
			return configValue;
		}

	}

	



} //namespace SE
