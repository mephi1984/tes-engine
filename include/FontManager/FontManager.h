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
    
    TTextBasicAreaParams(int height = 18, float horizontalPadding = 0, float verticalPadding = 0, TTextHorizontalAlignment textHorizontalAlignment = THA_LEFT, TTextVerticalAlignment textVerticalAlignment = TVA_BOTTOM)
    : Height(height)
    , HorizontalPadding(horizontalPadding)
    , VerticalPadding(verticalPadding)
    , TextHorizontalAlignment(textHorizontalAlignment)
    , TextVerticalAlignment(textVerticalAlignment)
	{
	}
    
    virtual void Serialize(boost::property_tree::ptree& propertyTree)
	{
		Height = propertyTree.get<int>("height", 18);
		HorizontalPadding = propertyTree.get<float>("horizontalPadding", 0);
		VerticalPadding = propertyTree.get<float>("verticalPadding", 0);
        
		std::string textHorizontalAlignment = propertyTree.get<std::string>("textHorizontalAlignment", "THA_LEFT");
		
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
        
		std::string textVerticalAlignment = propertyTree.get<std::string>("textVerticalAlignment", "TVA_BOTTOM");
		
        
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


	virtual void SerializeXml(boost::property_tree::ptree& propertyTree)
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
	size_t SheetWidth;
	size_t SheetHeight;
	std::map<size_t, TFontParams> CharMap;
};

class TTriangleList; //HalibutRender.h

class TFontManager : public TSerializeInterface
{
protected:
	std::map<std::string, TFont> FontMap;

	std::stack<std::string> FontStack;

	float DrawChar(Vector2f pos, size_t character);
	float DrawCharToVBO(Vector2f pos, size_t character, TTriangleList& triangleList, float zLevel = 0);

public:
	TFontManager() { }
	~TFontManager();

	virtual void Serialize(boost::property_tree::ptree& propertyTree);

	void AddFont(const std::string& fontName, const std::string& bitmapFile, const std::string& charmapFile);
	
	void PushFont(const std::string& fontName);
	void PopFont();

	Vector2f FitStringToBoxWithWordWrap(Vector2f posFrom, Vector2f posTo, TTextBasicAreaParams params, std::string& str, const std::string& fontName = "");

	void DrawString(Vector2f pos, TTextBasicAreaParams params, const std::string& str);
	
	TTriangleList DrawStringToVBO(Vector2f pos, TTextBasicAreaParams params, const std::string& str, float zLevel = 0);
	
	void DrawTextInBox(Vector2f posFrom, Vector2f posTo, TTextBasicAreaParams params, std::string str, bool wordWrap = true);

	std::string GetCurrentFontName();
	std::string GetCurrentFontTextureName();

	TTriangleList DrawTextInBoxToVBO(Vector2f posFrom, Vector2f posTo, TTextBasicAreaParams params, std::string str, bool wordWrap = true, float zLevel = 0);


template<typename CHARTYPE>
float GetCharAdvance(CHARTYPE character, const std::string &fontName = "")
{
	std::string fontName_ = fontName.size() > 0 ? fontName : GetCurrentFontName();

	if (FontMap.count(fontName_) == 0)
	{
		throw ErrorToLog("Trying to use unknown font! " + fontName_);
	}

	TFont& currentFont = FontMap[fontName_];

	static_assert(sizeof(CHARTYPE) <= 4, "LOL, we have to implement more than 4-byte character!"); // LOL, we have to implement more than 4-byte character!
		
	TFontParams& fontParams = currentFont.CharMap[static_cast<size_t>(character)];


	int scale_x = currentFont.SheetWidth;

	return fontParams.Advance*scale_x;

}

template<typename STRINGTYPE>
float GetTextAdvance(STRINGTYPE text, const std::string &fontName = "")
{
	std::string fontName_ = fontName.size() > 0 ? fontName : GetCurrentFontName();

	boost::erase_all(text, "\n"); //that is easier

	float advance = 0;

	for (size_t i=0; i<text.size(); i++)
	{
		advance += GetCharAdvance(text[i], fontName_);
	}

	return advance;
}

};


} //namespace SE


#endif