#include "include/TextureManager/SalmonTexture.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

#include "boost/gil/gil_all.hpp"
#include "boost/gil/extension/numeric/sampler.hpp"
#include "boost/gil/extension/numeric/resample.hpp"

#include "boost/assign.hpp"

#include "jpeglib.h"
#include <setjmp.h>

namespace SE
{

TTextureListClass::TTextureListClass()
{
    using namespace boost::assign;
    
    using namespace std;
 
	CreateFunctionMap[".bmp"] = boost::bind(&TTextureListClass::CreateTexDataFromBmp24, this, _1, _2);
	CreateFunctionMap[".bmp32"] = boost::bind(&TTextureListClass::CreateTexDataFromBmp32, this, _1, _2);
	CreateFunctionMap[".png"] = boost::bind(&TTextureListClass::CreateTexDataFromPng, this, _1, _2);
	CreateFunctionMap[".tga"] = boost::bind(&TTextureListClass::CreateTexDataFromTga, this, _1, _2);
    CreateFunctionMap[".jpg"] = boost::bind(&TTextureListClass::CreateTexDataFromJpg, this, _1, _2);
	CreateFunctionMap[".jpeg"] = boost::bind(&TTextureListClass::CreateTexDataFromJpg, this, _1, _2);
   

	AddFunctionMap["bmp24"] = [this](TTextureData& texData) -> cardinal
	{
		boost::function<cardinal()> f = boost::bind(&TTextureListClass::AddTextureBmp24Data, this, texData);
		return PerformInMainThread<cardinal>(f);
	};


	AddFunctionMap["bmp32"] = [this](TTextureData& texData) -> cardinal
	{
		boost::function<cardinal()> f = boost::bind(&TTextureListClass::AddTextureBmp32Data, this, texData);
		return PerformInMainThread<cardinal>(f);
	};
	
    
}

TTextureListClass::~TTextureListClass() 
{ 
	Clear();
	
	*Console<<"ResourceManager::TexList deleting";
}

void TTextureListClass::Clear()
{
	PerformInMainThreadAsync(boost::bind(&TTextureListClass::InnerClear, this));
}

cardinal TTextureListClass::InnerAddEmptyTexture(const std::string& texName, cardinal width, cardinal height)
{
	AssertIfInMainThread();

	cardinal texID;

	if (TexMap.count(texName) == 0)
	{

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		#ifdef TARGET_WIN32
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		#endif
		
		#ifndef TARGET_WIN32 //TARGET_ANDROID or TARGET_IOS
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		#endif
	
		
	}
	else
	{
		TexMap[texName].RefCount++;
		texID = TexMap[texName].TexID;
	}

	return texID;


}
cardinal TTextureListClass::InnerAddEmptyCubemapTexture(const std::string& texName, cardinal width, cardinal height)
{
	AssertIfInMainThread();
	
	cardinal texID;

	if (TexMap.count(texName) == 0)
	{

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

		#ifdef TARGET_WIN32
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
 
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		#endif
		
		#ifndef TARGET_WIN32 //TARGET_IOS or TARGET_ANDROID
	
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		
		#endif
	}
	else
	{
		TexMap[texName].RefCount++;
		texID = TexMap[texName].TexID;
	}

	return texID;
}

cardinal TTextureListClass::InnerAddDepthTexture(const std::string& texName, cardinal width, cardinal height)
{
	AssertIfInMainThread();

	
	#ifdef TARGET_WIN32
	cardinal texID;

	if (TexMap.count(texName) == 0)
	{

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

	}
	else
	{
		TexMap[texName].RefCount++;
		texID = TexMap[texName].TexID;
	}

	return texID;
	#endif
	
	#ifdef TARGET_ANDROID
	
	throw ErrorToLog("Trying to create depth texture on Android!!!");
	
	return 0;
	
	#endif
#ifdef TARGET_IOS
	
	throw ErrorToLog("Trying to create depth texture on iOS!!!");
	
	return 0;
	
#endif

}

void TTextureListClass::InnerDeleteTexture(TTextureMap::iterator itr)
{
	AssertIfInMainThread();

	(itr->second.RefCount)--;

	if (itr->second.RefCount == 0)
	{
		glDeleteTextures(1, &(itr->second.TexID));

		*Console << "ResourceManager::TexList texture " + (itr->first) + " deleted";

		TexMap.erase(itr);
	}
	else
		*Console<<"ResourceManager::TexList texture "+(itr->first)+" reference deleted";

}


void TTextureListClass::InnerClear()
{

	AssertIfInMainThread();

	if (TexMap.size() != 0)
	{
        
        BOOST_FOREACH(auto& i, TexMap)
        {
			glDeleteTextures(1,&(i.second.TexID));
        }

		TexMap.clear();

		*Console<<"ResourceManager::TexList cleared";
	}
}

void TTextureListClass::Serialize(boost::property_tree::ptree& propertyTree)
{
	
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, propertyTree.get_child("Textures"))
	{
		std::string fileName = v.second.get<std::string>("FileName");
		std::string texName = v.second.get<std::string>("TexName", "");

		AddTexture(fileName, texName);
	}
}

void TTextureListClass::NormalizeTexData(TTextureData& texData)
{

	using namespace boost::gil;

	if (IsPower2(texData.Width) && IsPower2(texData.Height))
	{
		return;
	}

	cardinal newWidth = GetGreaterPower2(texData.Width);
	cardinal newHeight = GetGreaterPower2(texData.Height);

	if (!strcmp(texData.Format, "bmp32"))
	{

		boost::shared_array<char> newData(new char [newWidth * newHeight * 4]);

		rgba8_view_t oldView = interleaved_view(texData.Width, texData.Height, reinterpret_cast<const rgba8_ptr_t>(texData.Data.get()), texData.DataSize / texData.Height);

		rgba8_image_t newImage(newWidth, newHeight);		

		resize_view(oldView, view(newImage), bilinear_sampler());

		texData.Data = newData;
		texData.Width = newWidth;
		texData.Height = newHeight;
		texData.DataSize = newWidth * newHeight * 4;

		oldView = interleaved_view(texData.Width, texData.Height, reinterpret_cast<const rgba8_ptr_t>(texData.Data.get()), texData.DataSize / texData.Height);

		copy_pixels(view(newImage), oldView);	

	}
	else if (!strcmp(texData.Format, "bmp24"))
	{
		boost::shared_array<char> newData(new char [newWidth * newHeight * 3]);

		rgb8_view_t oldView = interleaved_view(texData.Width, texData.Height, reinterpret_cast<const rgb8_ptr_t>(texData.Data.get()), texData.DataSize / texData.Height);

		rgb8_image_t newImage(newWidth, newHeight);		

		resize_view(oldView, view(newImage), bilinear_sampler());

		texData.Data = newData;
		texData.Width = newWidth;
		texData.Height = newHeight;
		texData.DataSize = newWidth * newHeight * 3;

		oldView = interleaved_view(texData.Width, texData.Height, reinterpret_cast<const rgb8_ptr_t>(texData.Data.get()), texData.DataSize / texData.Height);

		copy_pixels(view(newImage), oldView);	
	}
	else
	{
		throw ErrorToLog("TTextureListClass::NormalizeTexData Format unsupported: "+std::string(texData.Format));
	}
}


bool TTextureListClass::CreateTexDataFromBmp24(const std::string& filename, TTextureData& texData)
{

	cardinal fileSize;
	boost::shared_array<char> fileArr = CreateMemFromFile<char>(filename, fileSize);
	
	if (fileSize<22)
		throw ErrorFileNotCorrect(filename);

	//This refers to BITMAPV5HEADER

	strcpy(texData.Format, "bmp24");
	texData.Width = *reinterpret_cast<cardinal*>(&fileArr[18]);
	texData.Height = *reinterpret_cast<cardinal*>(&fileArr[22]);
	
	texData.DataSize = texData.Height * texData.Width * 3;

	texData.Data = boost::shared_array<char>(new char [texData.DataSize]);
	
	cardinal pos = *reinterpret_cast<cardinal*>(&fileArr[10]);
	cardinal x = 0;

	for (cardinal i=0; i<texData.Width; i++)
		for(cardinal j=0; j<texData.Height; j++)
		{
				
			if (pos+3>fileSize)
				throw ErrorFileTooShort(filename);

			x = (i * texData.Height + j) + (i * texData.Height + j) + (i * texData.Height + j);
	
			texData.Data[x + 2] = fileArr[pos++];
			texData.Data[x + 1] = fileArr[pos++];
			texData.Data[x + 0] = fileArr[pos++];
		
		}

	return true;
}


bool TTextureListClass::CreateTexDataFromBmp32(const std::string& filename, TTextureData& texData)
{
	cardinal fileSize;
	
	boost::shared_array<char> fileArr = CreateMemFromFile<char>(filename, fileSize);

	if (fileSize<22)
		throw ErrorFileNotCorrect(filename);

	//Meaning BITMAPV5HEADER

	strcpy(texData.Format, "bmp32");
	texData.Width = *reinterpret_cast<cardinal*>(&fileArr[18]);
	texData.Height = *reinterpret_cast<cardinal*>(&fileArr[22]);

	texData.DataSize = texData.Height * texData.Width * 4;

	texData.Data = boost::shared_array<char>(new char [texData.DataSize]);

	cardinal pos = *reinterpret_cast<cardinal*>(&fileArr[10]);
	cardinal x = 0;

	for (cardinal i=0; i<texData.Width; i++)
		for(cardinal j=0; j<texData.Height; j++)
		{
				
			if (pos+4>fileSize)
				throw ErrorFileTooShort(filename);

			x = (i * texData.Height + j) * 4;
			texData.Data[x + 2] = fileArr[pos++];
			texData.Data[x + 1] = fileArr[pos++];
			texData.Data[x + 0] = fileArr[pos++];
            texData.Data[x + 3] = fileArr[pos++];
		}

	return true;
}

bool TTextureListClass::CreateTexDataFromTga(const std::string& filename, TTextureData& texData)
{
	
	bool result = LoadTGA(filename, texData);

	if (!result)
	{
		throw ErrorFileNotCorrect(filename);
	}

	return result;

}


bool TTextureListClass::CreateTexDataFromJpg(const std::string& filename, TTextureData& texData)
{
	return LoadJpg(filename, texData);
}


bool TTextureListClass::CreateTexDataFromPng(const std::string& filename, TTextureData& texData)
{
	cardinal fileSize;
	boost::shared_array<char> fileArr = CreateMemFromFile<char>(filename, fileSize);

	if (fileSize < 22)
		throw ErrorFileNotCorrect(filename);

	TPngDataStruct s = read_png_file(fileArr, fileSize);

	texData.Width = s.Width;
	texData.Height = s.Height;

	cardinal bytesPerPixel;

	if (s.ColorType == PNG_COLOR_TYPE_RGB)
	{
		strcpy(texData.Format, "bmp24");

		bytesPerPixel = 3;
	}
	else if (s.ColorType == PNG_COLOR_TYPE_RGBA)
	{
		strcpy(texData.Format, "bmp32");

		bytesPerPixel = 4;
	}

	texData.DataSize = texData.Height * texData.Width * bytesPerPixel;
	texData.Data = boost::shared_array<char>(new char [texData.DataSize]);


	cardinal x;

	for (cardinal i=0; i<texData.Width; i++)
	{
		for(cardinal j=0; j<texData.Height; j++)
		{

			x = (i + (texData.Height - j - 1) * texData.Width) * bytesPerPixel;
			texData.Data[x + 2] = s.RowPointers[j][bytesPerPixel*i + 2];
			texData.Data[x + 1] = s.RowPointers[j][bytesPerPixel*i + 1];
			texData.Data[x + 0] = s.RowPointers[j][bytesPerPixel*i + 0];

			if (bytesPerPixel == 4)
			{
				texData.Data[x + 3] = s.RowPointers[j][bytesPerPixel*i + 3];
			}
		}
	}

	
	for (int y=0; y<s.Height; y++)
	{
		free (s.RowPointers[y]);
	}

	free(s.RowPointers);

	png_destroy_read_struct(&s.PngPtr, &s.InfoPtr,(png_infopp)0);

	return true;


}

cardinal TTextureListClass::AddTextureBmp24Data(const TTextureData& texData)
{
	AssertIfInMainThread();

	cardinal TexID = 0;

	glGenTextures(1, &TexID);
	if (TexID == 0)
	{
		throw ErrorToLog("glGenTextures did not work");
	}

    glBindTexture(GL_TEXTURE_2D, TexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
	#ifdef TARGET_WIN32
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    #endif

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texData.Width, texData.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, &texData.Data[0]);

    return TexID;
}

cardinal TTextureListClass::AddTextureBmp32Data(const TTextureData& texData)
{
	AssertIfInMainThread();

	cardinal TexID;
	glGenTextures(1, &TexID);
	if (TexID == 0)
	{
		throw ErrorToLog("glGenTextures did not work");
	}

    glBindTexture(GL_TEXTURE_2D, TexID);

	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   
    #ifdef TARGET_WIN32
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	#endif
	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texData.Width, texData.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texData.Data[0]);

    return TexID;
}

cardinal TTextureListClass::AddCubemapTextureBmp24Data(TTextureData* texData)
{
	AssertIfInMainThread();

	cardinal TexID;
	glGenTextures(1, &TexID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TexID);
	
	#ifdef TARGET_WIN32
		
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	#endif

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, GL_RGB, texData[0].Width, texData[0].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, &(texData[0].Data[0]));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGB, texData[1].Width, texData[1].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, &(texData[1].Data[0]));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGB, texData[2].Width, texData[2].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, &(texData[2].Data[0]));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGB, texData[3].Width, texData[3].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, &(texData[3].Data[0]));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGB, texData[4].Width, texData[4].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, &(texData[4].Data[0]));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGB, texData[5].Width, texData[5].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, &(texData[5].Data[0]));

    return TexID;
}

cardinal TTextureListClass::GetTextureHeight(const std::string& texName)
{
	if (TexMap.count(texName) != 0)
			return TexMap[texName].Height;
		else
			return 0;
}

cardinal TTextureListClass::GetTextureWidth(const std::string& texName)
{
	if (TexMap.count(texName) != 0)
			return TexMap[texName].Width;
		else
			return 0;
}


cardinal TTextureListClass::AddTextureDirectly(const std::string& filename, std::string texName)
{
	cardinal TexID;

	//Basically:
	if (filename == "")
		return 0;

	if (TexMap.count(texName) == 0)
	{
		std::string texext = GetFileExt(filename);
        
        TTextureData texData;
        if (CreateFunctionMap[texext](filename, texData))
        {
            
            NormalizeTexData(texData);
            TexID = AddFunctionMap[std::string(texData.Format)](texData);
           
            *Console<<"ResourceManager::TexList Texture added: "+texName+" with id = "+tostr(TexID);
            TexMap[texName].RefCount = 1;
            TexMap[texName].TexID = TexID;
            TexMap[texName].Width = texData.Width;
            TexMap[texName].Height = texData.Height;
            
        }
        else
        {
            TexID = 0;
            *Console<<"ResourceManager::TexList ERROR - TEXTURE LOAD FAILED: "+texName;
            
        }
        
        return TexID;
        
	}
	else
	{
		++TexMap[texName].RefCount;
		*Console<<"ResourceManager::TexList Texture reference added: "+texName;
		return TexMap[texName].TexID;
	}

}

cardinal TTextureListClass::AddTexture(const std::string& fileName)
{
	return AddTexture(fileName, GetFileName(fileName));
}

cardinal TTextureListClass::AddTexture(const std::string& fileName, std::string texName)
{
	if (texName == "")
	{
		texName = GetFileName(fileName);
	}
	   
	std::string fullFileName = ST::PathToResources + fileName;

	std::string realFileName;

#ifdef TARGET_WIN32
	if (GetFileName(fileName).find('.') == std::string::npos)
	{
		realFileName = AutocompleteExtension(fullFileName); //AutocompleteExtension works for Windows only
	}
	else
	{
		realFileName = fullFileName; //AutocompleteExtension works for Windows only
	}
#else
    realFileName = fullFileName;
#endif


	return AddTextureDirectly(realFileName, texName);
}


cardinal TTextureListClass::AddTextureFromUserdata(const std::string& fileName, std::string texName)
{
	
	if (!IsFileExistsInUserData(fileName))
	{
		throw ErrorToLog("File not found in userdata: "+fileName);
	}

	std::string fullFileName = GetFilePathUserData(fileName);

	return AddTextureDirectly(fullFileName, texName);
}


cardinal TTextureListClass::AddCubemapTexture(std::string filename[6])
{

	filename[0] = ST::PathToResources + filename[0];
	filename[1] = ST::PathToResources + filename[1];
	filename[2] = ST::PathToResources + filename[2];
	filename[3] = ST::PathToResources + filename[3];
	filename[4] = ST::PathToResources + filename[4];
	filename[5] = ST::PathToResources + filename[5];

	std::string texname = GetFileName(filename[0]);
	std::string texext;
	cardinal TexID;
	int i;

	if (TexMap.count(texname) == 0)
	{
		TTextureData texData[6];
		
		for (i = 0; i < 6; i++)
		{
			texext = GetFileExt(filename[i]);
			*Console<<"Going to add texture: "+filename[i];
			if (texext == ".bmp")
			{
				if (!CreateTexDataFromBmp24(filename[i],texData[i]))
				{
					*Console<<"ResourceManager::TexList ERROR - TEXTURE LOAD FAILED: "+texname;
					return 0;
				}
			}
			else
			{
				*Console<<"ResourceManager::TexList ERROR - FORMAT NOT SUPPORTED: "+texext;
				return 0;
			}

		}

		//All textures have been inserted into texData[6], lets add them

		boost::function<cardinal()> f = boost::bind(&TTextureListClass::AddCubemapTextureBmp24Data, this, texData);

		TexID = PerformInMainThread<cardinal>(f);

		if (TexID != 0)
		{
			TexMap[texname].RefCount = 1;
			TexMap[texname].TexID = TexID;
			*Console<<"ResourceManager::TexList texture added succesfuly: "+texname;
			return TexID;
		}
		else
		{
			*Console<<"ResourceManager::TexList ERROR - TEXTURE ADD TO OPENGL FAILED: "+texname;
			return 0;
		}


	}
	else
	{
		++TexMap[texname].RefCount;
		*Console<<"ResourceManager::TexList Texture reference added: "+texname;
		return TexMap[texname].TexID;
	}

}

cardinal TTextureListClass::AddEmptyTexture(const std::string& texName, cardinal width, cardinal height)
{
	boost::function<cardinal()> f = boost::bind(&TTextureListClass::InnerAddEmptyTexture, this, texName, width, height);

	return PerformInMainThread<cardinal>(f);
}


cardinal TTextureListClass::AddEmptyCubemapTexture(const std::string& texName, cardinal width, cardinal height)
{
	boost::function<cardinal()> f = boost::bind(&TTextureListClass::InnerAddEmptyCubemapTexture, this, texName, width, height);

	return PerformInMainThread<cardinal>(f);
}


cardinal TTextureListClass::AddDepthTexture(const std::string& texName, cardinal width, cardinal height)
{
	boost::function<cardinal()> f = boost::bind(&TTextureListClass::InnerAddEmptyCubemapTexture, this, texName, width, height);

	return PerformInMainThread<cardinal>(f);
}

void TTextureListClass::DeleteTexture(const std::string& texName)
{
	if (TexMap.count(texName) != 0)
	{
		InnerDeleteTexture(TexMap.find(texName));
	}
}

void TTextureListClass::DeleteTexture(cardinal texID)
{
	TTextureMap::iterator i = TexMap.begin();

	while (i != TexMap.end())
	{
		if (i->second.TexID == texID)
		{
			InnerDeleteTexture(i);

			i = TexMap.end(); //to go out of loop
		}
		else
		{
			++i;
		}

	}

}


void TTextureListClass::PrintTextureList()
{
	TTextureMap::iterator i;

	Console->PrintImmediate("======= Texture List ==========");

	for (i = TexMap.begin(); i != TexMap.end(); ++i)
	{
		Console->PrintImmediate("ID="+tostr(i->second.TexID) + " " + i->first +" Ref="+tostr(i->second.RefCount));
	}
}


void TTextureListClass::BindFunctions()
{
	//*Console<<"Bind functions in TTextureListClass\n";
	ResourceManager->ScriptManager.AddFunction("ptl", TScriptInfo("PrintTextureList"), *this, &TTextureListClass::PrintTextureList);
}

void TTextureListClass::SaveTexDataToPlainBmpToUserData(const std::string& fileName, TTextureData texData)
{

	std::string fullFileName = GetFilePathUserData(fileName);
	FILE * pFile;
	pFile = fopen (fullFileName.c_str(), "wb");

	if (pFile == NULL)
	{
		throw ErrorToLog("pFile == NULL in TTextureListClass::SaveTexDataToPlainBmp");
	}

	char DummyBuffer[10];

	fwrite (DummyBuffer, 1, 10, pFile); //0 -> 10

	int rgbDataOffset = 26;

	fwrite (&rgbDataOffset, 4, 1, pFile); //10 -> 14

	fwrite (DummyBuffer, 1, 4, pFile); //14 -> 18

	fwrite (&(texData.Width), 4, 1, pFile); //18 -> 22
	fwrite (&(texData.Height), 4, 1, pFile); //22 -> 26

	cardinal x = 0;

	for (cardinal i = 0; i < texData.Width; i++)
	{
		for(cardinal j = 0; j < texData.Height; j++)
		{
			x = (i * texData.Height + j) + (i * texData.Height + j) + (i * texData.Height + j);

			fwrite (&(texData.Data[x + 2]), 1, 1, pFile);
			fwrite (&(texData.Data[x + 1]), 1, 1, pFile);
			fwrite (&(texData.Data[x + 0]), 1, 1, pFile);
		}
	}

	fclose (pFile);
	
}


} //namespace SE
