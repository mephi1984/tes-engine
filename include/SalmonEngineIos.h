#ifndef SALMON_ENGINE_ANDROID_H_INCLUDED
#define SALMON_ENGINE_ANDROID_H_INCLUDED
/*
This code combines all headers for Salmon engine into one header file
*/

#include "include/SalmonEngineInterface.h"

namespace SE
{

extern TIosConsole* Console;
extern TSalmonRendererIos* Renderer;
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

	virtual void OuterDraw();

	virtual void InnerDraw() = 0;
	//What to draw

	virtual void OuterUpdate(cardinal timer);

	virtual void InnerUpdate(cardinal timer) = 0;
	//To do on update

	virtual void UpdateQuick() { };
	//To process input - this method is called more frequently than Update()  

	virtual void OnMouseMove(TMouseState& mouseState) { }
	//To do on mouse move (with or without pressed buttons)

	virtual void OnMouseDown(TMouseState& mouseState) { }
	//To do on mouse up (with or without pressed buttons)

	virtual void OnMouseUp(TMouseState& mouseState) { }
	//To do on mouse down (with or without pressed buttons)
	//Be careful - even when user "upped" button, in mouseState this button appears as pressed

	virtual void OnMouseWheel(short int delta) { }
	//To do on mouse wheel move
};

} //namespace SE


#include "include/GUIManager/WidgetTemplatesImpl.h"

#endif
