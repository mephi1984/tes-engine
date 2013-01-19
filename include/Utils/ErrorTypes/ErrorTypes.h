#ifndef ERROR_TYPES_H_INCLUDED
#define ERROR_TYPES_H_INCLUDED

/*
This code contains exception types for asserting and log watch
*/

#include "include/Utils/Console/Console.h"
#include "include/Utils/DataTypes/DataTypes.h"

namespace SE
{

#ifdef TARGET_WIN32
extern TFileConsole* Console;
#endif

#ifdef TARGET_ANDROID
extern TJavaConsole* Console;
#endif


#ifdef TARGET_IOS
extern TIosConsole* Console;
#endif

class ErrorCommon
{
public:
	ErrorCommon() {/* *Console<<"Common error!";*/ }
	virtual ~ErrorCommon() { }
};

class ErrorToLog : public ErrorCommon
{
public:
	ErrorToLog(const std::string& message) { *Console<<message; }
};

class ErrorFile : public ErrorCommon
{
public:
	ErrorFile(const std::string& fileName) { *Console<<"File error in file: "+fileName; }
};

class ErrorFileNotFound : public ErrorFile
{
public:
	ErrorFileNotFound(const std::string& fileName) : ErrorFile(fileName)  { *Console<<"File not found: "<<fileName; }
};

class ErrorFileNotCorrect : public ErrorFile
{
public:
	ErrorFileNotCorrect(const std::string& fileName) : ErrorFile(fileName) { *Console<<"File not correct: "+fileName; }
};

class ErrorFileNotRead : public ErrorFile
{
public:
	ErrorFileNotRead(const std::string& fileName) : ErrorFile(fileName) { *Console<<"File could not be read: "+fileName; }
};

class ErrorFileNotLoaded : public ErrorFile
{
public:
	ErrorFileNotLoaded(const std::string& fileName) : ErrorFile(fileName) { *Console<<"File could not be loaded: "+fileName; }
};

class ErrorFileTooShort : public ErrorFile
{
public:
	ErrorFileTooShort(const std::string& fileName) : ErrorFile(fileName) { *Console<<"File is too short: "+fileName; }
};


class ErrorShaderNotExists : public ErrorCommon
{
public:
	ErrorShaderNotExists(const std::string& shaderName) { *Console<<"Shader not exists: "+shaderName; }
};


class ErrorShaderStackOverflow : public ErrorCommon
{
public:
	ErrorShaderStackOverflow() { *Console<<"Shader stack overflow!"; }
};


} //namespace SE

#endif