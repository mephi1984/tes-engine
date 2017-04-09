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
#ifdef TARGET_WINDOWS_UNIVERSAL
#include "include/Render/SalmonRender/SalmonRenderWindowsUniversal.h"
#endif
#ifdef TARGET_IOS
#include "include/Render/SalmonRender/SalmonRenderIos.h"
#endif


#include "include/Render/RenderMisc.h"

#include "include/TextureManager/SalmonTexture.h"
#include "include/ShaderManager/ShaderManager.h"
#include "include/FrameManager/FrameManager.h"
#include "include/LightManager/LightManager.h"

#include "include/FontManager/FontManager.h"
#include "include/SmartValueManager/SmartValueManager.h"
#include "include/GUIManager/GUIManager.h"
#include "include/GUIManager/NewGuiManager.h"


namespace SE
{

class TResourceManager
{
protected:
public:
	TTextureListClass TexList;
	TShaderManager ShaderManager;
	TFrameManager FrameManager;
	TFontManager FontManager;
    TSmartValueManager SmartValueManager;
	TGUIManager GUIManager;

	NewGuiManager newGuiManager;

	void Update(size_t timer);

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

	virtual void OuterUpdate(size_t timer);
	//To do on update

	virtual void UpdateQuick() { }
	//To process input - this method is called more frequently than Update()  

	virtual void OuterOnTapDown(Vector2f p, int touchNumber);

	virtual void OuterOnTapUp(Vector2f p, int touchNumber);

	virtual void OuterOnTapUpAfterMove(Vector2f p, int touchNumber);

	virtual void OuterOnMove(Vector2f pos, Vector2f shift, int touchNumber);


	virtual void InnerInit() { }

	virtual void InnerDeinit() { }

	virtual void InnerDraw() { }
	
	virtual void InnerUpdate(size_t timer) { }
	


	virtual void InnerOnTapDown(Vector2f p) { }
	
	virtual void InnerOnTapUp(Vector2f p) { }

	virtual void InnerOnTapUpAfterMove(Vector2f p) { }
	
	virtual void InnerOnMove(Vector2f pos, Vector2f shift) { }



	virtual void OnMouseMove(TMouseState& mouseState); //Windows only

	virtual void InnerOnMouseMove(TMouseState& mouseState) { } //Windows only

	virtual void OnMouseWheel(short int delta) { }
	//To do on mouse wheel move

	virtual void OnKeyPress(size_t key) { } //Try not to override this. But if you need to override, call ancestor!
    
    virtual void OnScale(float scale) { }
};


} //namespace SE

#endif

