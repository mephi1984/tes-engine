#ifndef SALMON_ENGINE_INTERFACE_H_INCLUDED
#define SALMON_ENGINE_INTERFACE_H_INCLUDED

/*
This code combines all headers for Salmon engine into one header file
*/


#include "include/ApplicationInterface.h"

#include "include/Render/SalmonRender/SalmonRenderInterface.h"

#ifdef TARGET_ANDROID
#include "include/Render/SalmonRender/SalmonRenderAndroid.h"
#endif
#ifdef TARGET_WIN32
#include "include/Render/SalmonRender/SalmonRenderWindows.h"
#endif
#ifdef TARGET_IOS
#include "include/Render/SalmonRender/SalmonRenderIos.h"
#endif


#include "include/Render/RenderMisc.h"

#include "include/Animation/SalmonAnimation.h"
#include "include/TextureManager/SalmonTexture.h"
#include "include/ShaderManager/ShaderManager.h"
#include "include/FrameManager/FrameManager.h"
#include "include/LightManager/LightManager.h"
#include "include/ScriptManager/ScriptManager.h"
#ifdef TARGET_ANDROID
#include "include/SoundManager/SoundManagerAndroid.h"
#endif
#ifdef TARGET_WIN32
#include "include/SoundManager/SoundManagerWindows.h"
#endif
#ifdef TARGET_IOS
#include "include/SoundManager/SoundManagerIos.h"
#endif
#include "include/FontManager/FontManager.h"
#include "include/SimpleLand/SimpleLand.h"
#include "include/SmartValueManager/SmartValueManager.h"

#include "include/ModelManager/NewModelManager.h"


namespace SE
{

class TResourceManager
{
protected:
public:
	std::string PathToResources;

	TTextureListClass TexList;
	TModelManager ModelManager;
	TFlexModelManager FlexModelManager;
	TShaderManager ShaderManager;
	TFrameManager FrameManager;
	TLightManager LightManager;
	TModelAnimManager ModelAnimManager;
	TScriptManager ScriptManager;
	TFontManager FontManager;
    TSmartValueManager SmartValueManager;

#ifdef TARGET_ANDROID
	TSoundManagerAndroid SoundManager;
#endif
#ifdef TARGET_WIN32
	TSoundManagerWindows SoundManager;
#endif
#ifdef TARGET_IOS
	TSoundManagerIos SoundManager;
#endif

	~TResourceManager() { }
};


struct TMouseState
{
	int X;
	int Y;
	bool LeftButtonPressed;
	bool MiddleButtonPressed;
	bool RightButtonPressed;
};



class TApplicationAncestor : public TApplicationInterface
{
protected:
	
public:

	TApplicationAncestor();


	virtual void OuterDeinit();
	//To do on deinit

	virtual void OuterDraw();
	//What to draw

	virtual void OuterUpdate(cardinal timer);
	//To do on update

	virtual void UpdateQuick() { }
	//To process input - this method is called more frequently than Update()  

	virtual void OuterOnTapDown(vec2 p);

	virtual void OuterOnTapUp(vec2 p);

	virtual void OuterOnMove(vec2 shift);
	
	virtual void InnerOnTapDown(vec2 p) { }
	
	virtual void InnerOnTapUp(vec2 p) { }
	
	virtual void InnerOnMove(vec2 shift) { }

	virtual void OnMouseMove(TMouseState& mouseState) { } //Windows only

	virtual void OnMouseWheel(short int delta) { }
	//To do on mouse wheel move

	virtual void OnKeyPress(cardinal key) { } //Try not to override this. But if you need to override, call ancestor!
};


} //namespace SE

#endif

