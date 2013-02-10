#ifndef JNI_API_H_INCLUDED
#define JNI_API_H_INCLUDED

#include <jni.h>
#include <string>
#include "include/Utils/DataTypes/DataTypes.h"
#include "include/Utils/ErrorTypes/ErrorTypes.h"
#include "boost/shared_array.hpp"

#include "zip.h"
#include "zipint.h"
#include <zlib.h>
#include <zconf.h>

namespace SE
{

/*

This code mostly hard to explain
You may treat this like a factory-class

But you cannot put JNI functions inside a class, that's why it is not a real class

The only thing you need from here - functions JniCreateDataFromFile and JniCreateDataFromAlignedFile for FileUtils.h/cpp

See doc for details 

*/

extern const std::string CONST_JAVA_FILESYSTEM_CLASS_NAME;

//All 3 deprecated
extern cardinal* FileArr;
extern int FileSize;
extern std::string ApkFilePath;

extern "C" {
    JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_SetupEnviroment(JNIEnv* env, jobject thiz);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_SetupApkFilePath(JNIEnv* env, jobject thiz, jstring s);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_ConsoleOut(JNIEnv* env, jobject thiz, jstring s);
	
	//All 2 deprecated
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_CreateFile(JNIEnv* env, jobject thiz, int fileSize);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_WriteToFile(JNIEnv* env, jobject thiz, jbyteArray buffer, int bufferSize);
	
	
};

//Deprecated
void JniCallOpenFile(const std::string& fileName);


//Deprecated
template<typename TYPENAME>
boost::shared_array<TYPENAME> JniCreateMemFromFileOld(const std::string& fileName, cardinal& blockCount)
{
    
    //if size is too large - need to do some work
    cardinal SIZEOF_TYPENAME = sizeof(TYPENAME);
    if (SIZEOF_TYPENAME > 4)
    {
        throw ErrorToLog("ERROR! Size of type too large - file is not properly aligned. Need work!");
    }
	
	if (FileArr != NULL)
		throw ErrorToLog("ERROR: Trying to reach two files at same time!!!!");

	JniCallOpenFile(fileName);
	
	// ...
	//Wait until Java pass all file data to FileArr through WriteToFile()
	// ...

	//Give pointer away
	boost::shared_array<TYPENAME> r(reinterpret_cast<TYPENAME*>(FileArr));
	blockCount = FileSize % SIZEOF_TYPENAME == 0 ? FileSize / SIZEOF_TYPENAME: FileSize / SIZEOF_TYPENAME + 1;
	FileArr = NULL; 
	FileSize = 0;
	return r;
}


std::string JniGetApplicationDir();

void JniLoadSound(const std::string& fileName);
void JniPlaySound(const std::string& soundName);

void JniPlayMusic(const std::string& fileName);
void JniPlayMusicLooped(const std::string& fileName);
void JniStopMusic();

void JniReleaseAllSoundsAndMusic();
void JniPauseAllSoundsAndMusic();//NIU right now
void JniResumeAllSoundsAndMusic(); //NIU right now

void LoadApk(const std::string& fileName, std::vector<char>& returnData);

void PrintApkContent();


template<typename TYPENAME>
boost::shared_array<TYPENAME> JniCreateMemFromFile(const std::string& fileName, cardinal& blockCount)
{
	
	try
	{

	//if size is too large - need to do some work
    cardinal SIZEOF_TYPENAME = sizeof(TYPENAME);
    if (SIZEOF_TYPENAME > 4)
    {
        throw ErrorToLog("ERROR! Size of type too large - file is not properly aligned. Need work!");
    }
	
	std::vector<char> fileData;
	
	LoadApk(fileName, fileData);
	
	cardinal fileSize = fileData.size();
	
	blockCount = fileSize % SIZEOF_TYPENAME == 0 ? fileSize / SIZEOF_TYPENAME: fileSize / SIZEOF_TYPENAME + 1;
	
	boost::shared_array<TYPENAME> result(new TYPENAME[blockCount]);
   
    memcpy(&result[0], &fileData[0], fileSize);

	return result; 
	
	}
	catch(ErrorCommon e)
	{
		throw;
	}
}

} //namespace SE


#endif