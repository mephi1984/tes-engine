
#include "include/Utils/Utils.h"
#include "include/SalmonEngineWindows.h"


#include "Windowsx.h"

namespace SE
{

//==============================================================
//========= GLOBAL VARIABLES - REMEMBER THIS LIST!!! ===========
//==============================================================

HGLRC hRC;		//Render context
HWND Hwnd;		//Main window handle
HDC hDC;		//Device context

//==============================================================
//=============== GLOBAL VARS ENDS =============================
//==============================================================

//There might be anything
const char CONST_WINDOW_CLASS_NAME[] = "TSalmonEngineForm";

const size_t CONST_TIMER_INTERVAL = 10;

//To calculate when to call Update
size_t NewTickCount;
size_t LastTickCount;

//User application
TApplication* App = NULL;

Vector2f MouseButtonPos;
Vector2f MouseTotalShift;
bool MouseButtonPressed = false;
bool MouseMoved = false;


TApplication::TApplication() 
{
}

TApplication::~TApplication()
{
}

void TApplication::OnKeyPress(size_t key)
{
	if (IsConsoleOut)
	{
		
		if (key == 96 || key == 126 /*|| key == 184 || key == 168*/) //~, 184 and 168 is russian e
		{
			IsConsoleOut = false;
		}
		else if (key == 8) //Backspace
		{
			if (Console->ConsoleInput.size() > 0 && Console->ConsoleCursor > 0)
			{
				Console->ConsoleInput.erase(Console->ConsoleInput.begin() + Console->ConsoleCursor-1);
				Console->ConsoleCursor--;
			}
		}
		else if (key == 13|| key == 10) //Enter
		{
			//ResourceManager->ScriptManager.RunScript(Console->ConsoleInput);

			Console->InputHistory.push_back(Console->ConsoleInput);

			Console->InputHistoryCursor = Console->InputHistory.size();

			Console->ConsoleInput = "";
			Console->ConsoleCursor = 0;
		}
		else if (key >= 32 && key <= 255) //ASCII
		{
			Console->ConsoleInput.insert(Console->ConsoleInput.begin()+Console->ConsoleCursor, static_cast<char>(key));
			Console->ConsoleCursor++;
		}
		else if (key > 0x100) //VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT
		{
			size_t realKey = key - 0x100;

			if (realKey == VK_UP)
			{
				if (Console->InputHistoryCursor > 0)
				{
					Console->InputHistoryCursor--;
					Console->ConsoleInput = Console->InputHistory[Console->InputHistoryCursor];
					Console->ConsoleCursor = Console->ConsoleInput.size();
				}
			}
			if (realKey == VK_DOWN)
			{
				if ( Console->InputHistoryCursor+1 < Console->InputHistory.size())
				{
					Console->InputHistoryCursor++;
					Console->ConsoleInput = Console->InputHistory[Console->InputHistoryCursor];
					Console->ConsoleCursor = Console->ConsoleInput.size();
				}
			}
			if (realKey == VK_LEFT)
			{
				if (Console->ConsoleCursor > 0)
				{
					Console->ConsoleCursor--;
				}
			}
			if (realKey == VK_RIGHT)
			{
				if (Console->ConsoleCursor < Console->ConsoleInput.size())
				{
					Console->ConsoleCursor++;
				}
			}

		}
		else
		{
			*Console<<"Key is "+tostr(key)+ "\n";
		}

	}
	else
	{
		if (key == 96 || key == 126 /*|| key == 184 || key == 168*/) //~, 184 and 168 is russian e
		{
			IsConsoleOut = true;
			Console->ConsoleInput = "";
			Console->ConsoleCursor = Console->ConsoleInput.size();
		}
		else
		{
			ResourceManager->GUIManager.KeyPressedSignal(static_cast<int>(key));
			ResourceManager->newGuiManager.OnKeyPressed(static_cast<int>(key));
		}

		App->InnerOnKeyPress(key);
	}
}


void DrawScene()
{
	RECT clientRect;


	if (GetClientRect(Hwnd, &clientRect))
	{
		App->ChangeWidthHeight(clientRect.right, clientRect.bottom, clientRect.right, clientRect.bottom);
	}

	App->OuterDraw();
    SwapBuffers(hDC);
}

void ProcessTickCount()
{
	//Can't choose witch one is better

	/*
	App->UpdateQuick();
	NewTickCount = GetTickCount();
	if (NewTickCount - LastTickCount > CONST_TIMER_INTERVAL)
	{
		LastTickCount = NewTickCount;
		App->OuterUpdate(CONST_TIMER_INTERVAL);
	}*/


	NewTickCount = GetTickCount();
	if (NewTickCount - LastTickCount > CONST_TIMER_INTERVAL)
	{
		App->OuterUpdate(NewTickCount - LastTickCount);
		LastTickCount = NewTickCount;
	}

}

LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    static PAINTSTRUCT ps;

	TMouseState mouseState;

    switch(uMsg) 
	{
	case WM_CLOSE:
		Hwnd = 0; //HACK to force window to close
		PostQuitMessage(0);
		//Xperimental
		return DefWindowProc(hWnd, uMsg, wParam, lParam); 

		break;
	case WM_PAINT: 

		
		DrawScene();
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);

		//ProcessTickCount();
		break;
	case WM_SIZE:
		
		PostMessage(hWnd, WM_PAINT, 0, 0);
		break;
	case WM_CHAR:
		App->OnKeyPress(wParam);
		/*
		switch (wParam) 
		{
		case 27:
				PostQuitMessage(0);
				Hwnd = 0;
				
				//Xperimental
				return DefWindowProc(hWnd, uMsg, wParam, lParam); 

				break;
		//... Will add more code later
		}*/
		break;

case WM_MOUSEMOVE:
		mouseState.X = GET_X_LPARAM(lParam);
		mouseState.Y = GET_Y_LPARAM(lParam);


		mouseState.LeftButtonPressed = (wParam & MK_LBUTTON);
		mouseState.MiddleButtonPressed = (wParam & MK_MBUTTON);
		mouseState.RightButtonPressed = (wParam & MK_RBUTTON);

		App->OnMouseMove(mouseState);

		if (MouseButtonPressed)
		{
			Vector2f currentMousePos(static_cast<float>(mouseState.X), static_cast<float>(Renderer->GetScreenHeight() - mouseState.Y));
			
			currentMousePos(0) *= SE::Renderer->GetMatrixWidth() / SE::Renderer->GetScreenWidth();
			currentMousePos(1) *= SE::Renderer->GetMatrixHeight() / SE::Renderer->GetScreenHeight();

			Vector2f shift = (MouseButtonPos - currentMousePos);

			App->OuterOnMove(currentMousePos, shift, 0);

			MouseButtonPos = currentMousePos;

			MouseTotalShift += shift;

			if (fabs(MouseTotalShift(0)) > 10.f || fabs(MouseTotalShift(1)) > 10.f)
			{
				MouseMoved = true;
			}
		}

		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		mouseState.X = GET_X_LPARAM(lParam);
		mouseState.Y = GET_Y_LPARAM(lParam);
		mouseState.LeftButtonPressed = (wParam & MK_LBUTTON);
		mouseState.MiddleButtonPressed = (wParam & MK_MBUTTON);
		mouseState.RightButtonPressed = (wParam & MK_RBUTTON);

		MouseButtonPos = Vector2f(static_cast<float>(mouseState.X), static_cast<float>(Renderer->GetScreenHeight() - mouseState.Y));

		MouseButtonPos(0) *= SE::Renderer->GetMatrixWidth() / SE::Renderer->GetScreenWidth();
		MouseButtonPos(1) *= SE::Renderer->GetMatrixHeight() / SE::Renderer->GetScreenHeight();

		if (mouseState.LeftButtonPressed)
		{
			App->OuterOnTapDown(MouseButtonPos, 0);

		}

		MouseButtonPressed = true;

		MouseMoved = false;

		MouseTotalShift = Vector2f(0,0);
		


		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		mouseState.X = GET_X_LPARAM(lParam);
		mouseState.Y = GET_Y_LPARAM(lParam);
		mouseState.LeftButtonPressed = (wParam & MK_LBUTTON);
		mouseState.MiddleButtonPressed = (wParam & MK_MBUTTON);
		mouseState.MiddleButtonPressed = (wParam & MK_RBUTTON);


		MouseButtonPos = Vector2f(static_cast<float>(mouseState.X), static_cast<float>(Renderer->GetScreenHeight() - mouseState.Y));
		MouseButtonPos(0) *= SE::Renderer->GetMatrixWidth() / SE::Renderer->GetScreenWidth();
		MouseButtonPos(1) *= SE::Renderer->GetMatrixHeight() / SE::Renderer->GetScreenHeight();



		if (MouseMoved)
		{
			App->OuterOnTapUpAfterMove(MouseButtonPos, 0);
		}
		else
		{
			App->OuterOnTapUp(MouseButtonPos, 0);
		}

		MouseButtonPressed = false;

		break;
	
	case WM_MOUSEWHEEL:
		App->OnMouseWheel(wParam >> 16);
		break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 

bool CreateOpenGLWindow(const char* title, int x, int y, int width, int height,HWND& hWnd, HDC& hDC, HGLRC& hRC)
{
    int pf;
    WNDCLASS wc;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    if (!hInstance) 
	{
		hInstance = GetModuleHandle(NULL);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = CONST_WINDOW_CLASS_NAME;

		if (!RegisterClass(&wc)) 
		{
			return NULL;
		}
    }

	RECT r;
	r.left = x;
	r.right = x + width;
	r.top = y;
	r.bottom = y + height;
	
	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN; 

	AdjustWindowRect(&r, windowStyle, false);
	
	hWnd = CreateWindow(CONST_WINDOW_CLASS_NAME, title, windowStyle, 0, 0, r.right-r.left, r.bottom-r.top, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) 
	{
		return false;
    }

    hDC = GetDC(hWnd);

    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) 
	{
		return false;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) 
	{
		return false;
    } 

    hRC = wglCreateContext( hDC );
	wglMakeCurrent( hDC, hRC );

    return true;
}

bool CreateEngine(int width, int height, int x, int y, std::string windowName, std::string logFileName)
{
	
	//Here console log is not created so we can not use ErrorCommon
	try
	{
		Console = new TFileConsole(logFileName);
	}
	catch(...)
	{
		return false;
	}

	//Here console log is already created

	*Console<<"Log started";
    
	if (!CreateOpenGLWindow(windowName.c_str(), x, y, width, height, Hwnd, hDC, hRC))
	{
		*Console<<"Unable to create OpenGL Window!";
		return false;
	}

	try
	{

		Renderer = new TSalmonRenderer;
		ResourceManager = new TResourceManager;


		return Renderer->BindOpenGLFunctions();

	}
	catch (...)
	{
		return false;
	}
}

void DestroyEngine()
{
	try
	{

		*Console<<"Program prepares to quit";

		delete ResourceManager;
		delete Renderer;


		wglMakeCurrent(NULL, NULL);
		ReleaseDC(Hwnd, hDC);
		wglDeleteContext(hRC);
		DestroyWindow(Hwnd);

	}
	catch(...)
	{
		//... let the ErrorTypes.h do the buisness
	}


	delete Console;

}

void MainLoop(TApplication* app)
{
	App = app;

	MSG msg;

	ShowWindow(Hwnd, SW_SHOW);
	UpdateWindow(Hwnd);

	NewTickCount = GetTickCount();
	LastTickCount = NewTickCount;

	bool StayIn = true;

	while (StayIn) 
	{
		while(PeekMessage(&msg, Hwnd, 0, 0, PM_NOREMOVE)) 
		{
			if(GetMessage(&msg, Hwnd, 0, 0)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} 
			else
			{
				StayIn = false;
			}
		}
		DrawScene();

		ProcessTickCount();
	}
}



} //namespace SE

