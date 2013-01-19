#ifndef HALIBUT_ENGINE_INTERFACE_H_INCLUDED
#define HALIBUT_ENGINE_INTERFACE_H_INCLUDED

#include "include/ApplicationInterface.h"

#include "include/TextureManager/SalmonTexture.h"
#include "include/ShaderManager/ShaderManager.h"
#include "include/ScriptManager/ScriptManager.h"
#include "include/GUIManager/GUIManager.h"
#include "include/FrameManager/FrameManager.h"
#include "include/FontManager/FontManager.h"
#include "include/HalibutAnimation/HalibutAnimation.h"
#include "include/SmartValueManager/SmartValueManager.h"



#ifdef TARGET_WIN32
#include "include/SoundManager/SoundManagerWindows.h"
#endif
#ifdef TARGET_ANDROID
#include "include/SoundManager/SoundManagerAndroid.h"
#endif
#ifdef TARGET_IOS
#include "include/SoundManager/SoundManagerIos.h"
#endif

namespace SE
{

//Windows only
struct TMouseState
{
	int X;
	int Y;
	bool LeftButtonPressed;
	bool MiddleButtonPressed;
	bool RightButtonPressed;
};




class THalibutResourceManager
{
public:
	std::string PathToResources;

	TTextureListClass TexList;
	TShaderManager ShaderManager;
    TScriptManager ScriptManager;
    TGUIManager GUIManager;
	TFrameManager FrameManager;
	TFontManager FontManager;
	THalibutAnimationManager HalibutAnimationManager;
	TSmartValueManager SmartValueManager;

	#ifdef TARGET_WIN32
	TSoundManagerWindows SoundManager;
	#endif
	#ifdef TARGET_ANDROID
	TSoundManagerAndroid SoundManager;
	#endif
    #ifdef TARGET_IOS
	TSoundManagerIos SoundManager;
    #endif
};


class TApplicationAncestor : public TApplicationInterface
{
protected:
	
public:

	TApplicationAncestor();

	//If you override them, call ancestor:
	virtual void OuterDraw();
	virtual void OuterUpdate(cardinal dt);


	virtual void OuterOnTapDown(vec2 p);
	virtual void OuterOnTapUp(vec2 p);
	virtual void OuterOnMove(vec2 shift);
	
	virtual void InnerOnTapDown(vec2 p) { }
	
	virtual void InnerOnTapUp(vec2 p) { }
	
	virtual void InnerOnMove(vec2 shift) { }
	
	virtual void OnFling(vec2 v) { } //Android only

	virtual void OnMouseMove(TMouseState& mouseState) { } //Windows only

	virtual void OnMouseWheel(short int delta) { } //Windows only

	virtual void OnKeyPress(cardinal key) { } //Try not to override this. But if you need to override, call ancestor!

};

} //namespace SE

#endif