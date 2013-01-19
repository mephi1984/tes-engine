#include "include/HalibutEngineAndroid.h"


namespace SE
{

TJavaConsole* Console = NULL;

THalibutResourceManager* ResourceManager = NULL;

THalibutRendererAndroid* Renderer = NULL;


TApplication::TApplication()
{
}

void TApplication::OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{

    OuterDeinit();
    
    Console = new TJavaConsole;
	
	*Console<<std::string("Console successfully started!!!");
	
    ResourceManager = new THalibutResourceManager;

	Renderer = new THalibutRendererAndroid;
	
	ResourceManager->ScriptManager.BindBasicFunctions();
	
	InnerInit();
	
	Renderer->InitOpenGL(screenWidth, screenHeight, matrixWidth, matrixHeight);
	
	
}

void TApplication::OuterDeinit()
{
	if (Console != NULL)
	{
	*Console<<"OuterDeinit";
    }
	InnerDeinit();
    
	
	if (ResourceManager != NULL)
    {
	   delete ResourceManager;
	   ResourceManager = NULL;
    }
	
    if (Renderer != NULL)
    {
	   delete Renderer;
	   Renderer = NULL;
    }
    
    
    
    if (Console != NULL)
    {
	   delete Console;
	   Console = NULL;
    }
	
}

TApplication::~TApplication()
{
    OuterDeinit();
}


void TApplication::OnKeyPress(cardinal key)
{
	//For Android 
	//7-16 => "0".."9"
	//29-54 => "a".."z"
	//

	if (IsConsoleOut)
	{
		
		if (key == 96 || key == 126) //` or ~
		{
	
			IsConsoleOut = false;
		}
		else if (key == 8) //Backspace
		{
			if (Console->ConsoleInput.size() > 0 && Console->ConsoleCursor > 0)
			{
				//Console->ConsoleInput.erase(Console->ConsoleInput.end()-1);
				Console->ConsoleInput.erase(Console->ConsoleInput.begin() + Console->ConsoleCursor-1);
				Console->ConsoleCursor--;
			}
		}
		else if (key == 13 || key == 10) //Enter
		{
			ResourceManager->ScriptManager.RunScript(Console->ConsoleInput);
			*Console<<"===>"+Console->ConsoleInput+"<===";

			Console->InputHistory.push_back(Console->ConsoleInput);

			Console->InputHistoryCursor = Console->InputHistory.size();

			Console->ConsoleInput = "";
			Console->ConsoleCursor = 0;
		}
		else if (key >= 32 && key <= 255) //ASCII
		{
			//Console->ConsoleInput += static_cast<char>(key);
			Console->ConsoleInput.insert(Console->ConsoleInput.begin()+Console->ConsoleCursor, static_cast<char>(key));
			Console->ConsoleCursor++;
		}
		else if (key > 0x100) //VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT
		{
		/*
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
			}*/
		}
		else
		{
			*Console<<std::string("Key is ")+tostr(key);
		}

	}
	else
	{
		if (key == 96 || key == 126) //~
		{
			IsConsoleOut = true;
			Console->ConsoleInput = "";
			Console->ConsoleCursor = Console->ConsoleInput.size();
		}
	}
}


} //namespace SE