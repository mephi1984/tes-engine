#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

/*
This code contains console/log to work with

Use global variable Console like that:

*Console<<"something"<<endl;
*/

#include <iostream>
#include <string>

#ifdef TARGET_WIN32
#include <fstream>
#endif
#ifdef TARGET_LINUX
#include <fstream>
#endif
#ifdef TARGET_ANDROID
#include <asm/page.h>
#include <limits.h>

#include "boost/thread.hpp"
#endif


#include "include/Render/RenderMisc.h"


namespace SE
{

extern const std::string CONST_CONSOLE_TEX_NAME;



class TSimpleConsole
{
protected:
	std::string History;

	std::string TextSavedInTriangleList;

	std::shared_ptr<TTriangleList> HistoryTriangeList;

	

public:
	std::string ConsoleInput;
	cardinal ConsoleCursor;
	std::vector<std::string> InputHistory;
	cardinal InputHistoryCursor;

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
    boost::mutex ConsoleMutex;
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

