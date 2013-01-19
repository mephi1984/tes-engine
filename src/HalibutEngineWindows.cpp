#include "include/Utils/Utils.h"
#include "include/HalibutEngineWindows.h"

namespace SE
{

//==============================================================
//========= GLOBAL VARIABLES - REMEMBER THIS LIST!!! ===========
//==============================================================

HGLRC hRC;		//Render context
HWND Hwnd;		//Main window handle
HDC hDC;		//Device context

TFileConsole* Console = NULL;

THalibutRenderer* Renderer = NULL;

THalibutResourceManager* ResourceManager = NULL;


//==============================================================
//=============== GLOBAL VARS ENDS =============================
//==============================================================

const int CONST_DEFAULT_SCREEN_WIDTH = 800;
const int CONST_DEFAULT_SCREEN_HEIGHT = 600;

const cardinal CONST_TIMER_INTERVAL = 10;

//There might be anything
const char CONST_WINDOW_CLASS_NAME[] = "TSalmonEngineForm";

//To calculate when to call Update
cardinal NewTickCount;
cardinal LastTickCount;

vec2 MouseButtonPos;
vec2 MouseTotalShift;
bool MouseButtonPressed = false;
bool MouseMoved = false;

//User application
TApplication* App = NULL;


TApplication::TApplication() 
	: X(0)
	, Y(0)
	, Width(800)
	, Height(600)
	, MatrixWidth(0.f)
	, MatrixHeight(0.f)
	, WindowName("Halibut Engine")
	, LogFilename("log.txt")
{
	for(int i=0; i<256; i++)
	{
		KeyboardBuffer[i] = 0;
	}
}

TApplication::~TApplication()
{
}

const std::string& TApplication::GetLogFilename()
{
	return LogFilename;
} 

char TApplication::GetKeyState(cardinal keyNum)
{
	if (keyNum > 255)
		throw ErrorToLog("GetKeyState - invalid key number!");

	return KeyboardBuffer[keyNum];
}


void TApplication::SetKeyboardBuffer(const unsigned char* source)
{
	memcpy((unsigned char*)KeyboardBuffer, source, 256);
}

void TApplication::OnKeyPress(cardinal key)
{

	if (IsConsoleOut)
	{
		
		if (key == 96 || key == 126) //~
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
			ResourceManager->ScriptManager.RunScript(Console->ConsoleInput);

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
			cardinal realKey = key - 0x100;

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
			*Console<<"Key is "+tostr(key)+endl;
		}

	}
	else
	{
		ResourceManager->GUIManager.KeyPressedSignal(key);

		if (key == 96 || key == 126) //~
		{
			IsConsoleOut = true;
			Console->ConsoleInput = "";
			Console->ConsoleCursor = Console->ConsoleInput.size();
		}
	}
}


void DrawScene()
{
	App->OuterDraw();
	SwapBuffers(hDC);
}

void ProcessTickCount()
{
	//unsigned char keyboardStateArr[256];

	NewTickCount = GetTickCount();
	if (NewTickCount - LastTickCount > CONST_TIMER_INTERVAL)
	{
		/*
		GetKeyboardState((unsigned char*)keyboardStateArr);
		App->SetKeyboardBuffer((unsigned char*)keyboardStateArr);
		*/
	
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

		ProcessTickCount();
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
		}
		*/
		break;
	case WM_KEYDOWN:
		//Special for arrow keys

		if (wParam == VK_UP || wParam == VK_DOWN || wParam == VK_LEFT || wParam == VK_RIGHT)
		{
			App->OnKeyPress(0x100 + wParam);
		}

		break;
	case WM_MOUSEMOVE:
		mouseState.X = (lParam << 16) >> 16;
		mouseState.Y = lParam >> 16;
		mouseState.LeftButtonPressed = (wParam & MK_LBUTTON);
		mouseState.MiddleButtonPressed = (wParam & MK_MBUTTON);
		mouseState.RightButtonPressed = (wParam & MK_RBUTTON);

		App->OnMouseMove(mouseState);

		if (MouseButtonPressed)
		{
			vec2 currentMousePos(static_cast<float>(mouseState.X), static_cast<float>(App->Height - mouseState.Y));
			vec2 shift = (MouseButtonPos - currentMousePos);
			//shift.v[1] = - shift.v[1];
			App->OuterOnMove(shift);
			//App->OuterOnMove(currentMousePos - MouseButtonPos);
			MouseButtonPos = currentMousePos;

			MouseTotalShift += shift;

			if (fabs(MouseTotalShift.v[0]) > 10.f || fabs(MouseTotalShift.v[1]) > 10.f)
			{
				MouseMoved = true;
			}
		}

		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		mouseState.X = (lParam << 16) >> 16;
		mouseState.Y = lParam >> 16;
		mouseState.LeftButtonPressed = (wParam & MK_LBUTTON);
		mouseState.MiddleButtonPressed = (wParam & MK_MBUTTON);
		mouseState.RightButtonPressed = (wParam & MK_RBUTTON);

		MouseButtonPos = vec2(static_cast<float>(mouseState.X), static_cast<float>(App->Height - mouseState.Y));

		if (mouseState.LeftButtonPressed)
		{
			App->OuterOnTapDown(MouseButtonPos);

		}

		MouseButtonPressed = true;

		MouseMoved = false;

		MouseTotalShift = vec2(0,0);
		


		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		mouseState.X = (lParam << 16) >> 16;
		mouseState.Y = lParam >> 16;
		mouseState.LeftButtonPressed = (wParam & MK_LBUTTON);
		mouseState.MiddleButtonPressed = (wParam & MK_MBUTTON);
		mouseState.MiddleButtonPressed = (wParam & MK_RBUTTON);

		if (!MouseMoved)
		{
			App->OuterOnTapUp(vec2(static_cast<float>(mouseState.X), static_cast<float>(App->Height - mouseState.Y)));
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

int MainLoop(TApplication& application)
{
    MSG msg;

	//Here console log is not created so we can not use ErrorCommon
	try
	{
		Console = new TFileConsole(application.GetLogFilename());
	}
	catch(...)
	{
		exit(1);
	}

	//Here console log is already created
	try
	{

	*Console<<"Log started";
    
	if (!CreateOpenGLWindow(application.WindowName.c_str(), application.X, application.Y, application.Width, application.Height,Hwnd,hDC,hRC))
	{
		*Console<<"Unable to create OpenGL Window!";
		exit(1);
	}

	*Console<<"OpenGL Window created";

	
		if (App != NULL)
			throw ErrorCommon();

		App = &application;

		Renderer = new THalibutRenderer;
		ResourceManager = new THalibutResourceManager;

		if (Renderer->BindOpenGLFunctions())
		{
			if (App->MatrixWidth == 0)
			{
				App->MatrixWidth = static_cast<float>(App->Width);
			}

			if (App->MatrixHeight == 0)
			{
				App->MatrixHeight = static_cast<float>(App->Height);
			}

			App->OuterInit(App->Width, App->Height, App->MatrixWidth, App->MatrixHeight);

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

		*Console<<"Program prepares to quit";

		App->OuterDeinit();

		delete ResourceManager;
		delete Renderer;


		wglMakeCurrent(NULL, NULL);
		ReleaseDC(Hwnd,hDC);
		wglDeleteContext(hRC);
		DestroyWindow(Hwnd);

	}
	catch(ErrorCommon)
	{
		//... let the ErrorTypes.h do the buisness
	}


	delete Console;

	
    return 0;
}


} //namespace SE
