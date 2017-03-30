#pragma once

/*
This code contains texture manager
*/

#ifdef TARGET_WIN32
	#include "include/OpenGlExt/OpenGlExt.h"
#endif

#ifdef TARGET_ANDROID
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif


#include "include/Utils/Utils.h"
#include "include/Utils/PngHelper.h"
#include "include/Utils/JpegHelper.h"
#include "include/Utils/TgaLoader.h"

namespace SE
{


struct TTextureInfo
{
	size_t Width;
	size_t Height;
	GLuint TexID;
	size_t RefCount;
};


struct TTextureData
{
	size_t Width;
	size_t Height;
	char Format[8];
	size_t DataSize;
	boost::shared_array<char> Data;
};

typedef std::map<std::string, TTextureInfo, std::less<std::string>> TTextureMap;


class TTextureListClass : public TSerializeInterface
{
protected:
    
	TTextureMap TexMap;
    
    std::map<std::string, boost::function<bool(const std::string&, TTextureData&)>> CreateFunctionMap;
    std::map<std::string, boost::function<size_t(TTextureData&)>> AddFunctionMap;

	void NormalizeTexData(TTextureData& texData);

	bool CreateTexDataFromBmp24(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromBmp32(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromTga(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromPng(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromJpg(const std::string& filename, TTextureData& texData);
	
	size_t AddTextureBmp24Data(const TTextureData& texData); //MAIN THREAD ONLY
	size_t AddTextureBmp32Data(const TTextureData& texData); //MAIN THREAD ONLY
	size_t AddCubemapTextureBmp24Data(TTextureData* texData); //MAIN THREAD ONLY
	//size_t AddCubemapTextureBmp32Data(TTextureData* texData); Not implemented yet


	size_t InnerAddEmptyTexture(const std::string& texName, size_t width, size_t height);  //MAIN THREAD ONLY
	size_t InnerAddEmptyCubemapTexture(const std::string& texName, size_t width, size_t height);  //MAIN THREAD ONLY
	size_t InnerAddDepthTexture(const std::string& texName, size_t width, size_t height);  //MAIN THREAD ONLY
	void InnerDeleteTexture(TTextureMap::iterator itr);  //MAIN THREAD ONLY //MAIN THREAD ONLY
	

	void InnerClear(); //MAIN THREAD ONLY

public:
	TTextureListClass();
	~TTextureListClass();

	void Clear();

	virtual void Serialize(boost::property_tree::ptree& propertyTree);

	size_t operator[](const std::string& s)
	{
		if (TexMap.count(s) != 0)
			return TexMap[s].TexID;
		else
			return 0;
	}

	size_t GetTextureHeight(const std::string& texName);
	size_t GetTextureWidth(const std::string& texName);

	size_t AddTextureDirectly(const std::string& filename, std::string texName = ""); //Loads texture directly from this file or fails
	size_t AddTexture(const std::string& fileName);
	size_t AddTexture(const std::string& fileName, std::string texName); //Adds path to resources to the filename then call previous one
	size_t AddTextureFromUserdata(const std::string& fileName, std::string texName = ""); //Same as above but checks if file is created in user data

	size_t AddCubemapTexture(std::string filename[6]); // "posx.bmp","negx.bmp","posy.bmp","negy.bmp","posz.bmp","negz.bmp"


	size_t AddEmptyTexture(const std::string& texName, size_t width, size_t height);
	size_t AddEmptyCubemapTexture(const std::string& texName, size_t width, size_t height);
	size_t AddDepthTexture(const std::string& texName, size_t width, size_t height);
	void DeleteTexture(const std::string& texName);
	void DeleteTexture(size_t texID);

	void PrintTextureList();

	virtual void BindFunctions();

	void SaveTexDataToPlainBmpToUserData(const std::string& fileName, TTextureData texData);
};

} //namespace SE
