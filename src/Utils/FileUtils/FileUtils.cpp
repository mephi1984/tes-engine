#include "include/Utils/FileUtils/FileUtils.h"

#include "include/Utils/Utils.h"

#include "include/Engine.h"

#include <string>

namespace SE
{
   
	//Xperimental HACK!!!
	#ifdef UTILS_ENGINE
TFileConsole* Console;
#endif

    namespace ST
    {
        
        std::string PathToResources;
        
    }

bool findString(char* in, char* list)
{
  int thisLength = strlen(in);
  while (*list != 0)
  {
    int length = strcspn(list," ");
    
    if( thisLength == length )
      if (!strncmp(in,list,length))
        return true;
        
    list += length + 1;
  }
  return false;
}

std::string GetFilePath(const std::string& filename)
{
	//any symbol - "/" or "\"
	
	int i = filename.find_last_of("/\\");

	if (i==filename.npos)
	{
		return "";
	}

	std::string r = filename.substr(0, i+1);

	return r;
}


bool IsFileExistsInUserData(const std::string& filename)
{

	std::string realFileName = GetFilePathUserData(filename);

	FILE* f;

	f = fopen(realFileName.c_str(), "r");
	
	if (f == NULL)
	{
		return false;
	}
	
	fclose(f);
	
	return true;
}



std::string GetFilePathUserData(const std::string& filename)
{
#ifdef TARGET_WIN32
	std::string realFileName = filename;
#endif
#ifdef TARGET_ANDROID
	std::string realFileName = JniGetApplicationDir()+"/" + filename;
#endif
#ifdef TARGET_IOS
	std::string realFileName = IosGetFilePathUserData(filename);
#endif

	return realFileName;
}


#ifdef TARGET_WIN32
void GetFileList(const std::string& searchkey, std::vector<std::string> &list)
{
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(searchkey.c_str(), &fd);

    if(h == INVALID_HANDLE_VALUE)
    {
        return;
    }

    while(1)
    {

        list.push_back(fd.cFileName);

        if(FindNextFile(h, &fd) == FALSE)
            break;
    }
}
#endif

#ifdef TARGET_IOS
//Special for IOS -> Foundation.h conflicts with sq_plus.h
std::string GetPathToResources()
{
    return ST::PathToResources;
}
#endif



} //namespace SE
