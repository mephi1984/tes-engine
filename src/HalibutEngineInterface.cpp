#include "include/HalibutEngineInterface.h"
#include "include/Engine.h"


namespace SE
{

TApplicationAncestor::TApplicationAncestor() 
{
}


void TApplicationAncestor::OuterDraw() 
{   

    CheckGlError("THalibutApplicationInterface::OuterDraw 1");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	RenderUniform1i("Texture", 0);
	RenderUniform1f("Transparency", 1.f);

	InnerDraw();

	CheckGlError("THalibutApplicationInterface::OuterDraw 1.5");

	ResourceManager->GUIManager.Draw();

	if (IsConsoleOut) 
	{
		Console->Draw();
	}
    CheckGlError("THalibutApplicationInterface::OuterDraw 2");

}



void TApplicationAncestor::OuterUpdate(cardinal dt)
{
    CheckGlError();
	ResourceManager->GUIManager.Update(dt);
	ResourceManager->HalibutAnimationManager.Update(dt);

    
#ifndef TARGET_IOS
	ResourceManager->SoundManager.Update(dt);
#endif
    CheckGlError();
	InnerUpdate(dt);

	CheckGlError();
}


void TApplicationAncestor::OuterOnTapDown(vec2 p)
{
	ResourceManager->GUIManager.OnMouseDown(p);
	InnerOnTapDown(p);
}

void TApplicationAncestor::OuterOnTapUp(vec2 p)
{
	ResourceManager->GUIManager.OnMouseUp(p);
	InnerOnTapUp(p);
}

void TApplicationAncestor::OuterOnMove(vec2 shift)
{
	ResourceManager->GUIManager.OnMove(shift);
	InnerOnMove(shift);
}


} //namespace SE