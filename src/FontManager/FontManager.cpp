#include "include/FontManager/FontManager.h"
#include "include/Engine.h"

#include <boost/algorithm/string.hpp>

namespace SE
{

const float CONST_HEIGHT_COEF = 1.0f;

const int CONST_MAX_FONT_STACK_SIZE = 10;



TFontManager::~TFontManager()
{
	*Console<<"TFontManager deleting...";
}

void TFontManager::Serialize(boost::property_tree::ptree& propertyTree)
{
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree.get_child("Fonts"))
	{
	
		std::string fontName = v.second.get<std::string>("FontName");
		std::string bitmapFileName = v.second.get<std::string>("BitmapFileName");
		std::string charmapFileName = v.second.get<std::string>("CharmapFileName");

		AddFont(fontName, bitmapFileName, charmapFileName);

	}

}

void TFontManager::AddFont(const std::string& fontName, const std::string& bitmapFile, const std::string&  charmapFile)
{
	if (FontMap.count(fontName) > 0)
	{
		throw ErrorToLog("Trying to add font twice or some font files not found!" + fontName);
	}

	TFont& currentFont = FontMap[fontName];

	std::string texName = GetFileName(bitmapFile);

	currentFont.TexName = texName;

	ResourceManager->TexList.AddTexture(bitmapFile);

	currentFont.SheetWidth = ResourceManager->TexList.GetTextureWidth(texName);
	currentFont.SheetHeight = ResourceManager->TexList.GetTextureHeight(texName);

	size_t byteCount;

	boost::shared_array<char> charmapFileArr = boost::shared_array<char>(CreateMemFromFile<char>(ST::PathToResources+charmapFile, byteCount));

	//Need to rewrite this code :(

	TFontParams fontParams;
	size_t character;

	std::stringstream charmapFileStream;
	charmapFileStream.write(&charmapFileArr[0], byteCount);

	while(!charmapFileStream.eof())
	{

		charmapFileStream>>character;
		charmapFileStream>>fontParams.ShiftX;
		charmapFileStream>>fontParams.ShiftY;
		charmapFileStream>>fontParams.InternalShiftX;
		charmapFileStream>>fontParams.InternalShiftY;
		charmapFileStream>>fontParams.BitmapWidth;
		charmapFileStream>>fontParams.BitmapHeight;
		charmapFileStream>>fontParams.Advance;

		currentFont.CharMap[character] = fontParams;
	}


	
}
	


void TFontManager::PushFont(const std::string& fontName)
{
	if (FontStack.size() >= CONST_MAX_FONT_STACK_SIZE)
	{
		throw ErrorToLog("Error: font stack overflow!");
	}
	FontStack.push(fontName);
}

void TFontManager::PopFont()
{
	if (FontStack.size() == 0)
	{
		throw ErrorToLog("Error: font stack underflow!");
	}

	FontStack.pop();
}

std::string TFontManager::GetCurrentFontName()
{
	if (FontStack.size() == 0)
	{
		throw ErrorToLog("Error: font stack underflow!");
	}

	return FontStack.top();
}

std::string TFontManager::GetCurrentFontTextureName()
{
	return FontMap[GetCurrentFontName()].TexName;
}


float TFontManager::DrawChar(Vector2f pos, size_t character)
{
	//Also see DrawCharToVBO

	std::string fontName = GetCurrentFontName();

	if (FontMap.count(fontName) == 0)
	{
		throw ErrorToLog("Trying to use unknown font! " + fontName);
	}

	TFont& currentFont = FontMap[fontName];

	TFontParams& fontParams = currentFont.CharMap[character];

	int scale_y = currentFont.SheetHeight;
	int scale_x = currentFont.SheetWidth;

	float height = scale_y*fontParams.BitmapHeight;

	float width = scale_x*fontParams.BitmapWidth;

	Vector2f p1 = pos + Vector2f(+ scale_x*fontParams.InternalShiftX, -height- scale_y*fontParams.InternalShiftY);
	Vector2f p2 = pos + Vector2f(width + scale_x*fontParams.InternalShiftX, - scale_y* fontParams.InternalShiftY);
	
	Vector2f t1 = Vector2f(fontParams.ShiftX,
		1 - fontParams.ShiftY - fontParams.BitmapHeight - fontParams.InternalShiftY);
	Vector2f t2 = Vector2f(fontParams.ShiftX + fontParams.BitmapWidth, 
		1 - fontParams.ShiftY - fontParams.InternalShiftY);
	
	Renderer->DrawRect(p1, p2, t1, t2);

	return fontParams.Advance*scale_x;
}

float TFontManager::DrawCharToVBO(Vector2f pos, size_t character, TTriangleList& triangleList)
{
	//Also see DrawChar

	std::string fontName = GetCurrentFontName();

	if (FontMap.count(fontName) == 0)
	{
		throw ErrorToLog("Trying to use unknown font! " + fontName);
	}

	TFont& currentFont = FontMap[fontName];

	TFontParams& fontParams = currentFont.CharMap[character];

	int scale_y = currentFont.SheetHeight;
	int scale_x = currentFont.SheetWidth;

	float height = scale_y * fontParams.BitmapHeight;
	float width = scale_x * fontParams.BitmapWidth;

	// pos is the left top corner of the rectangle containing the character

	Vector2f p1 = pos + Vector2f(scale_x * fontParams.InternalShiftX, -height - scale_y * fontParams.InternalShiftY);
	Vector2f p2 = pos + Vector2f(width + scale_x * fontParams.InternalShiftX, - scale_y * fontParams.InternalShiftY);
	
	Vector2f t1 = Vector2f(fontParams.ShiftX, 1 - fontParams.ShiftY - fontParams.BitmapHeight - fontParams.InternalShiftY);
	Vector2f t2 = Vector2f(fontParams.ShiftX + fontParams.BitmapWidth, 1 - fontParams.ShiftY - fontParams.InternalShiftY);

	triangleList.Data += MakeDataTriangleList(p1, p2, t1, t2);
	
	return fontParams.Advance * scale_x;
}



Vector2f TFontManager::FitStringToBoxWithWordWrap(Vector2f posFrom, Vector2f posTo, TTextBasicAreaParams params, std::string& str, const std::string& fontName)
{
	std::string fontName_ = fontName.size() > 0 ? fontName : GetCurrentFontName();

	float intervalY = CONST_HEIGHT_COEF * params.Height;

	size_t rows = 1;
    
    //Apply padding:
    
    posFrom(0) += params.HorizontalPadding;
    posTo(0) -= params.HorizontalPadding;
    posFrom(1) += params.VerticalPadding;
    posTo(1) -= params.VerticalPadding;
    

	float maxWidth = posTo(0) - posFrom(0);


	float cursor = 0;

	std::vector<std::string> explodedByParagraph;
	boost::split(explodedByParagraph, str, boost::is_any_of("\n"));

	std::string result, substr;

	float ws_adv = GetCharAdvance(' ', fontName_);

	for (auto &paragraph : explodedByParagraph)
	{
		for (auto iter = paragraph.begin(); iter != paragraph.end(); ++iter)
		{
			float adv;

			if (*iter != ' ')
			{
				auto end = iter + 1;
				while (end != paragraph.end() && *end != ' ') ++end;
				substr = std::string(iter, end);
				adv = GetTextAdvance(substr, fontName_);
				iter = end - 1;
			}
			else
			{
				adv = ws_adv;
				substr = " ";
			}

			if (adv + cursor <= maxWidth)
			{
				result += substr;
				cursor += adv;
			}
			else
			{
				if (adv <= maxWidth)
				{
					result += "\n" + substr;
					cursor = adv;
				}
				else
				{
					for (auto iter = substr.begin(); iter != substr.end(); ++iter)
					{
						result += *iter;
						cursor += GetCharAdvance(*iter, fontName_);
						if (cursor > maxWidth)
						{
							cursor = 0;
							result += "\n";
						}
					}
				}
			}
		}
		result += "\n";
	}

	cursor = 0;

	//Erase last symbol \n
	result.erase(result.end()-1, result.end());


	str = result;

	size_t found = str.find_first_of('\n');
	
	while (found != std::string::npos)
	{
		rows++;
		found = str.find_first_of('\n', found + 1);
	}
    
	// result_vec is the left top corner of the rectangle containing the text
    Vector2f result_vec = Vector2f(posFrom(0), posTo(1));
    
    if (params.TextHorizontalAlignment == THA_RIGHT)
    {
        result_vec(0) = posTo(0);
    }
    else if (params.TextHorizontalAlignment == THA_CENTER)
    {
        result_vec(0) = posFrom(0) + (posTo(0) - posFrom(0)) / 2.f;
    }
    
    if (params.TextVerticalAlignment == TVA_BOTTOM)
    {
        result_vec(1) = posFrom(1) + intervalY * rows;
    }
    else if (params.TextVerticalAlignment == TVA_CENTER)
    {
        result_vec(1) = posFrom(1) + (posTo(1) - posFrom(1) + intervalY * rows) / 2.f;
    }

	return result_vec;

}

TTriangleList TFontManager::DrawStringToVBO(Vector2f realPos, TTextBasicAreaParams params, const std::string& str)
{

	std::wstring ws;
	utf8toWStr(ws, str);

	std::string fontName = GetCurrentFontName();

	float width;

	TTriangleList triangleList;
    
    std::vector<std::wstring> rowArr;
    
    boost::split(rowArr, ws, boost::algorithm::is_any_of("\n"));

	// realPosFrom is the left top corner of the rectangle containing the string
	float startX = realPos(0);
    
    float rowsHeight = rowArr.size() * CONST_HEIGHT_COEF*params.Height;
    
	
    for(auto& rowStr : rowArr)
    {
        
        float rowWidth = GetTextAdvance(rowStr);
        
		if (params.TextHorizontalAlignment == THA_RIGHT)
		{
			realPos(0) -= rowWidth;
		}
		else if (params.TextHorizontalAlignment == THA_CENTER)
		{
			realPos(0) = floorf(realPos(0) - rowWidth * 0.5f);
		}

        for (size_t i=0; i<rowStr.size(); i++)
        {
            width = DrawCharToVBO(realPos, rowStr[i], triangleList);            
			realPos(0) += width;
        }

		realPos(0) = startX;
		realPos(1) -= CONST_HEIGHT_COEF*params.Height;
    }

	triangleList.RefreshBuffer();

	return triangleList;
}
	

void TFontManager::DrawString(Vector2f pos, TTextBasicAreaParams params, const std::string& str)
{
	TTriangleList triangleList = DrawStringToVBO(pos, params, str);

	Renderer->DrawTriangleList(triangleList);

}

void TFontManager::DrawTextInBox(Vector2f posFrom, Vector2f posTo, TTextBasicAreaParams params, std::string str, bool wordWrap)
{
	Vector2f realPosFrom;
	if (wordWrap)
	{
		realPosFrom = FitStringToBoxWithWordWrap(posFrom, posTo, params, str, GetCurrentFontName());
	}
	else
	{
		//realPosFrom = FitStringToBox(posFrom, posTo, params, str);
		throw ErrorToLog("Word wrap not supported!");
	}

	DrawString(realPosFrom, params, str);
}

TTriangleList TFontManager::DrawTextInBoxToVBO(Vector2f posFrom, Vector2f posTo, TTextBasicAreaParams params, std::string str, bool wordWrap)
{
	// realPosFrom is the left top corner of the rectangle containing the text
	Vector2f realPosFrom;
	if (wordWrap)
	{
		realPosFrom = FitStringToBoxWithWordWrap(posFrom, posTo, params, str, GetCurrentFontName());
	}
	else
	{
		throw ErrorToLog("Word wrap not supported!");
		//realPosFrom = FitStringToBox(posFrom, posTo, params, str);
	}
	return DrawStringToVBO(realPosFrom, params, str);
}

} //namespace SE