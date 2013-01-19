#ifndef HALIBUT_ENGINE_ANDROID_H_INCLUDED
#define HALIBUT_ENGINE_ANDROID_H_INCLUDED


#include "include/Utils/Utils.h"
#include "include/Utils/Console/console.h"

#include "include/Render/HalibutRender/HalibutRenderAndroid.h"

#include "include/HalibutEngineInterface.h"
//Android code only


namespace SE
{

extern TJavaConsole* Console;

extern THalibutResourceManager* ResourceManager;

extern THalibutRendererAndroid* Renderer;

class TApplication : public TApplicationAncestor
{
protected:
public:
	TApplication();

	virtual ~TApplication();
	
	virtual void OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);
	
	virtual void OuterDeinit();
	
	virtual void OnKeyPress(cardinal key);

};


} //namespace SE

//This file includes templates that call any of three singletones: Console, ResourceManager or Renderer

#include "include/GUIManager/WidgetTemplatesImpl.h"



#endif
