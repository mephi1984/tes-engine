#ifndef SALMON_ENGINE_INTERFACE_H_INCLUDED
#define SALMON_ENGINE_INTERFACE_H_INCLUDED

/*
This code combines all headers for Salmon engine into one header file
*/

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
#include "include/PhysicsManager/PhysicsManager.h"
#include "include/GUIManager/GUIManager.h"
#include "include/HalibutAnimation/HalibutAnimation.h"

namespace SE
{

class TResourceManager
{
protected:
public:
	std::string PathToResources;

	boost::thread::id MainThreadId;

	//std::vector<boost::function<void()>> MainThreadAsyncFunctionArr;

	//boost::mutex FuncListMutex;
	//std::list<TFuncToPerform> MainThreadSyncFunctionList;

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
	TGUIManager GUIManager;
	THalibutAnimationManager HalibutAnimationManager;

#ifdef TARGET_ANDROID
	TSoundManagerAndroid SoundManager;
#endif
#ifdef TARGET_WIN32
	TSoundManagerWindows SoundManager;
#endif
#ifdef TARGET_IOS
	TSoundManagerIos SoundManager;
#endif

	void Update(cardinal timer);

	~TResourceManager();
};


struct TMouseState
{
	int X;
	int Y;
	bool LeftButtonPressed;
	bool MiddleButtonPressed;
	bool RightButtonPressed;
};



class TApplicationAncestor
{
protected:
	bool IsConsoleOut;
public:

	TApplicationAncestor();

	virtual ~TApplicationAncestor();

	virtual void OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);

	
	virtual void OuterDeinit();


	virtual void OuterDraw();
	//What to draw

	virtual void OuterUpdate(cardinal timer);
	//To do on update

	virtual void UpdateQuick() { }
	//To process input - this method is called more frequently than Update()  

	virtual void OuterOnTapDown(vec2 p);

	virtual void OuterOnTapUp(vec2 p);

	virtual void OuterOnTapUpAfterMove(vec2 p);

	virtual void OuterOnMove(vec2 shift);


	virtual void InnerInit() { }

	virtual void InnerDeinit() { }

	virtual void InnerDraw() { }
	
	virtual void InnerUpdate(cardinal timer) { }
	


	virtual void InnerOnTapDown(vec2 p) { }
	
	virtual void InnerOnTapUp(vec2 p) { }

	virtual void InnerOnTapUpAfterMove(vec2 p) { }
	
	virtual void InnerOnMove(vec2 shift) { }



	virtual void OnMouseMove(TMouseState& mouseState) { } //Windows only

	virtual void OnMouseWheel(short int delta) { }
	//To do on mouse wheel move

	virtual void OnKeyPress(cardinal key) { } //Try not to override this. But if you need to override, call ancestor!
    
    virtual void OnScale(float scale) { }
};


} //namespace SE

#endif

