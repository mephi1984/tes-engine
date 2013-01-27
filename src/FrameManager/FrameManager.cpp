#include "include/FrameManager/FrameManager.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

//===================================================
//================ FRAME MANAGER ====================
//===================================================


TFrameManager::~TFrameManager()
{
	FreeFrameManager();
	*Console<<"ResourceManager::FrameManager deleting"; 
}

void TFrameManager::FreeFrameManager()
{
    BOOST_FOREACH(auto& i, FrameMap)
	{
		ResourceManager->TexList.DeleteTexture(i.second.TexName);
		glDeleteRenderbuffers(1, &(i.second.DepthRenderBuffer));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &(i.second.FrameBuffer));
	}

	FrameMap.clear();

}

cardinal TFrameManager::AddFrameRenderBuffer(const std::string& frameName,cardinal width,cardinal height)
{
	#ifdef TARGET_ANDROID
	#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE
	#endif
    #ifdef TARGET_IOS
    #define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE
    #endif
	
	if (FrameMap.count(frameName) == 0)
	{

		
		
		std::string texName = frameName + "Texture";
		cardinal texID = ResourceManager->TexList.AddEmptyTexture(texName,width,height);
	
		
		cardinal frameBuffer;
		cardinal depthRenderBuffer;
		cardinal status;


		glGenFramebuffers(1,&frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		
		
		glGenRenderbuffers(1, &depthRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
		
		#ifdef TARGET_WIN32
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		#endif
		#ifdef TARGET_ANDROID
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		#endif
        #ifdef TARGET_IOS
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        #endif
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
		
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		
		switch(status)
		{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			*Console<<"FrameManager:: frame buffer added: "+frameName;
			FrameMap[frameName].FrameBuffer = frameBuffer;
			FrameMap[frameName].DepthRenderBuffer = depthRenderBuffer;
			FrameMap[frameName].TexID = texID;
			FrameMap[frameName].Width = width;
			FrameMap[frameName].Height = height;
			FrameMap[frameName].TexName = texName;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			return FrameMap[frameName].FrameBuffer;

			break;
		default:
			*Console<<"FrameManager::ERROR - frame buffer can not be added correctly: "+frameName+", with error: "+tostr(status);
			ResourceManager->TexList.DeleteTexture(texName);
			glDeleteRenderbuffers(1, &depthRenderBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &frameBuffer);

			return 0;
		}
		
	}
	else
		return FrameMap[frameName].FrameBuffer;

	#ifdef TARGET_ANDROID
	#undef GL_FRAMEBUFFER_COMPLETE_EXT
	#endif
    #ifdef TARGET_IOS
    #undef GL_FRAMEBUFFER_COMPLETE_EXT
    #endif
    
}

cardinal TFrameManager::AddCubemapBuffer(const std::string& frameName,cardinal width,cardinal height)
{

	#ifdef TARGET_ANDROID
	#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE
	#endif
    #ifdef TARGET_IOS  
    #define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE
    #endif
	
	if (FrameMap.count(frameName) == 0)
	{

		std::string texName = frameName + "Texture";
		cardinal texID = ResourceManager->TexList.AddEmptyCubemapTexture(texName,width,height);

		cardinal frameBuffer;
		cardinal depthRenderBuffer;
		cardinal status;


		glGenFramebuffers(1,&frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glGenRenderbuffers(1, &depthRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
		
		#ifdef TARGET_WIN32
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		#endif
		#ifdef TARGET_ANDROID
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		#endif
        #ifdef TARGET_IOS
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        #endif
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texID, 0);

		//?? is it needed?
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, texID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, texID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, texID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, texID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, texID, 0);




		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		switch(status)
		{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			*Console<<"FrameManager:: frame buffer added: "+frameName;
			FrameMap[frameName].FrameBuffer = frameBuffer;
			FrameMap[frameName].DepthRenderBuffer = depthRenderBuffer;
			FrameMap[frameName].TexID = texID;
			FrameMap[frameName].Width = width;
			FrameMap[frameName].Height = height;
			FrameMap[frameName].TexName = texName;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return FrameMap[frameName].FrameBuffer;

			break;
		default:
			*Console<<"ERROR number : "+tostr(status)+endl;
			*Console<<"FrameManager::ERROR - frame buffer can not be added correctly: "+frameName;
			ResourceManager->TexList.DeleteTexture(texName);
			glDeleteRenderbuffers(1, &depthRenderBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &frameBuffer);

			return 0;
		}


	}
	else
		return FrameMap[frameName].FrameBuffer;
		
	#ifdef TARGET_ANDROID
	#undef GL_FRAMEBUFFER_COMPLETE_EXT
	#endif
    #ifdef TARGET_IOS
    #undef GL_FRAMEBUFFER_COMPLETE_EXT
    #endif

}

void TFrameManager::DeleteFrameRenderBuffer(const std::string& frameName)
{
	if (FrameMap.count(frameName) > 0)
	{
		ResourceManager->TexList.DeleteTexture(FrameMap[frameName].TexName);
		glDeleteRenderbuffers(1, &(FrameMap[frameName].DepthRenderBuffer));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &(FrameMap[frameName].FrameBuffer));

		FrameMap.erase(frameName);
	}
}

cardinal TFrameManager::AddDepthBuffer(const std::string& frameName, cardinal width, cardinal height)
{

	#ifdef TARGET_ANDROID
	#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE
	#endif
	
	if (FrameMap.count(frameName) == 0)
	{

		std::string texName = frameName + "Texture";
		cardinal depthTexID = ResourceManager->TexList.AddDepthTexture(texName,width,height);

		cardinal frameBuffer;
		cardinal status;


		glGenFramebuffers(1,&frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexID, 0);
		
		#ifdef TARGET_WIN32
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		#endif
		
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		switch(status)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			*Console<<"FrameManager:: frame buffer added: "+frameName;
			FrameMap[frameName].FrameBuffer = frameBuffer;
			FrameMap[frameName].DepthRenderBuffer = 0;
			FrameMap[frameName].TexID = depthTexID;
			FrameMap[frameName].Width = width;
			FrameMap[frameName].Height = height;
			FrameMap[frameName].TexName = texName;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return FrameMap[frameName].FrameBuffer;
			break;
		default:
			*Console<<"FrameManager::ERROR - frame buffer can not be added correctly: "+frameName;
			ResourceManager->TexList.DeleteTexture(texName);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &frameBuffer);

			return 0;
		}


	}
	else
		return FrameMap[frameName].FrameBuffer;
		
	#ifdef TARGET_ANDROID
	#undef GL_FRAMEBUFFER_COMPLETE_EXT
	#endif
}



cardinal TFrameManager::GetFrameTexture(const std::string& frameName)
{
	if (FrameMap.count(frameName) > 0)
	{
		return FrameMap[frameName].TexID;
	}
	else
		return 0;
}


ivec2 TFrameManager::GetFrameWidthHeight(const std::string& frameName)
{
	if (FrameMap.count(frameName) == 0)
	{
		throw ErrorToLog("Frame "+frameName+" not found!");
	}

	return ivec2(FrameMap[frameName].Width, FrameMap[frameName].Height);
}


TTextureData TFrameManager::ReadFromBufferToTexData(const std::string& frameName)
{
	TTextureData texData;
	texData.Width = FrameMap[frameName].Width;
	texData.Height = FrameMap[frameName].Height;
	texData.DataSize = texData.Width * texData.Height * 3;

	texData.Data = boost::shared_array<char>(new char[texData.DataSize]);

	boost::shared_array<char> data(new char[texData.Width * texData.Height * 4]);

	glReadPixels(0, 0, FrameMap[frameName].Width, FrameMap[frameName].Height, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

	for (cardinal i=0; i < texData.Width; i++)
	{
		for (cardinal j=0; j < texData.Height; j++)
		{
			int x = (i * texData.Height + j) + (i * texData.Height + j) + (i * texData.Height + j);

			int y = 4 * (i * texData.Height + j);

			texData.Data[x + 2] = data[y + 2];
			texData.Data[x + 1] = data[y + 1];
			texData.Data[x + 0] = data[y + 0];
		}
	}

	return texData;

}

} //namespace SE