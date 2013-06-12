#ifndef KEYBOARD_WIDGET_H_INCLUDED
#define KEYBOARD_WIDGET_H_INCLUDED

#include "include/Render/RenderMisc.h"
#include "include/Render/RenderParams.h"
#include "include/Utils/Utils.h"
#include "include/FontManager/FontManager.h"

#include "include/GUIManager/ButtonWidget.h"

namespace SE
{

	struct TVisualKey
	{
		ivec2 Pos;
		ivec2 WidthHeight;

		TVisualKey()
		{
		}

		TVisualKey(ivec2 pos, ivec2 widthHeight)
			: Pos(pos)
			, WidthHeight(widthHeight)
		{
		}

		bool CheckClicked(ivec2 p)
		{
			return (p.v[0] >= Pos.v[0] - WidthHeight.v[0]*0.5f && p.v[0] <= Pos.v[0] + WidthHeight.v[0]*0.5f) &&
				(p.v[1] >= Pos.v[1] - WidthHeight.v[1]*0.5f && p.v[1] <= Pos.v[1] + WidthHeight.v[1]*0.5f);
		}
	};

	
struct TKeyboardWidget : public TInstancingWidgetAncestor
{
	
protected:
	std::vector<std::tuple<TVisualKey, int, int>> VisualKeyArr;

	bool Shifted;


public:

	TKeyboardWidget();

	virtual bool CheckClick(vec2 mousePos);
	virtual void OnTapDown(vec2 pos);
	virtual void OnTapUp(vec2 pos);

	/*
	static TSquareStatic* CreateStatic(vec2 posFrom, vec2 posTo);
	static TSquareStatic* CreateStaticWithFiller(vec2 posFrom, vec2 posTo, boost::function<void(TSquareStatic*)> staticFillerFunc);
	static TSquareStatic* CreateStaticWithFillers(vec2 posFrom, vec2 posTo, std::vector<boost::function<void(TSquareStatic*)>> staticFillerFuncArr);

	static void StaticTextureFiller(vec2 posFrom, vec2 posTo, const std::string& texName, TSquareStatic* staticToFill);
	static void StaticTextFiller(vec2 posFrom, vec2 posTo, TTextParams buttonTextParams, TSquareStatic* staticToFill);

	static TSquareStatic* CreateStaticTexture(vec2 posFrom, vec2 posTo, const std::string& texName);
	static TSquareStatic* CreateStaticText(vec2 posFrom, vec2 posTo, TTextParams buttonTextParams);*/

};



}





#endif