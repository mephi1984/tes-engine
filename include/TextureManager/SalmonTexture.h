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


#ifdef TARGET_WINDOWS_UNIVERSAL
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//#include "glext.h"
//#include "include/OpenGlExt/OpenGlExt.h"
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
    std::map<std::string, boost::function<GLuint(TTextureData&)>> AddFunctionMap;

	void NormalizeTexData(TTextureData& texData);

	bool CreateTexDataFromBmp24(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromBmp32(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromTga(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromPng(const std::string& filename, TTextureData& texData);
	bool CreateTexDataFromJpg(const std::string& filename, TTextureData& texData);
	
	GLuint AddTextureBmp24Data(const TTextureData& texData); //MAIN THREAD ONLY
	GLuint AddTextureBmp32Data(const TTextureData& texData); //MAIN THREAD ONLY
	GLuint AddCubemapTextureBmp24Data(TTextureData* texData); //MAIN THREAD ONLY
	//size_t AddCubemapTextureBmp32Data(TTextureData* texData); Not implemented yet


	GLuint InnerAddEmptyTexture(const std::string& texName, size_t width, size_t height);  //MAIN THREAD ONLY
	GLuint InnerAddEmptyCubemapTexture(const std::string& texName, size_t width, size_t height);  //MAIN THREAD ONLY
	GLuint InnerAddDepthTexture(const std::string& texName, size_t width, size_t height);  //MAIN THREAD ONLY
	void InnerDeleteTexture(TTextureMap::iterator itr);  //MAIN THREAD ONLY //MAIN THREAD ONLY
	

	void InnerClear(); //MAIN THREAD ONLY

public:
	TTextureListClass();
	~TTextureListClass();

	void Clear();

	virtual void Serialize(boost::property_tree::ptree& propertyTree);

	GLuint operator[](const std::string& s)
	{
		if (TexMap.find(s) != TexMap.end())
			return TexMap[s].TexID;
		else
			return 0;
	}

	size_t GetTextureHeight(const std::string& texName);
	size_t GetTextureWidth(const std::string& texName);

	GLuint AddTextureDirectly(const std::string& filename, std::string texName = ""); //Loads texture directly from this file or fails
	GLuint AddTexture(const std::string& fileName);
	GLuint AddTexture(const std::string& fileName, std::string texName); //Adds path to resources to the filename then call previous one
	GLuint AddTextureFromUserdata(const std::string& fileName, std::string texName = ""); //Same as above but checks if file is created in user data

	GLuint AddCubemapTexture(std::string filename[6]); // "posx.bmp","negx.bmp","posy.bmp","negy.bmp","posz.bmp","negz.bmp"


	GLuint AddEmptyTexture(const std::string& texName, size_t width, size_t height);
	GLuint AddEmptyCubemapTexture(const std::string& texName, size_t width, size_t height);
	GLuint AddDepthTexture(const std::string& texName, size_t width, size_t height);
	void DeleteTexture(const std::string& texName);
	void DeleteTexture(GLuint texID);

	void PrintTextureList();

	virtual void BindFunctions();

	void SaveTexDataToPlainBmpToUserData(const std::string& fileName, TTextureData texData);
};

} //namespace SE
