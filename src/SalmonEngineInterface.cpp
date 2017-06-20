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


#ifdef TARGET_WINDOWS_UNIVERSAL
TFileConsole* Console;
TSalmonRendererWindowsUniversal* Renderer;
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

void TApplicationAncestor::ChangeWidthHeight(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{
	
	bool changed = false;

	if (screenWidth != Renderer->GetScreenWidth()
		|| screenHeight != Renderer->GetScreenHeight()
		|| matrixWidth != Renderer->GetMatrixWidth()
		|| matrixHeight != Renderer->GetMatrixHeight())
	{
		changed = true;
	}

	Renderer->ChangeWidthHeight(screenWidth, screenHeight, matrixWidth, matrixHeight);

	if (changed)
	{
		if (ResourceManager->newGuiManager.IsInited())
		{
			ResourceManager->newGuiManager.UpdateOnWindowResize();
		}

		InnerChangeWidthHeight(screenWidth, screenHeight, matrixWidth, matrixHeight);
	}

	
}

void TApplicationAncestor::InnerChangeWidthHeight(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{

}


void TApplicationAncestor::OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{
	ST::MainThreadId = std::this_thread::get_id();

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

	CheckGlError();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, Renderer->GetScreenWidth(), Renderer->GetScreenHeight());

	CheckGlError();

	glActiveTexture(GL_TEXTURE0);
	RenderUniform1i("Texture", 0);
	RenderUniform1f("Transparency", 1.f);

	CheckGlError();
	
	InnerDraw();

	CheckGlError();

	ResourceManager->GUIManager.Draw();

	CheckGlError();

	ResourceManager->newGuiManager.Draw();

	CheckGlError();

	Renderer->PushShader("DefaultShader");

	CheckGlError();
	
	if (IsConsoleOut) 
	{
		Console->Draw();
	}

	Renderer->PopShader();

	CheckGlError();
}

void TApplicationAncestor::OuterUpdate(size_t timer)
{
	
	TryUpdateMainThreadId();

	CheckGlError();

	ResourceManager->Update(timer);

	CheckGlError();

	InnerUpdate(timer);
	
	CheckGlError();
}


void TApplicationAncestor::OuterOnTapDown(Vector2f p, int touchNumber)
{
	ResourceManager->GUIManager.OnMouseDown(p, touchNumber);

	if (!ResourceManager->newGuiManager.OnMouseDown(p, touchNumber))
	{
		InnerOnTapDown(p);
	}
}

void TApplicationAncestor::OuterOnTapUp(Vector2f p, int touchNumber)
{
	ResourceManager->GUIManager.OnMouseUp(p, touchNumber);

	if (!ResourceManager->newGuiManager.OnMouseUp(p, touchNumber))
	{
		InnerOnTapUp(p);
	}
}

void TApplicationAncestor::OuterOnTapUpAfterMove(Vector2f p, int touchNumber)
{
    ResourceManager->GUIManager.OnMouseUpAfterMove(p, touchNumber);

	if (!ResourceManager->newGuiManager.OnMouseUpAfterMove(p, touchNumber))
	{
		InnerOnTapUpAfterMove(p);
	}
}

void TApplicationAncestor::OuterOnMove(Vector2f pos, Vector2f shift, int touchNumber)
{
	ResourceManager->GUIManager.OnMove(shift, touchNumber);

	if (!ResourceManager->newGuiManager.OnMove(pos, shift, touchNumber))
	{
		InnerOnMove(pos, shift);
	}
}

void TApplicationAncestor::OnMouseMove(TMouseState& mouseState) //Windows only
{
	if (!mouseState.LeftButtonPressed && !mouseState.RightButtonPressed && !mouseState.MiddleButtonPressed)
	{
		ResourceManager->newGuiManager.OnMouseMove(Vector2f(mouseState.X, SE::Renderer->GetMatrixHeight() - mouseState.Y));
	}

	InnerOnMouseMove(mouseState);
}

} //namespace SE