#ifndef SOUND_MANAGER_DATA_TYPES_H_INCLUDED
#define SOUND_MANAGER_DATA_TYPES_H_INCLUDED

#ifndef NOSOUND


#ifdef TARGET_ANDROID
//#include <asm/page.h>
//#include <limits.h>
#endif

#include "boost/thread.hpp"
#include "boost/smart_ptr.hpp"
#include "boost/bind.hpp"

#include <stdio.h>

#include <queue>


#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"


namespace SE
{

typedef boost::shared_array<char> TByteArrPtr;



struct TBasicWAVEHeader 
{    
    char Riff[4];//'RIFF'
    unsigned int RiffSize;
    char Wave[4];//'WAVE'
    char Fmt[4];//'fmt '
    unsigned int FmtSize;
    unsigned short Format;
    unsigned short Channels;
    unsigned int SamplesPerSec;
    unsigned int BytesPerSec;
    unsigned short BlockAlign;
    unsigned short BitsPerSample;
    char  Data[4];//'data'
    unsigned int DataSize;
};




struct TOggCallbackFileBuffer
{
    size_t FileSize; 
    size_t FilePos; 
    TByteArrPtr FileData;
    
    TOggCallbackFileBuffer(const TByteArrPtr& fileData, size_t fileSize)
        : FileData(fileData)
        , FileSize(fileSize)
        , FilePos(0)
    {
    }
    
    TOggCallbackFileBuffer()
        : FileData(NULL) //dummy
        , FileSize(0)
        , FilePos(0)
    {
    }
    
    TOggCallbackFileBuffer(const TOggCallbackFileBuffer& copyFrom)
    {
        FileSize = copyFrom.FileSize;
        FilePos = copyFrom.FilePos;
        FileData = copyFrom.FileData;
    }
    
    TOggCallbackFileBuffer& operator=(const TOggCallbackFileBuffer& copyFrom)
    {
        if (&copyFrom == this)
        {
            return *this;
        }
        
        FileSize = copyFrom.FileSize;
        FilePos = copyFrom.FilePos;
        FileData = copyFrom.FileData;
        
        return *this;
    }
    
};




extern ov_callbacks OggCallbacks;

} //namespace SE

#endif


#endif
