#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

/*
This code contains console/log to work with

Use global variable Console like that:

*Console<<"something"<<endl;
*/

#include <iostream>
#include <string>
#include <vector>

#ifdef TARGET_WIN32
#include <fstream>
#endif
#ifdef TARGET_LINUX
#include <fstream>
#endif
#ifdef TARGET_ANDROID
//#include <asm/page.h>
//#include <limits.h>
#endif

#ifdef TARGET_WINDOWS_UNIVERSAL
#include <fstream>
#endif

#include "include/Utils/DataTypes/DataTypes.h"

#ifndef UTILS_ENGINE
#include "include/Render/RenderMisc.h"
#endif

#include <thread>
#include <mutex>
namespace SE
{

extern const std::string CONST_CONSOLE_TEX_NAME;



class TSimpleConsole
{
protected:
	std::string History;

	std::string TextSavedInTriangleList;

#ifndef UTILS_ENGINE
	std::shared_ptr<TTriangleList> HistoryTriangeList;
#endif
	

public:
	std::string ConsoleInput;
	size_t ConsoleCursor;
	std::vector<std::string> InputHistory;
	size_t InputHistoryCursor;

	TSimpleConsole() : History(""), TextSavedInTriangleList(""), ConsoleCursor(0), InputHistoryCursor(0) { }
	
	virtual ~TSimpleConsole() { }
	
	void Clear() { History = ""; }

	void CutHistory();

	virtual std::string GetHistory();

#ifndef UTILS_ENGINE
	virtual void Draw();
#endif
};


#ifdef TARGET_WIN32

class TFileConsole : public TSimpleConsole
{
protected:
	std::string filename;
	std::ofstream f;
public:
	TFileConsole();
	
	TFileConsole(const std::string& Afilename);
	
	~TFileConsole();
	
	TFileConsole& operator<<(const std::string& s);
    
    void PrintImmediate(const std::string& s);
};

#endif


#ifdef TARGET_WINDOWS_UNIVERSAL

class TFileConsole : public TSimpleConsole
{
protected:
	std::string filename;
	std::ofstream f;
public:
	TFileConsole();

	TFileConsole(const std::string& Afilename);

	~TFileConsole();

	TFileConsole& operator<<(const std::string& s);

	void PrintImmediate(const std::string& s);
};

#endif


#ifdef TARGET_LINUX

class TFileConsole : public TSimpleConsole
{
protected:
	std::string filename;
	std::ofstream f;
public:
	TFileConsole();
	
	TFileConsole(const std::string& Afilename);
	
	~TFileConsole();
	
	TFileConsole& operator<<(const std::string& s);
    
    void PrintImmediate(const std::string& s);
};

#endif


#ifdef TARGET_ANDROID

class TJavaConsole : public TSimpleConsole
{
protected:
    std::mutex ConsoleMutex;
    std::string AppDir;
    std::string LogFilename;
public:

	TJavaConsole();
	
	TJavaConsole& operator<<(const std::string& s);
    
    void PrintImmediate(const std::string& s);
};

#endif


#ifdef TARGET_IOS

class TIosConsole : public TSimpleConsole
{
protected:
public:
	TIosConsole();
	
	//TFileConsole(const std::string& Afilename);
	
	~TIosConsole();
	
	TIosConsole& operator<<(const std::string& s);
    
    void PrintImmediate(const std::string& s);
};


#endif

} //namespace SE

#endif

