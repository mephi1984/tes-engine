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



void TResourceManager::Update(cardinal timer)
{

	SoundManager.Update(timer);

	GUIManager.Update(timer);
    
    ST::MainThreadIoService.run();
	
    ST::MainThreadIoService.reset();
}

TResourceManager::~TResourceManager()
{

}

//==================================================
//============ TApplicationAncestor ================
//==================================================

TApplicationAncestor::TApplicationAncestor()
	: IsConsoleOut(false) 
{
}

TApplicationAncestor::~TApplicationAncestor()
{
}

void TApplicationAncestor::OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{
	ST::MainThreadId = boost::this_thread::get_id();

	ResourceManager->ScriptManager.BindBasicFunctions();

	Renderer->InitOpenGL(screenWidth, screenHeight, matrixWidth, matrixHeight);

	InnerInit();
    
    CheckGlError();
    
    srand(static_cast<unsigned int>(time(0)));
}

	
void TApplicationAncestor::OuterDeinit()
{
	TryUpdateMainThreadId();
	*Console<<"Outer Deinit";
	CheckGlError("OuterDeinit");
	InnerDeinit();
}


void TApplicationAncestor::OuterDraw()
{
	TryUpdateMainThreadId();

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
	TryUpdateMainThreadId();

	ResourceManager->Update(timer);

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

void TApplicationAncestor::OuterOnTapUpAfterMove(vec2 p)
{
	InnerOnTapUpAfterMove(p);
}

void TApplicationAncestor::OuterOnMove(vec2 shift)
{
	ResourceManager->GUIManager.OnMove(shift);
	InnerOnMove(shift);
}

} //namespace SE