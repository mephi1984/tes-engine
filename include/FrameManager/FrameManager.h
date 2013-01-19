#ifndef FRAME_MANAGER_H_INCLUDED
#define FRAME_MANAGER_H_INCLUDED

/*
This code contains frame manager to do posteffects

Use global variable ResourceManager to get access to light manager

*/

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#ifdef TARGET_WIN32
#include "include/OpenGlExt/OpenGlExt.h"
#endif

#include "include/Utils/Utils.h"
#include "include/TextureManager/SalmonTexture.h"

namespace SE
{

struct TFrame
{
	cardinal FrameBuffer;
	cardinal DepthRenderBuffer;
	cardinal TexID;
	cardinal Width;
	cardinal Height;
	std::string TexName;
};

typedef std::map<std::string, TFrame> TFrameMap;

class TRendererInterface;
class THalibutRendererInterface;
class THalibutRenderer;
class THalibutRendererAndroid;
class THalibutRendererIos;
class TSalmonRendererInterface;
class TSalmonRenderer;
class TSalmonRendererAndroid;
class TSalmonRendererIos;

class TFrameManager
{
protected:
	TFrameMap FrameMap;
public:
	TFrameManager() {}
	
	~TFrameManager();

	void FreeFrameManager();
	//Clean up frame map

	cardinal AddFrameRenderBuffer(const std::string& frameName, cardinal width, cardinal height);
	//Add a new render buffer with name frameName. Width and height must be power of two (512, 1024 etc) 
	
	void DeleteFrameRenderBuffer(const std::string& frameName);
	//Delete render / depth buffer with name frameName 

	cardinal AddDepthBuffer(const std::string& frameName, cardinal width, cardinal height);
	//Add depth buffer with name frameName Width and height must be power of two (512, 1024 etc)

	cardinal AddCubemapBuffer(const std::string& frameName, cardinal width, cardinal height);
	//Add cubemap render buffer with name frameName Width and height must be power of two (512, 1024 etc)

	cardinal GetFrameTexture(const std::string& frameName);
	//Get texture ID from frame with name frameName

	ivec2 GetFrameWidthHeight(const std::string& frameName);

	TTextureData ReadFromBufferToTexData(const std::string& frameName);
	
	friend class TRendererInterface;
	friend class THalibutRendererInterface;
	friend class THalibutRenderer;
	friend class THalibutRendererAndroid;
	friend class THalibutRendererIos;
	friend class TSalmonRendererInterface;
	friend class TSalmonRenderer;
	friend class TSalmonRendererAndroid;
	friend class TSalmonRendererIos;

};

} //namespace SE

#endif
