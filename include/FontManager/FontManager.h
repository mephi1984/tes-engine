#ifndef FONT_MANAGER_H_INCLUDED
#define FONT_MANAGER_H_INCLUDED


#include "include/Utils/Utils.h"


namespace SE
{

enum TTextHorizontalAlignment
{
	THA_LEFT,
	THA_CENTER,
	THA_RIGHT
};

enum TTextVerticalAlignment
{
	TVA_BOTTOM,
	TVA_CENTER,
	TVA_TOP
};

struct TTextBasicAreaParams : public TSerializeInterface
{
    int Height;
	float HorizontalPadding;
	float VerticalPadding;
	TTextHorizontalAlignment TextHorizontalAlignment;
	TTextVerticalAlignment TextVerticalAlignment;
    
    TTextBasicAreaParams(int height = 12, float horizontalPadding = 2, float verticalPadding = 2, TTextHorizontalAlignment textHorizontalAlignment = THA_LEFT, TTextVerticalAlignment textVerticalAlignment = TVA_BOTTOM)
    : Height(height)
    , HorizontalPadding(horizontalPadding)
    , VerticalPadding(verticalPadding)
    , TextHorizontalAlignment(textHorizontalAlignment)
    , TextVerticalAlignment(textVerticalAlignment)
	{
	}
    
    virtual void Serialize(boost::property_tree::ptree& propertyTree)
	{
		Height = propertyTree.get<int>("<xmlattr>.height");
		HorizontalPadding = propertyTree.get<float>("<xmlattr>.horizontalPadding");
		VerticalPadding = propertyTree.get<float>("<xmlattr>.verticalPadding");
        
		std::string textHorizontalAlignment = propertyTree.get<std::string>("<xmlattr>.textHorizontalAlignment");
		
		if (textHorizontalAlignment == "THA_RIGHT")
		{
			TextHorizontalAlignment = THA_RIGHT;
		}
		else if (textHorizontalAlignment == "THA_CENTER")
		{
			TextHorizontalAlignment = THA_CENTER;
		}
		else
		{
			TextHorizontalAlignment = THA_LEFT;
		}
        
		std::string textVerticalAlignment = propertyTree.get<std::string>("<xmlattr>.textVerticalAlignment");
		
        
		if (textVerticalAlignment == "TVA_TOP")
		{
			TextVerticalAlignment = TVA_TOP;
		}
		else if (textVerticalAlignment == "TVA_CENTER")
		{
			TextVerticalAlignment = TVA_CENTER;
		}
		else
		{
			TextVerticalAlignment = TVA_BOTTOM;
		}
		
        
	}
};



struct TFontParams
{
	float ShiftX;
	float ShiftY;
	float InternalShiftX;
	float InternalShiftY;
	float BitmapHeight;
	float BitmapWidth;
	float Advance;
};

struct TFont
{
	std::string TexName;
	cardinal SheetWidth;
	cardinal SheetHeight;
	std::map<cardinal, TFontParams> CharMap;
};

class TTriangleList; //HalibutRender.h

class TFontManager : public TSerializeInterface
{
protected:
	std::map<std::string, TFont> FontMap;

	std::stack<std::string> FontStack;

	float DrawChar(vec2 pos, cardinal character);
	float DrawCharToVBO(vec2 pos, cardinal character, TTriangleList& triangleList);

	vec2 FitStringToBoxWithWordWrap(vec2 posFrom, vec2 posTo, TTextBasicAreaParams params, std::string& str);
public:
	TFontManager() { }
	~TFontManager();

	virtual void Serialize(boost::property_tree::ptree& propertyTree);

	void AddFont(const std::string& fontName, const std::string& bitmapFile, const std::string& charmapFile);
	
	void PushFont(const std::string& fontName);
	void PopFont();

	void DrawString(vec2 pos, TTextBasicAreaParams params, const std::string& str);
	
	TTriangleList DrawStringToVBO(vec2 pos, TTextBasicAreaParams params, const std::string& str);
	
	void DrawTextInBox(vec2 posFrom, vec2 posTo, TTextBasicAreaParams params, std::string str, bool wordWrap = true);

	std::string GetCurrentFontName();
	std::string GetCurrentFontTextureName();

	TTriangleList DrawTextInBoxToVBO(vec2 posFrom, vec2 posTo, TTextBasicAreaParams params, std::string str, bool wordWrap = true);


	template<typename CHARTYPE>
float GetCharAdvance(CHARTYPE character)
{
	std::string fontName = GetCurrentFontName();

	if (FontMap.count(fontName) == 0)
	{
		throw ErrorToLog("Trying to use unknown font! " + fontName);
	}

	TFont& currentFont = FontMap[fontName];

	static_assert(sizeof(CHARTYPE) <= 4, "LOL, we have to implement more than 4-byte character!"); // LOL, we have to implement more than 4-byte character!
		
	TFontParams& fontParams = currentFont.CharMap[static_cast<cardinal>(character)];


	int scale_x = currentFont.SheetWidth;

	return fontParams.Advance*scale_x;

}

template<typename STRINGTYPE>
float GetTextAdvance(STRINGTYPE text)
{

	boost::erase_all(text, "\n"); //that is easier

	float advance = 0;

	for (size_t i=0; i<text.size(); i++)
	{
		advance += GetCharAdvance(text[i]);
	}

	return advance;
}

};


} //namespace SE


#endif