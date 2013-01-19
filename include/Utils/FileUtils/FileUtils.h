#ifndef FILE_UTILS_H_INCLUDED
#define FILE_UTILS_H_INCLUDED

/*
This code contains API to ease file access

WINDOWS AND ANDROID
*/

#include "include/Utils/DataTypes/DataTypes.h"
#include "include/Utils/Console/Console.h"
#include "boost/shared_array.hpp"

#ifdef TARGET_WIN32
#include "include/Utils/WinApi/WinApi.h"
#endif

#ifdef TARGET_ANDROID
#include "include/Utils/JniApi/JniApi.h"
#endif

#ifdef TARGET_IOS
#include "include/Utils/IosApi/IosApi.h"
#endif

namespace SE
{

bool findString(char* in, char* list);
//utility for opengl extensions parsing. Not safe


bool IsFileExistsInUserData(const std::string& filename);
std::string GetFilePathUserData(const std::string& filename);


#ifdef TARGET_WIN32

template<typename TYPENAME>
boost::shared_array<TYPENAME> CreateMemFromFile(const std::string& fileName, cardinal& intCount)
{
	cardinal SIZEOF_TYPENAME = sizeof(TYPENAME);
    
    FILE * pFile;
    
    long fSize;
    
    size_t result;
    
    TYPENAME* fileData;
    
    std::string realFileName = fileName;
    
    
    if (fopen_s(&pFile, realFileName.c_str(), "rb" ) != 0) 
    {
        throw ErrorToLog("File not loaded: " + fileName);
    }
        
        // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    fSize = ftell (pFile);
    rewind (pFile);
        
    fileData = new TYPENAME [fSize % SIZEOF_TYPENAME == 0 ? fSize/SIZEOF_TYPENAME : fSize/SIZEOF_TYPENAME + 1];
       
    result = fread (&fileData[0], 1, fSize, pFile);
    
    if (result != fSize)
    {
        throw ErrorToLog("File not loaded: " + fileName);
    }
    
    // terminate
    fclose (pFile);
    
    intCount = fSize;
    
    return boost::shared_array<TYPENAME>(fileData);
	
}
#endif

#ifdef TARGET_ANDROID

template<typename TYPENAME>
boost::shared_array<TYPENAME> CreateMemFromFile(const std::string& fileName, cardinal& intCount)
{
	if (std::string(fileName.begin(), fileName.begin() + 5) == "/data")
	{
		*Console<<"File is in userdata - "+fileName;

		cardinal SIZEOF_TYPENAME = sizeof(TYPENAME);

		FILE * pFile;

		long fSize;

		size_t result;

		TYPENAME* fileData;

		std::string realFileName = fileName;

		pFile = fopen ( realFileName.c_str(), "rb" );
		if (pFile == NULL) 
		{
			throw ErrorToLog("File not loaded: " + fileName);
		}

		// obtain file size:
		fseek (pFile , 0 , SEEK_END);
		fSize = ftell (pFile);
		rewind (pFile);

		fileData = new TYPENAME [fSize % SIZEOF_TYPENAME == 0 ? fSize/SIZEOF_TYPENAME : fSize/SIZEOF_TYPENAME + 1];

		result = fread (&fileData[0], 1, fSize, pFile);

		if (result != fSize)
		{
			throw ErrorToLog("File not loaded: " + fileName);
		}

		// terminate
		fclose (pFile);

		intCount = fSize;

		return boost::shared_array<TYPENAME>(fileData);
	}

	*Console<<"File is in resources - "+fileName;

	return JniCreateMemFromFile<TYPENAME>(fileName, intCount);
}


#endif

#ifdef TARGET_IOS

template<typename TYPENAME>
boost::shared_array<TYPENAME> CreateMemFromFile(const std::string& fileName, cardinal& intCount)
{
    cardinal SIZEOF_TYPENAME = sizeof(TYPENAME);
    
    FILE * pFile;
    
    long fSize;
    
    size_t result;
    
    TYPENAME* fileData;
    
    std::string realFileName = IosGetFileReadPath(fileName);
    
    pFile = fopen ( realFileName.c_str(), "rb" );
    if (pFile == NULL) 
    {
        throw ErrorToLog("File not loaded: " + fileName);
    }
        
        // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    fSize = ftell (pFile);
    rewind (pFile);
        
    fileData = new TYPENAME [fSize % SIZEOF_TYPENAME == 0 ? fSize/SIZEOF_TYPENAME : fSize/SIZEOF_TYPENAME + 1];
       
    result = fread (&fileData[0], 1, fSize, pFile);
    
    if (result != fSize)
    {
        throw ErrorToLog("File not loaded: " + fileName);
    }
    
    // terminate
    fclose (pFile);
    
    intCount = fSize;
    
    return boost::shared_array<TYPENAME>(fileData);
        
}


#endif

//utitily to process texture uploading
inline char* GetFileName(const char* filename)
{
	char* fname = (char*)filename + strlen(filename);

	while ((*fname != '\\')&&(*fname != '/')&&(fname >= filename )) 
		--fname;

	++fname;

	return fname;
}

inline std::string GetFileName(const std::string& filename)
{
	std::string::const_iterator i = filename.end() - 1;

	while ((i > filename.begin() )&&(*i != '\\')&&(*i != '/')) 
		--i;
	
	if (*i == '\\' || *i == '/')
	{
		i++;
	}

	return std::string(i, filename.end());
}

//utitily to process texture uploading
inline char* GetFileExt(const char* filename)
{
	char* fext = (char*)filename + strlen(filename);

	while (*fext != '.') 
		--fext;

	return fext;
}

inline std::string GetFileExt(const std::string& filename)
{
	std::string::const_iterator i = filename.end() - 1;

	while (*i != '.') 
		--i;

	return std::string(i, filename.end());

}

std::string GetFilePath(const std::string& filename);

#ifdef TARGET_IOS

//Special for IOS, because Foundation.h conflicts with sq_plus.h

// Obviously, this returns ResourceManager->PathToResources

std::string GetPathToResources();
#endif

} //namespace SE

#endif