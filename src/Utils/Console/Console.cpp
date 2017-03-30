#include "include/Engine.h"
#include "boost/date_time.hpp"

#ifdef TARGET_ANDROID
#include <android/log.h>

#define APPNAME "SalmonEngine"

#endif

namespace SE
{

const size_t CONST_MAX_ROWS_IN_HISTORY = 20;

const float CONST_CONSOLE_PART_OF_SCREEN = 0.35f;

const int CONST_ROW_WIDTH = 14;

const std::string CONST_CONSOLE_TEX_NAME = "console_bkg.bmp";

#ifndef UTILS_ENGINE
void TSimpleConsole::Draw()
{


	RenderUniform1f("Transparency", 1.f);

	if (HistoryTriangeList.get() == 0)
	{
		HistoryTriangeList = std::shared_ptr<TTriangleList>(new TTriangleList);
	}

	float screenWidth = Renderer->GetMatrixWidth();
	float screenHeight = Renderer->GetMatrixHeight();
	glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList[CONST_CONSOLE_TEX_NAME]);
	
	Renderer->PushProjectionMatrix(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
	Renderer->PushMatrix();
	Renderer->LoadIdentity();

	Renderer->DrawRect(Vector2f(0.f, screenHeight*(1.f - CONST_CONSOLE_PART_OF_SCREEN)), Vector2f(screenWidth, screenHeight));
	
	if (TextSavedInTriangleList != History)
	{
		TextSavedInTriangleList = History;
		*HistoryTriangeList = ResourceManager->FontManager.DrawTextInBoxToVBO(Vector2f(0.f, screenHeight * (1.f - CONST_CONSOLE_PART_OF_SCREEN)), Vector2f(static_cast<float>(screenWidth), static_cast<float>(screenHeight)), CONST_ROW_WIDTH, TextSavedInTriangleList);
	}

	TTriangleList consoleInputTriangleList = ResourceManager->FontManager.DrawTextInBoxToVBO(Vector2f(0.f, screenHeight * (1.f - CONST_CONSOLE_PART_OF_SCREEN)), Vector2f(static_cast<float>(screenWidth), static_cast<float>(screenHeight)), CONST_ROW_WIDTH, ">"+ConsoleInput);

	glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList[ResourceManager->FontManager.GetCurrentFontTextureName()]);

	Renderer->DrawTriangleList(*HistoryTriangeList);
	Renderer->DrawTriangleList(consoleInputTriangleList);

	std::string ConsoleInputAtCursor = std::string((">"+ConsoleInput), 0, ConsoleCursor+1);

	float advance = ResourceManager->FontManager.GetTextAdvance(ConsoleInputAtCursor);

	TTriangleList consoleCursorTriangleList = ResourceManager->FontManager.DrawTextInBoxToVBO(Vector2f(advance, screenHeight*(1.f - CONST_CONSOLE_PART_OF_SCREEN)), Vector2f(static_cast<float>(screenWidth), static_cast<float>(screenHeight)), 14, "|");

	Renderer->DrawTriangleList(consoleCursorTriangleList);
	
	Renderer->PopMatrix();
	Renderer->PopProjectionMatrix();

}

#endif

void TSimpleConsole::CutHistory()
{
	/*
	size_t i = 0;

	size_t pos = std::string::npos;
	
	do
	{
		if (pos != std::string::npos && pos > 0)
		{
			pos--;
		}

		pos = History.find_last_of('\n', pos);
		i++;
	}
	while (pos != std::string::npos && i != CONST_MAX_ROWS_IN_HISTORY);

	if (pos != std::string::npos)
	{
		History = History.substr(pos);
	}*/
}

std::string TSimpleConsole::GetHistory()
{
	return History;
}


#ifdef TARGET_WIN32

TFileConsole::TFileConsole() 
	: filename("conlog.txt")
	, f("conlog.txt") 
{
}

TFileConsole::TFileConsole(const std::string& Afilename) 
	: filename(Afilename)
	, f(Afilename) 
{
}

TFileConsole::~TFileConsole() 
{
	f.close();
}

TFileConsole& TFileConsole::operator<<(const std::string& s) 
{
	boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();
	
	std::string string_with_time_mark;

	string_with_time_mark = to_simple_string(t)+": "+s;

	PrintImmediate(string_with_time_mark);

	History += string_with_time_mark+"\n";
	CutHistory();

	return *this;
}


void TFileConsole::PrintImmediate(const std::string& s)
{
    
	f<<s+ "\n";
	f.flush();

	OutputDebugString( (s+ "\n").c_str() );
    
}



#endif


#ifdef TARGET_LINUX

TFileConsole::TFileConsole() 
	: filename("conlog.txt")
	, f("conlog.txt") 
{
}

TFileConsole::TFileConsole(const std::string& Afilename) 
	: filename(Afilename)
	, f(Afilename) 
{
}

TFileConsole::~TFileConsole() 
{
	f.close();
}

TFileConsole& TFileConsole::operator<<(const std::string& s) 
{
	boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();
	
	std::string string_with_time_mark;

	string_with_time_mark = to_simple_string(t)+": "+s;

	PrintImmediate(string_with_time_mark);

	History += string_with_time_mark+endl;
	CutHistory();

	return *this;
}


void TFileConsole::PrintImmediate(const std::string& s)
{
    
	f<<s+endl;
	f.flush();
  
}



#endif


#ifdef TARGET_ANDROID

#include "include/Utils/JniApi/JniApi.h"

TJavaConsole::TJavaConsole()
{
	boost::mutex::scoped_lock scoped_lock(ConsoleMutex);

    AppDir = JniGetApplicationDir();
    LogFilename = AppDir + "/fishrungames-log.txt";

	FILE* file = fopen(LogFilename.c_str(), "w");
		
	if (file != NULL)
	{
		fclose(file);
	}
}

TJavaConsole& TJavaConsole::operator<<(const std::string& s) 
{ 
    boost::mutex::scoped_lock scoped_lock(ConsoleMutex);

	boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();
	
	std::string string_with_time_mark;

	string_with_time_mark = boost::posix_time::to_simple_string(t)+": "+s;

    PrintImmediate(string_with_time_mark);

	History += string_with_time_mark+'\n';
	CutHistory();


	//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "%s", string_with_time_mark.c_str());


	

	return *this; 
}


void TJavaConsole::PrintImmediate(const std::string& s)
{
    
	FILE* file = fopen(LogFilename.c_str(), "a");
    
	if (file != NULL)
	{
		fputs(std::string(s+'\n').c_str(), file);
		fflush(file);
		fclose(file);
	}
    
}


#endif


#ifdef TARGET_IOS

TIosConsole::TIosConsole() 
{
}



TIosConsole::~TIosConsole() 
{

}

TIosConsole& TIosConsole::operator<<(const std::string& s) 
{
	boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();
	
	std::string string_with_time_mark;
    
	string_with_time_mark = to_simple_string(t)+": "+s;
    
	PrintImmediate(string_with_time_mark);
    
	History += string_with_time_mark+endl;
	CutHistory();
    
	return *this;
}

void TIosConsole::PrintImmediate(const std::string& s)
{
    printf("%s", std::string(s+endl).c_str());
    
}


#endif


} //namespace SE



