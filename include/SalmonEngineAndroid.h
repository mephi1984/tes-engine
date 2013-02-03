#ifndef SALMON_ENGINE_ANDROID_H_INCLUDED
#define SALMON_ENGINE_ANDROID_H_INCLUDED
/*
This code combines all headers for Salmon engine into one header file
*/

#include "include/SalmonEngineInterface.h"

namespace SE
{

extern TJavaConsole* Console;
extern TSalmonRendererAndroid* Renderer;
extern TResourceManager* ResourceManager;

class TApplication : public TApplicationAncestor
{
protected:
public:
	int X, Y, Width, Height; //Window position and size

	TApplication() : X(0), Y(0), Width(4), Height(4) { }

	virtual void OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);

	virtual void InnerInit() = 0;
	//To do on init

	virtual void OuterDeinit();

	virtual void InnerDeinit() = 0;
	//To do on deinit

	virtual void InnerDraw() = 0;
	//What to draw

	virtual void InnerUpdate(cardinal timer) = 0;
	//To do on update
	
};

} //namespace SE

#include "include/GUIManager/WidgetTemplatesImpl.h"
#include "include/Utils/ThreadUtilsImpl.h"

#endif
