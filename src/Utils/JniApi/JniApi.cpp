#include "include/Utils/JniApi/JniApi.h"

#include "include/Utils/Utils.h"

namespace SE
{

JavaVM* JavaVirtualMachine = 0;

const std::string CONST_JAVA_FILESYSTEM_CLASS_NAME = "fishrungames/engine/FileWrapper";

//Private data

//All 3 deprecated
cardinal* FileArr = NULL;
int FileSize = 0;
int filePointer;

std::string ApkFilePath;

//Private methods
jclass JniTryFindClass(const std::string& className);
jmethodID JniTryFindStaticMethod(jclass clazz, const std::string& methodId, const std::string methodSignature);
void CheckCommonExceptionWithMessage(const std::string& message);

//Deprecated
void JniCallOpenFile(const std::string& fileName);


//==================================================
//=============== IMPLEMENTATION ===================
//==================================================


JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_SetupEnviroment(JNIEnv* env, jobject thiz)
{
    int JVMResult;
    JVMResult = env->GetJavaVM(&JavaVirtualMachine);
    
    //TODO: Check if jmvresult != 0
}

JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_SetupApkFilePath(JNIEnv* env, jobject thiz, jstring s)
{

	const char *nativeString = env->GetStringUTFChars(s, 0);
	
	ApkFilePath = std::string(nativeString);
	
   env->ReleaseStringUTFChars(s, nativeString);

}

JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_ConsoleOut(JNIEnv* env, jobject thiz, jstring s)
{
	const char *nativeString = env->GetStringUTFChars(s, 0);

   *Console<<std::string("OUT OF JAVA: ")+nativeString;

   env->ReleaseStringUTFChars(s, nativeString);

}


//Deprecated
JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_CreateFile(JNIEnv* env, jobject thiz, int fileSize)
{
	FileSize = fileSize;
	FileArr =  new cardinal [fileSize % 4 == 0 ? fileSize/4 : fileSize/4 + 1];
	filePointer = 0;
}
	
	
//Deprecated
JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_WriteToFile(JNIEnv* env, jobject thiz, jbyteArray buffer, int bufferSize)
{
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);
	jenv->GetByteArrayRegion(buffer, 0, bufferSize, (jbyte*)((char*)FileArr + filePointer));
	filePointer += bufferSize;
	
}

//===================================================
//============== Private methods ====================
//===================================================

jclass JniTryFindClass(const std::string& className)
{
    
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);
    
	jclass clazz = jenv->FindClass(className.c_str());
	
	
	if (jenv->ExceptionOccurred() != 0)
	{
		jenv->ExceptionClear();
		throw ErrorToLog("JNI: failed to find class: "+className);
	} 

    return clazz;
}

jmethodID JniTryFindStaticMethod(jclass clazz, const std::string& methodId, const std::string methodSignature)
{
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jmethodID mid = jenv->GetStaticMethodID(clazz, methodId.c_str(), methodSignature.c_str());
	
	if (jenv->ExceptionOccurred() != 0)
	{
		jenv->ExceptionClear(); 
		throw ErrorToLog("JNI: failed to find static method: "+methodId+" with signature: "+methodSignature);
	}
	
	return mid;
}

void CheckCommonExceptionWithMessage(const std::string& message)
{
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);
    
	if (jenv->ExceptionOccurred() != 0)
	{
		jenv->ExceptionClear(); 
		throw ErrorToLog(message);
	}
}


//Deprecated
void JniCallOpenFile(const std::string& fileName)
{
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

	jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);

    jmethodID mid = JniTryFindStaticMethod(clazz, "OpenFile", "(Ljava/lang/String;)I");

	jstring jstr = jenv->NewStringUTF(fileName.c_str());


	CheckCommonExceptionWithMessage("Fail in JniCallOpenFile\n");
	
	//Any problem? - see console log
	if (jenv->CallStaticIntMethod(clazz, mid, jstr) != 1)
		throw ErrorToLog("ERROR: Could not open file: " + fileName);
}


std::string JniGetApplicationDir()
{
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);
    
	jmethodID mid = JniTryFindStaticMethod(clazz, "GetApplicationDir", "()Ljava/lang/String;");
	
	jstring jstr = (jstring)jenv->CallStaticObjectMethod(clazz, mid);
	
	const char *nativeString = jenv->GetStringUTFChars(jstr, 0);
    
    std::string s(nativeString);

    jenv->ReleaseStringUTFChars(jstr, nativeString);
    
    return s;

}


void JniLoadSound(const std::string& fileName)
{
    
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);
    
	jmethodID mid = JniTryFindStaticMethod(clazz, "LoadSound", "(Ljava/lang/String;)V");
	
	jstring jstr = jenv->NewStringUTF(fileName.c_str());

	CheckCommonExceptionWithMessage("Fail in JniLoadSound");
	
	//Any problem? - see console log
	jenv->CallStaticVoidMethod(clazz, mid, jstr);
	
}

void JniPlaySound(const std::string& soundName)
{
    
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);
    
	jmethodID mid = JniTryFindStaticMethod(clazz, "PlaySound", "(Ljava/lang/String;)V");
	
	jstring jstr = jenv->NewStringUTF(soundName.c_str());

	CheckCommonExceptionWithMessage("Fail in JniPlaySound");
	
	//Any problem? - see console log
	jenv->CallStaticVoidMethod(clazz, mid, jstr);
}

void JniPlayMusic(const std::string& fileName)
{
	*Console<<"In play music\n";
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);
    
	jmethodID mid = JniTryFindStaticMethod(clazz, "PlayMusic", "(Ljava/lang/String;)V");
	
	jstring jstr = jenv->NewStringUTF(fileName.c_str());

	CheckCommonExceptionWithMessage("Fail in JniPlayMusic");
	
	//Any problem? - see console log
	jenv->CallStaticVoidMethod(clazz, mid, jstr);
}


void JniPlayMusicLooped(const std::string& fileName)
{
    JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);
    
	jmethodID mid = JniTryFindStaticMethod(clazz, "PlayMusicLooped", "(Ljava/lang/String;)V");
	
	jstring jstr = jenv->NewStringUTF(fileName.c_str());

	CheckCommonExceptionWithMessage("Fail in JniPlayMusicLooped");
	
	//Any problem? - see console log
	jenv->CallStaticVoidMethod(clazz, mid, jstr);
}


void JniStopMusic()
{
	JNIEnv* jenv;
	
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);
    
	jmethodID mid = JniTryFindStaticMethod(clazz, "StopMusic", "()V");

	CheckCommonExceptionWithMessage("Fail in JniPlayMusicLooped");
	
	//Any problem? - see console log
	jenv->CallStaticVoidMethod(clazz, mid);
}

void JniReleaseAllSoundsAndMusic()
{
	JNIEnv* jenv;
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);
    
	jmethodID mid = JniTryFindStaticMethod(clazz, "ReleaseAllSoundsAndMusic", "()V");

	CheckCommonExceptionWithMessage("Fail in JniReleaseAllSoundsAndMusic");
	
	//Any problem? - see console log
	jenv->CallStaticVoidMethod(clazz, mid);
}

void JniPauseAllSoundsAndMusic()
{
	JNIEnv* jenv;
	
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);
    
	jmethodID mid = JniTryFindStaticMethod(clazz, "PauseAllSoundsAndMusic", "()V");

	CheckCommonExceptionWithMessage("Fail in JniPauseAllSoundsAndMusic");
	
	//Any problem? - see console log
	jenv->CallStaticVoidMethod(clazz, mid);
}

void JniResumeAllSoundsAndMusic()
{
	JNIEnv* jenv;
	
    JavaVirtualMachine->GetEnv((void**)&jenv, JNI_VERSION_1_4);

    jclass clazz = JniTryFindClass(CONST_JAVA_FILESYSTEM_CLASS_NAME);
    
	jmethodID mid = JniTryFindStaticMethod(clazz, "ResumeAllSoundsAndMusic", "()V");

	CheckCommonExceptionWithMessage("Fail in JniResumeAllSoundsAndMusic");
	
	//Any problem? - see console log
	jenv->CallStaticVoidMethod(clazz, mid);
}



void PrintApkContent()
{
	zip* APKArchive = zip_open(ApkFilePath.c_str(), 0, NULL);
    if (APKArchive == NULL)
	{
		throw ErrorToLog("Error in loadAPK");
	}
	
	//Just for debug, print APK contents
    int numFiles = zip_get_num_files(APKArchive);
	
    for (int i=0; i<numFiles; i++)
	{
		const char* name = zip_get_name(APKArchive, i, 0);
        if (name == NULL)
		{
			*Console<<"Error reading zip file name at index "+tostr(i)+" "+std::string(name);
			throw ErrorToLog("Error in loadAPK");
		}
		
		*Console<<"Ok reading zip file name at index "+tostr(i)+" "+std::string(name);
	}
	
	zip_close(APKArchive);
}



void LoadApk(const std::string& fileName, std::vector<char>& returnData)
{
	
	//PrintApkContent();
	
	if (ApkFilePath == "")
	{
		throw ErrorToLog("Apk file path is not set!");
	}
	
	//*Console<<"ApkFilePath = "+ApkFilePath+"\n";
	
    zip* APKArchive = zip_open(ApkFilePath.c_str(), 0, NULL);
	
    if (APKArchive == NULL)
	{
		throw ErrorToLog("Error in loadAPK, apk archive is not valid");
	}
  
    zip_file *zfile = zip_fopen(APKArchive, ("assets/" + fileName).c_str(), 0);
	
    char buf[4096];

    if (zfile != NULL)
    {
        size_t bytesRead;
        do
        {
            bytesRead = zip_fread(zfile, &buf[0], 4096);
            returnData.insert(returnData.end(), buf, buf + bytesRead);
        }
        while (bytesRead == 4096);
        zip_fclose(zfile);
    }
	else
    {
        throw ErrorToLog("Error in loadAPK, data in apk archive is broken: "+fileName);
    }
   
    zip_close(APKArchive);
    
}


} //namespace SE