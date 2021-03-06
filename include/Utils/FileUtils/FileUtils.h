#ifndef FILE_UTILS_H_INCLUDED
#define FILE_UTILS_H_INCLUDED

/*
This code contains API to ease file access

WINDOWS AND ANDROID
*/

#include "include/Utils/DataTypes/DataTypes.h"
#include "include/Utils/ErrorTypes/ErrorTypes.h"
#include "include/Utils/Console/Console.h"
#include "boost/shared_array.hpp"

#ifdef TARGET_WIN32
#include "include/Utils/WinApi/WinApi.h"
#endif

#ifdef TARGET_WINDOWS_UNIVERSAL
#include "include/Utils/WindowsUniversalApi/WindowsUniversalApi.h"
#endif


#ifdef TARGET_ANDROID
#include "include/Utils/JniApi/JniApi.h"
#endif

#ifdef TARGET_IOS
#include "include/Utils/IosApi/IosApi.h"
#endif

namespace SE
{
    
namespace ST
{
    
	extern std::string PathToResources;

}

bool findString(char* in, char* list);
//utility for opengl extensions parsing. Not safe


bool IsFileExistsInUserData(const std::string& filename);

std::string GetFilePathUserData(const std::string& filename);



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
	auto i = filename.end() - 1;

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
	auto i = filename.end() - 1;

	while (*i != '.') 
		--i;

	return std::string(i, filename.end());

}

inline std::string GetFileNameWithoutExt(const std::string& filename)
{
	std::string result = GetFileName(filename);

	std::string::iterator i = result.end() - 1;

	while (*i != '.') 
		--i;

	return std::string(result.begin(), i);
}

std::string GetFilePath(const std::string& filename);



#ifdef TARGET_WIN32

void GetFileList(const std::string& searchkey, std::vector<std::string> &list);

std::string AutocompleteExtension(const std::string& fileName);

template<typename TYPENAME>
boost::shared_array<TYPENAME> CreateMemFromFile(const std::string& fileName, size_t& intCount)
{
	size_t SIZEOF_TYPENAME = sizeof(TYPENAME);
    
    FILE * pFile;
    
    long fSize;
    
    size_t result;
    
    TYPENAME* fileData;

    
    if (fopen_s(&pFile, fileName.c_str(), "rb" ) != 0) 
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
		delete[] fileData;
        throw ErrorToLog("File not loaded: " + fileName);
    }
    
    // terminate
    fclose (pFile);
    
    intCount = fSize;
    
    return boost::shared_array<TYPENAME>(fileData);
	
}
#endif

#ifdef TARGET_WINDOWS_UNIVERSAL

template<typename TYPENAME>
boost::shared_array<TYPENAME> CreateMemFromFile(const std::string& fileName, size_t& intCount)
{
	size_t SIZEOF_TYPENAME = sizeof(TYPENAME);

	FILE * pFile;

	long fSize;

	size_t result;

	TYPENAME* fileData;

	pFile = fopen(fileName.c_str(), "rb");

	if (!pFile)
	{
		throw ErrorToLog("File not loaded: " + fileName);
	}

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	fSize = ftell(pFile);
	rewind(pFile);

	fileData = new TYPENAME[fSize % SIZEOF_TYPENAME == 0 ? fSize / SIZEOF_TYPENAME : fSize / SIZEOF_TYPENAME + 1];

	result = fread(&fileData[0], 1, fSize, pFile);

	if (result != fSize)
	{
		delete[] fileData;
		throw ErrorToLog("File not loaded: " + fileName);
	}

	// terminate
	fclose(pFile);

	intCount = fSize;

	return boost::shared_array<TYPENAME>(fileData);

}
#endif

#ifdef TARGET_LINUX

template<typename TYPENAME>
boost::shared_array<TYPENAME> CreateMemFromFile(const std::string& fileName, size_t& intCount)
{
	size_t SIZEOF_TYPENAME = sizeof(TYPENAME);
    
    FILE * pFile;
    
    long fSize;
    
    size_t result;
    
    TYPENAME* fileData;

    pFile = fopen(fileName.c_str(), "rb" );

    if (!pFile) 
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
		delete[] fileData;
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
boost::shared_array<TYPENAME> CreateMemFromFile(const std::string& fileName, size_t& intCount)
{
	if (std::string(fileName.begin(), fileName.begin() + 5) == "/data")
	{
		*Console<<"File is in userdata - "+fileName;

		size_t SIZEOF_TYPENAME = sizeof(TYPENAME);

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
			delete[] fileData;
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
boost::shared_array<TYPENAME> CreateMemFromFile(const std::string& fileName, size_t& intCount)
{
    size_t SIZEOF_TYPENAME = sizeof(TYPENAME);
    
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
		delete[] fileData;
        throw ErrorToLog("File not loaded: " + fileName);
    }
    
    // terminate
    fclose (pFile);
    
    intCount = fSize;
    
    return boost::shared_array<TYPENAME>(fileData);
        
}


#endif

#ifdef TARGET_IOS

//Special for IOS, because Foundation.h conflicts with sq_plus.h

// Obviously, this returns ST::PathToResources

std::string GetPathToResources();
#endif

} //namespace SE

#endif