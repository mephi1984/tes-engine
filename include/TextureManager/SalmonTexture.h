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
#include "include/ScriptManager/ScriptManager.h"

namespace SE
{


struct TTextureInfo
{
	cardinal Width;
	cardinal Height;
	cardinal TexID;
	cardinal RefCount;
};


struct TTextureData
{
	cardinal Width;
	cardinal Height;
	char Format[8];
	cardinal DataSize;
	boost::shared_array<char> Data;
};

typedef std::map<std::string, TTextureInfo, std::less<std::string> > TTextureMap;


class TTextureListClass : public TSerializeInterface, public TFunctionBinderInterface
{
protected:
    
	TTextureMap TexMap;
    
    std::map<std::string, boost::function<bool(const std::string&, TTextureData&)>> CreateFunctionMap;
    std::map<std::string, boost::function<cardinal(TTextureData&)>> AddFunctionMap;

	void NormalizeTexData(TTextureData& texData);

	bool CreateTexDataFromBmp24(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromBmp32(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromTga(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromPng(const std::string& filename, TTextureData& texData);
	
	cardinal AddTextureBmp24Data(const TTextureData& texData); //MAIN THREAD ONLY
	cardinal AddTextureBmp32Data(const TTextureData& texData); //MAIN THREAD ONLY
	cardinal AddCubemapTextureBmp24Data(TTextureData* texData); //MAIN THREAD ONLY
	//cardinal AddCubemapTextureBmp32Data(TTextureData* texData); Not implemented yet


	cardinal InnerAddEmptyTexture(const std::string& texName, cardinal width, cardinal height);  //MAIN THREAD ONLY
	cardinal InnerAddEmptyCubemapTexture(const std::string& texName, cardinal width, cardinal height);  //MAIN THREAD ONLY
	cardinal InnerAddDepthTexture(const std::string& texName, cardinal width, cardinal height);  //MAIN THREAD ONLY
	void InnerDeleteTexture(TTextureMap::iterator itr);  //MAIN THREAD ONLY //MAIN THREAD ONLY
	

	void InnerClear(); //MAIN THREAD ONLY

public:
	TTextureListClass();
	~TTextureListClass();

	void Clear();

	virtual void Serialize(boost::property_tree::ptree& propertyTree);

	cardinal operator[](const std::string& s)
	{
		if (TexMap.count(s) != 0)
			return TexMap[s].TexID;
		else
			return 0;
	}

	cardinal GetTextureHeight(const std::string& texName);
	cardinal GetTextureWidth(const std::string& texName);

	cardinal AddTextureDirectly(const std::string& filename, std::string texName = ""); //Loads texture directly from this file or fails
	cardinal AddTexture(const std::string& fileName);
	cardinal AddTexture(const std::string& fileName, std::string texName); //Adds path to resources to the filename then call previous one
	cardinal AddTextureFromUserdata(const std::string& fileName, std::string texName = ""); //Same as above but checks if file is created in user data

	cardinal AddCubemapTexture(std::string filename[6]); // "posx.bmp","negx.bmp","posy.bmp","negy.bmp","posz.bmp","negz.bmp"


	cardinal AddEmptyTexture(const std::string& texName, cardinal width, cardinal height);
	cardinal AddEmptyCubemapTexture(const std::string& texName, cardinal width, cardinal height);
	cardinal AddDepthTexture(const std::string& texName, cardinal width, cardinal height);
	void DeleteTexture(const std::string& texName);
	void DeleteTexture(cardinal texID);

	void PrintTextureList();

	virtual void BindFunctions();

	void SaveTexDataToPlainBmpToUserData(const std::string& fileName, TTextureData texData);
};

} //namespace SE
