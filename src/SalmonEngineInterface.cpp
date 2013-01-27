#include "include/SalmonEngineInterface.h"
namespace SE
{

#ifdef TARGET_ANDROID
TJavaConsole* Console;
TSalmonRendererAndroid* Renderer;
#endif


#ifdef TARGET_WIN32
TFileConsole* Console;
TSalmonRenderer* Renderer;
#endif

#ifdef TARGET_IOS
TIosConsole* Console;
TSalmonRendererIos* Renderer;
#endif

TResourceManager* ResourceManager;


TApplicationAncestor::TApplicationAncestor()
{

}

void TApplicationAncestor::OuterDeinit()
{
    InnerDeinit();
}


void TApplicationAncestor::OuterDraw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	RenderUniform1i("Texture", 0);
	RenderUniform1f("Transparency", 1.f);
	
	InnerDraw();

	glDisable(GL_DEPTH_TEST);

	Renderer->SetOrthoProjection();
	Renderer->LoadIdentity();

	ResourceManager->GUIManager.Draw();

	Renderer->PushShader("DefaultShader");
	
	if (IsConsoleOut) 
	{
		Console->Draw();
	}
	
	Renderer->PopShader();
	glEnable(GL_DEPTH_TEST);
}

void TApplicationAncestor::OuterUpdate(cardinal timer)
{
	ResourceManager->SoundManager.Update(timer);

	ResourceManager->GUIManager.Update(timer);

	InnerUpdate(timer);

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

void TApplicationAncestor::OuterOnTapUpAfterShift(vec2 p)
{
	InnerOnTapUpAfterShift(p);
}

void TApplicationAncestor::OuterOnMove(vec2 shift)
{
	ResourceManager->GUIManager.OnMove(shift);
	InnerOnMove(shift);
}

} //namespace SE