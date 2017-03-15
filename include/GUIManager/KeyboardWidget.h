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
		Vector2i Pos;
		Vector2i WidthHeight;

		TVisualKey()
		{
		}

		TVisualKey(Vector2i pos, Vector2i widthHeight)
			: Pos(pos)
			, WidthHeight(widthHeight)
		{
		}

		bool CheckClicked(Vector2i p)
		{
			
			return (p(0) >= Pos(0) - WidthHeight(0)*0.5f && p(0) <= Pos(0) + WidthHeight(0)*0.5f) &&
				(p(1) >= Pos(1) - WidthHeight(1)*0.5f && p(1) <= Pos(1) + WidthHeight(1)*0.5f);
		}
	};

	
struct TKeyboardWidget : public TInstancingWidgetAncestor
{
	
protected:
	std::vector<std::tuple<TVisualKey, int, int>> VisualKeyArr;

	bool Shifted;


public:

	TKeyboardWidget();

	virtual bool CheckClick(Vector2f mousePos);
	virtual void OnTapDown(Vector2f pos);
	virtual void OnTapUp(Vector2f pos);

	/*
	static TSquareStatic* CreateStatic(Vector2f posFrom, Vector2f posTo);
	static TSquareStatic* CreateStaticWithFiller(Vector2f posFrom, Vector2f posTo, boost::function<void(TSquareStatic*)> staticFillerFunc);
	static TSquareStatic* CreateStaticWithFillers(Vector2f posFrom, Vector2f posTo, std::vector<boost::function<void(TSquareStatic*)>> staticFillerFuncArr);

	static void StaticTextureFiller(Vector2f posFrom, Vector2f posTo, const std::string& texName, TSquareStatic* staticToFill);
	static void StaticTextFiller(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParams, TSquareStatic* staticToFill);

	static TSquareStatic* CreateStaticTexture(Vector2f posFrom, Vector2f posTo, const std::string& texName);
	static TSquareStatic* CreateStaticText(Vector2f posFrom, Vector2f posTo, TTextParams buttonTextParams);*/

};



}





#endif