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

	cardinal byteCount;

	boost::shared_array<char> charmapFileArr = boost::shared_array<char>(CreateMemFromFile<char>(ResourceManager->PathToResources+charmapFile, byteCount));

	//Need to rewrite this code :(

	TFontParams fontParams;
	cardinal character;

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


float TFontManager::DrawChar(vec2 pos, cardinal character)
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

	vec2 p1 = pos + vec2(+ scale_x*fontParams.InternalShiftX, -height- scale_y*fontParams.InternalShiftY);
	vec2 p2 = pos + vec2(width + scale_x*fontParams.InternalShiftX, - scale_y* fontParams.InternalShiftY);
	
	vec2 t1 = vec2(fontParams.ShiftX,
		1 - fontParams.ShiftY - fontParams.BitmapHeight - fontParams.InternalShiftY);
	vec2 t2 = vec2(fontParams.ShiftX + fontParams.BitmapWidth, 
		1 - fontParams.ShiftY - fontParams.InternalShiftY);
	
	Renderer->DrawRect(p1, p2, t1, t2);

	return fontParams.Advance*scale_x;
}

float TFontManager::DrawCharToVBO(vec2 pos, cardinal character, TTriangleList& triangleList)
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

	float height = scale_y*fontParams.BitmapHeight;

	float width = scale_x*fontParams.BitmapWidth;
	
	vec2 p1 = pos + vec2(+ scale_x*fontParams.InternalShiftX, -height - scale_y*fontParams.InternalShiftY);
	vec2 p2 = pos + vec2(width + scale_x*fontParams.InternalShiftX, - scale_y* fontParams.InternalShiftY);

	
	vec2 t1 = vec2(fontParams.ShiftX,
		1 - fontParams.ShiftY - fontParams.BitmapHeight - fontParams.InternalShiftY);
	vec2 t2 = vec2(fontParams.ShiftX + fontParams.BitmapWidth, 
		1 - fontParams.ShiftY - fontParams.InternalShiftY);

	triangleList.Data += MakeDataTriangleList(p1, p2, t1, t2);
	
	return fontParams.Advance*scale_x;
}


vec2 TFontManager::FitStringToBoxWithWordWrap(vec2 posFrom, vec2 posTo, TTextBasicAreaParams params, std::string& str)
{

	std::string fontName = GetCurrentFontName();

	float intervalY = CONST_HEIGHT_COEF * params.Height;

	size_t rows = 1;
    
    //Apply padding:
    
    posFrom.v[0] += params.HorizontalPadding;
    posTo.v[0] -= params.HorizontalPadding;
    posFrom.v[1] += params.VerticalPadding;
    posTo.v[1] -= params.VerticalPadding;
    

	float maxWidth = posTo.v[0] - posFrom.v[0];


	//cardinal p = 0;
	vec2 cursor;


	std::vector<std::string> explodedByParagraph;

	boost::split(explodedByParagraph, str, boost::is_any_of("\n"), boost::token_compress_on);

	std::string result;


	for (size_t i=0; i<explodedByParagraph.size(); i++)
	{
		std::vector<std::string> explodedByWord;

		boost::split(explodedByWord, explodedByParagraph[i], boost::is_space(), boost::token_compress_on);

		for (size_t j=0; j< explodedByWord.size(); j++)
		{
			std::string s = explodedByWord[j];

			float adv = GetTextAdvance(s);

			if (adv + cursor.v[0] < maxWidth)
			{
				result += s + " ";
				cursor.v[0] += adv + GetCharAdvance(' ');
			}
			else
			{
				if (adv < maxWidth)
				{
					result += "\n" + s + " "; 
					cursor.v[0] = adv + GetCharAdvance(' ');
				}
				else
				{
					for (int k=0; k<s.size(); k++)
					{
						result += s[k];
						cursor.v[0] += GetCharAdvance(s[k]);
						if (cursor.v[0] > maxWidth)
						{
							cursor.v[0] = 0;
							result += "\n";
						}
					}
				}
			}
		}

		result += "\n";
		cursor.v[0] = 0;

	}

	//Erase last symbol \n
	result.erase(result.end()-1, result.end());


	str = result;

	size_t found = str.find_first_of('\n');
	
	while (found != std::string::npos)
	{
		rows++;
		found = str.find_first_of('\n', found + 1);
	}
    
    vec2 result_vec = vec2(posFrom.v[0], posFrom.v[1] + intervalY*rows);
    
    if (params.TextHorizontalAlignment == THA_RIGHT)
    {
        result_vec.v[0] = posTo.v[0];
    }
    else if (params.TextHorizontalAlignment == THA_CENTER)
    {
        result_vec.v[0] = (posTo.v[0] + posFrom.v[0])*0.5f;
    }
    
    if (params.TextVerticalAlignment == TVA_TOP)
    {
        result_vec.v[1] = posTo.v[1];
    }
    else if (params.TextVerticalAlignment == TVA_CENTER)
    {
        result_vec.v[1] = (posTo.v[1] + posFrom.v[1])*0.5f;
    }

	return result_vec;

}

TTriangleList TFontManager::DrawStringToVBO(vec2 pos, TTextBasicAreaParams params, const std::string& str)
{

	std::wstring ws;
	utf8toWStr(ws, str);

	std::string fontName = GetCurrentFontName();

	float width;

	TTriangleList triangleList;
    
    std::vector<std::wstring> rowArr;
    
    boost::split(rowArr, ws, boost::algorithm::is_any_of("\n"));

	vec2 startPos = pos;
    
    float rowsHeight = rowArr.size() * CONST_HEIGHT_COEF*params.Height;
    
	
    BOOST_FOREACH(auto& rowStr, rowArr)
    {
        
        float rowWidth = GetTextAdvance(rowStr);
        
        for (cardinal i=0; i<rowStr.size(); i++)
        {
            vec2 realPos = pos;
            if (params.TextHorizontalAlignment == THA_RIGHT)
            {
                realPos.v[0] = realPos.v[0] - rowWidth;
            }
            else if (params.TextHorizontalAlignment == THA_CENTER)
            {
                realPos.v[0] = realPos.v[0] - rowWidth*0.5f;
				realPos.v[0] = floorf(realPos.v[0]);
            }
            
            if (params.TextVerticalAlignment == TVA_CENTER)
            {
                realPos.v[1] = realPos.v[1] + rowsHeight*0.5f;
				realPos.v[1] = floorf(realPos.v[1]);
            }
            
            width = DrawCharToVBO(realPos, rowStr[i], triangleList);
            
            pos += vec2(width, 0);
        }
        
        pos.v[1] -= CONST_HEIGHT_COEF*params.Height;
        pos.v[0] = startPos.v[0];
    }

	triangleList.RefreshBuffer();

	return triangleList;
}
	

void TFontManager::DrawString(vec2 pos, TTextBasicAreaParams params, const std::string& str)
{
	TTriangleList triangleList = DrawStringToVBO(pos, params, str);

	Renderer->DrawTriangleList(triangleList);

}

void TFontManager::DrawTextInBox(vec2 posFrom, vec2 posTo, TTextBasicAreaParams params, std::string str, bool wordWrap)
{
	vec2 realPosFrom;
	if (wordWrap)
	{
		realPosFrom = FitStringToBoxWithWordWrap(posFrom, posTo, params, str);
	}
	else
	{
		//realPosFrom = FitStringToBox(posFrom, posTo, params, str);
		throw ErrorToLog("Word wrap not supported!");
	}

	DrawString(realPosFrom, params, str);
}

TTriangleList TFontManager::DrawTextInBoxToVBO(vec2 posFrom, vec2 posTo, TTextBasicAreaParams params, std::string str, bool wordWrap)
{

	vec2 realPosFrom;
	if (wordWrap)
	{
		realPosFrom = FitStringToBoxWithWordWrap(posFrom, posTo, params, str);
	}
	else
	{
		throw ErrorToLog("Word wrap not supported!");
		//realPosFrom = FitStringToBox(posFrom, posTo, params, str);
	}
	return DrawStringToVBO(realPosFrom, params, str);
}

} //namespace SE