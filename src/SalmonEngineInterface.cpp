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



void TResourceManager::Update(size_t timer)
{

	GUIManager.Update(timer);
	newGuiManager.Update(timer);

	ST::MainThreadIoService.poll_one();
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

	//ResourceManager->ScriptManager.BindBasicFunctions();

	Renderer->InitOpenGL(screenWidth, screenHeight, matrixWidth, matrixHeight);

	ResourceManager->newGuiManager.Init();

	InnerInit();

	
    
    CheckGlError();
    
    srand(static_cast<unsigned int>(time(0)));
}

	
void TApplicationAncestor::OuterDeinit()
{
	ResourceManager->newGuiManager.Deinit();
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
	ResourceManager->newGuiManager.Draw();

	Renderer->PushShader("DefaultShader");
	
	if (IsConsoleOut) 
	{
		Console->Draw();
	}
	
	Renderer->PopShader();
	glEnable(GL_DEPTH_TEST);
}

void TApplicationAncestor::OuterUpdate(size_t timer)
{
	TryUpdateMainThreadId();

	ResourceManager->Update(timer);

	InnerUpdate(timer);

	CheckGlError();
}


void TApplicationAncestor::OuterOnTapDown(Vector2f p, int touchNumber)
{
	ResourceManager->GUIManager.OnMouseDown(p, touchNumber);
	ResourceManager->newGuiManager.OnMouseDown(p, touchNumber);
	InnerOnTapDown(p);
}

void TApplicationAncestor::OuterOnTapUp(Vector2f p, int touchNumber)
{
	ResourceManager->GUIManager.OnMouseUp(p, touchNumber);
	ResourceManager->newGuiManager.OnMouseUp(p, touchNumber);
	InnerOnTapUp(p);
}

void TApplicationAncestor::OuterOnTapUpAfterMove(Vector2f p, int touchNumber)
{
    ResourceManager->GUIManager.OnMouseUpAfterMove(p, touchNumber);
	ResourceManager->newGuiManager.OnMouseUpAfterMove(p, touchNumber);
	InnerOnTapUpAfterMove(p);
}

void TApplicationAncestor::OuterOnMove(Vector2f pos, Vector2f shift, int touchNumber)
{
	ResourceManager->GUIManager.OnMove(shift, touchNumber);
	ResourceManager->newGuiManager.OnMove(pos, shift, touchNumber);
	InnerOnMove(shift);
}

} //namespace SE