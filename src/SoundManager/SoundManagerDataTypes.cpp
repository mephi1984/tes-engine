#ifndef NOSOUND

#include "include/SoundManager/SoundManagerDataTypes.h"


namespace SE
{


extern "C"
{ 
size_t OggFileRead(void* toBuf, size_t varsize, size_t varcount, void* fromBuf);

int OggFileSeek(void *fromBuf, ogg_int64_t offset, int whence);

long OggFileTell(void *fromBuf);
}

size_t OggFileRead(void* toBuf, size_t varsize, size_t varcount, void* fromBuf)
{
    TOggCallbackFileBuffer& callbackFileBuffer = *(reinterpret_cast<TOggCallbackFileBuffer*>(fromBuf));
    
    if (callbackFileBuffer.FilePos+varsize*varcount > callbackFileBuffer.FileSize)
    {
        size_t dataLeft = callbackFileBuffer.FileSize - callbackFileBuffer.FilePos;
        
        if (dataLeft != 0)
        {
            memcpy(toBuf, &callbackFileBuffer.FileData[callbackFileBuffer.FilePos], dataLeft);
        }
        
        callbackFileBuffer.FilePos += dataLeft;
  
        return dataLeft;
    }
    else
    {
        memcpy(toBuf, &callbackFileBuffer.FileData[callbackFileBuffer.FilePos], varsize*varcount);
        callbackFileBuffer.FilePos += varsize*varcount;
 
        return varsize*varcount;
    }
}



int OggFileSeek(void *fromBuf, ogg_int64_t offset, int whence)
{
    TOggCallbackFileBuffer& callbackFileBuffer = *(reinterpret_cast<TOggCallbackFileBuffer*>(fromBuf));

    size_t newPos;
    
    switch(whence)
    {
case SEEK_SET:
        newPos = static_cast<size_t>(offset);
        break;
case SEEK_CUR:
        newPos = static_cast<size_t>(callbackFileBuffer.FilePos + offset);
        break;
case SEEK_END:
        newPos = static_cast<size_t>(callbackFileBuffer.FileSize - 1 - offset);
        break;
	};
    
    callbackFileBuffer.FilePos = newPos;
    
    return 0;
    
}

long OggFileTell(void *fromBuf)
{
    TOggCallbackFileBuffer& callbackFileBuffer = *(reinterpret_cast<TOggCallbackFileBuffer*>(fromBuf));
    
    return callbackFileBuffer.FilePos;
}



ov_callbacks OggCallbacks = {
  (size_t (*)(void *, size_t, size_t, void *))  OggFileRead,
  (int (*)(void *, ogg_int64_t, int))           OggFileSeek,
  (int (*)(void *))                             NULL,
  (long (*)(void *))                            OggFileTell
};


#endif //NOSOUND


} //namespace SE