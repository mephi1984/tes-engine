#ifndef NOSOUND
#include "include/SoundManager/SoundManagerWindows.h"

#include "include/Engine.h"

namespace SE
{

TWaveFile OggSoundFromFile(const std::string& fileName)
{

    
    cardinal fileSize;
    TByteArrPtr fileData = CreateMemFromFile<char>(fileName, fileSize);
    
    OggVorbis_File testOggFile;
    
    TOggCallbackFileBuffer oggBuffer(fileData, fileSize);
 
    if(ov_open_callbacks(&oggBuffer, &testOggFile, NULL, 0, OggCallbacks) < 0) 
        throw ErrorToLog("Error! Input does not appear to be an Ogg bitstream: "+fileName+"\n");

    vorbis_info *pInfo;
    
    pInfo = ov_info(&testOggFile, -1);
    
    int channels = 0;
    
    
    channels = pInfo->channels;
    
    long freq;
    freq = pInfo->rate;
    
    std::vector<char> bufferData;
    
    const int BUFFER_SIZE = 32768;
    int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[BUFFER_SIZE];

    do 
    {
        bytes = ov_read(&testOggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
        bufferData.insert(bufferData.end(), array, array + bytes);
    } while (bytes > 0);
    
    //delete [] fileData;
    
    TBasicWAVEHeader soundHeader;
    
    soundHeader.SamplesPerSec = freq;
    soundHeader.DataSize = static_cast<unsigned short>(bufferData.size());
    soundHeader.BitsPerSample = 16;
    soundHeader.Channels = channels;

	//Just in case...
	//soundHeader.
    
    ov_clear(&testOggFile);
 
    char* rawBufferData = new char[bufferData.size()];
    memcpy(rawBufferData, &bufferData[0], bufferData.size());
    
	TWaveFile result;

	result.WaveHeader = soundHeader;
	result.WaveData = TByteArrPtr(rawBufferData);
	result.WaveDataCursor = 0;


    return result;
    
}


TWaveFile LoadWaveFile(const std::string& fileName)
{
	TWaveFile waveFile;

	cardinal byteCount;

	boost::shared_array<char> filePtr = CreateMemFromFile<char>(fileName.c_str(), byteCount);

    if (byteCount <= sizeof(TBasicWAVEHeader))
        throw ErrorToLog("Invalid wave file: "+fileName+"!!");
    
    //Possibly not aligned???
    waveFile.WaveHeader = *(reinterpret_cast<TBasicWAVEHeader*>(&filePtr[0]));
    
    if (byteCount < sizeof(TBasicWAVEHeader) + waveFile.WaveHeader.DataSize)
        throw ErrorToLog("Wave file too short: "+fileName+"!!");


	waveFile.WaveData = TByteArrPtr(new char [waveFile.WaveHeader.DataSize]);
    
    memcpy(&(waveFile.WaveData[0]), &filePtr[0] + sizeof(TBasicWAVEHeader), waveFile.WaveHeader.DataSize);

	waveFile.WaveDataCursor = 0;

	return waveFile;
}


DSBUFFERDESC FillPrimaryWaveBufferDescription()
{
	DSBUFFERDESC bufferDesc;

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	return bufferDesc;
}

WAVEFORMATEX FillPrimaryWaveFormat()
{

	WAVEFORMATEX waveFormat;

	// Setup the format of the primary sound buffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	return waveFormat;
}

DSBUFFERDESC FillSecondaryWaveBufferDescription(cardinal bufferSize, WAVEFORMATEX* pWaveFormat)
{
	DSBUFFERDESC bufferDesc;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = bufferSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = pWaveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	return bufferDesc;
}

WAVEFORMATEX FillSecondaryWaveFormat(cardinal samplesPerSec)
{
	WAVEFORMATEX waveFormat;

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	
	waveFormat.nSamplesPerSec = samplesPerSec;
	
	waveFormat.wBitsPerSample = 16;
	
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	return waveFormat;
}


//============================================
//============================================
//============================================



void TMusicDataAccessInterface::ResetDataCursor()
{
	goneOverEnd = false;
	InnerResetDataCursor();
}

void TMusicDataAccessInterface::ReadDataToBuffer_Looped(char* buffer, int size)
{
	int totalbytesRead = 0;
	int bytesRead;
	int sizeLeft = size;

	while(totalbytesRead < size)
	{
		InnerReadDataToBuffer(&buffer[totalbytesRead], sizeLeft, bytesRead);
		totalbytesRead += bytesRead;
		sizeLeft -= bytesRead;
	}

}


int TMusicDataAccessInterface::ReadDataToBuffer(char* buffer, int size)
{
	int result;
	int totalbytesRead = 0;
	int bytesRead;
	int sizeLeft = size;

	while(totalbytesRead < size && (!goneOverEnd))
	{
		result = InnerReadDataToBuffer(&buffer[totalbytesRead], sizeLeft, bytesRead);

		goneOverEnd = (result == -1);

		totalbytesRead += bytesRead;
		sizeLeft -= bytesRead;
	}

	if (goneOverEnd)
	{
		for (int i = totalbytesRead; i < size; i++)
		{
			buffer[i] = 0x00;
		}
	}

	if (goneOverEnd)
	{
		return -1;
	}
	else
	{
		return 1;
	}

}

//============================================
//============================================
//============================================



TMusicStreamAncestor::TMusicStreamAncestor()
	: IsPlaying(false)
	, Looped(false)
	, BufferCursorPos(0)
	, PlayCursorPos(0)
	, TotalPlayCursorPos(0)
	, IsLoaded(false)
{
}


TMusicStreamAncestor::~TMusicStreamAncestor()
{
}


void TMusicStreamAncestor::Load(const std::string& musicFileName)
{

	std::string fileName = musicFileName;

	//MusicFile = LoadWaveFile(fileName);
	LoadSoundData(fileName);

	DirectSoundBuffer.soundBuffer = ResourceManager->SoundManager.CreateDirectSound8InterfaceBuffer(CONST_DIRECTSOUND_BUFFER_SIZE, GetSamplesPerSec());

	InitialFillBuffer();

	BufferCursorPos = 0;
	PlayCursorPos = 0;
	TotalPlayCursorPos = 0;

	IsLoaded = true;
}

void TMusicStreamAncestor::Clear()
{
	if (IsLoaded)
	{
		ClearSoundData();

		DirectSoundBuffer.soundBuffer->Release();
		DirectSoundBuffer.soundBuffer = NULL;

		BufferCursorPos = 0;
		PlayCursorPos = 0;
		TotalPlayCursorPos = 0;

		IsLoaded = false;
	}
}



void TMusicStreamAncestor::Update()
{
	
	if (!IsPlaying)
	{
		return;
	}

	cardinal readPosition;
	HRESULT r;

	r = DirectSoundBuffer.soundBuffer->GetCurrentPosition(reinterpret_cast<DWORD*>(&readPosition), NULL);

	if (r != DS_OK)
	{
		throw ErrorToLog("Error in TWaveMusicStruct::Update");
	}


	if (readPosition >= PlayCursorPos)
	{
		TotalPlayCursorPos += (readPosition - PlayCursorPos);
	}
	else
	{
		TotalPlayCursorPos += (CONST_DIRECTSOUND_BUFFER_SIZE - PlayCursorPos) + readPosition;
	}

	PlayCursorPos = readPosition;

	if (CheckIfSoundIsOver() && !Looped)
	{
		Stop();
		return;
	}

	
	char* bufferPtr1;
	char* bufferPtr2;
	cardinal bufferSize1, bufferSize2;

	cardinal size = GetMaxWriteSize(BufferCursorPos, CONST_DIRECTSOUND_BUFFER_SIZE, DirectSoundBuffer);

	if (size == 0)
	{
		return;
	}

	r = DirectSoundBuffer.soundBuffer->Lock(BufferCursorPos, size, reinterpret_cast<void**>(&bufferPtr1), reinterpret_cast<DWORD*>(&bufferSize1), reinterpret_cast<void**>(&bufferPtr2), reinterpret_cast<DWORD*>(&bufferSize2), 0);
	
	if (r != DS_OK)
	{
		throw ErrorToLog("Error in UpdateBufferWithWaveFile");
	}

	if (Looped)
	{
		ReadDataToBuffer_Looped(bufferPtr1, bufferSize1);
	}
	else
	{
		ReadDataToBuffer(bufferPtr1, bufferSize1);
	}

	if (bufferPtr2 != NULL)
	{
		if (Looped)
		{
			ReadDataToBuffer_Looped(bufferPtr2, bufferSize2);
		}
		else
		{
			ReadDataToBuffer(bufferPtr2, bufferSize2);
		}
	}

	BufferCursorPos = (BufferCursorPos + bufferSize1 + bufferSize2) % CONST_DIRECTSOUND_BUFFER_SIZE;

	r = DirectSoundBuffer.soundBuffer->Unlock(reinterpret_cast<void*>(bufferPtr1), (bufferSize1), reinterpret_cast<void*>(bufferPtr2), (bufferSize2));

	if(FAILED(r))
	{
		throw ErrorToLog("Error in UpdateBufferWithWaveFile");
	}
}


void TMusicStreamAncestor::Play()
{
	if (!IsLoaded)
	{
		return;
	}
	Looped = false;
	InnerPlay();
}

void TMusicStreamAncestor::PlayLooped()
{
	if (!IsLoaded)
	{
		return;
	}
	Looped = true;
	InnerPlay();
}

void TMusicStreamAncestor::Stop()	
{
	if (!IsLoaded)
	{
		return;
	}

	HRESULT r;

	IsPlaying = false;
	
	ResetDataCursor();

	BufferCursorPos = 0;
	PlayCursorPos = 0;
	TotalPlayCursorPos = 0;

	r = DirectSoundBuffer.soundBuffer->Stop();
	
	r = DirectSoundBuffer.soundBuffer->SetCurrentPosition(0);

	if (r != DS_OK)
	{
		throw ErrorToLog("Error in TWaveMusicStruct::Stop");
	}


	if (r != DS_OK)
	{
		throw ErrorToLog("Error in TWaveMusicStruct::Stop");
	}

	InitialFillBuffer();

}



void TMusicStreamAncestor::InnerPlay()
{
	if (IsPlaying)
	{
		return;
	}

	HRESULT result;

	// Set position at the beginning of the sound buffer.
	result = DirectSoundBuffer.soundBuffer->SetCurrentPosition(0);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}
 
	// Set volume of the buffer to 100%.
	result = DirectSoundBuffer.soundBuffer->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}
 
	// Play the contents of the secondary sound buffer.
	result = DirectSoundBuffer.soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}


	IsPlaying = true;
}

void TMusicStreamAncestor::InitialFillBuffer()
{
	HRESULT r;

	cardinal bufferSize;

	char *bufferPtr;

	r = DirectSoundBuffer.soundBuffer->Lock(0, CONST_DIRECTSOUND_BUFFER_SIZE, reinterpret_cast<void**>(&bufferPtr), reinterpret_cast<DWORD*>(&bufferSize), NULL, 0, 0);
	
	if(FAILED(r))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}

	if (ReadDataToBuffer(bufferPtr, CONST_DIRECTSOUND_BUFFER_SIZE) != 1)
	{
		throw ErrorToLog("Assert on LoadAndPlaySound");
	}

	// Unlock the secondary buffer after the data has been written to it.
	r = DirectSoundBuffer.soundBuffer->Unlock(reinterpret_cast<void*>(bufferPtr), bufferSize, NULL, 0);
	
	if(FAILED(r))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}

}


//===============================================
//===============================================
//===============================================


TSoundManagerWindows::TSoundManagerWindows() 
{ 
	InitDirectSound();
}


TSoundManagerWindows::~TSoundManagerWindows() 
{
	SoundMap.clear();
	ShutdownDirectSound();
}

void TSoundManagerWindows::InitDirectSound()
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;
 
	// Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &DirectSound, NULL);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in InitDirectSound");
	}
 
	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = DirectSound->SetCooperativeLevel(Hwnd, DSSCL_PRIORITY); //Hwnd is global variable defined in HalibutEngineWindows
	if(FAILED(result))
	{
		throw ErrorToLog("Error in InitDirectSound");
	}

	// Setup the primary buffer description.
	bufferDesc = FillPrimaryWaveBufferDescription();
 
	// Get control of the primary sound buffer on the default sound device.
	result = DirectSound->CreateSoundBuffer(&bufferDesc, &PrimaryBuffer, NULL);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in InitDirectSound");
	}

	waveFormat = FillPrimaryWaveFormat();
 
	// Set the primary buffer to be the wave format specified.
	result = PrimaryBuffer->SetFormat(&waveFormat);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in InitDirectSound");
	}

}

void TSoundManagerWindows::ShutdownDirectSound()
{
	// Release the primary sound buffer pointer.
	if(PrimaryBuffer)
	{
		PrimaryBuffer->Release();
		PrimaryBuffer = 0;
	}
 
	// Release the direct sound interface pointer.
	if(PrimaryBuffer)
	{
		PrimaryBuffer->Release();
		PrimaryBuffer = 0;
	}
 
}



IDirectSoundBuffer8* TSoundManagerWindows::CreateDirectSound8InterfaceBuffer(cardinal dataSize, cardinal samplesPerSec)
{
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;

	IDirectSoundBuffer8* soundBuffer;


	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat = FillSecondaryWaveFormat(samplesPerSec);
 
	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc = FillSecondaryWaveBufferDescription(dataSize, &waveFormat);

	// Create a temporary sound buffer with the specific buffer settings.
	result = DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in CreateDirectSound8InterfaceBuffer");
	}
 
	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<void**>(&soundBuffer));


	if(FAILED(result))
	{
		throw ErrorToLog("Error in CreateDirectSound8InterfaceBuffer");
	}
 
	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	return soundBuffer;
}



void TSoundManagerWindows::LoadSound(const std::string& soundFileName)
{
	std::string soundName = std::string(GetFileName(soundFileName.c_str()));

	if (SoundMap.count(soundName) != 0)
	{
		return;
	}

	HRESULT result;

	std::string fileName = ST::PathToResources + soundFileName;


	TWaveFile waveFile;
	
	if (GetFileExt(fileName) == ".ogg") 
	{
		waveFile = OggSoundFromFile(fileName);
	}
	else if (GetFileExt(fileName) == ".wav")
	{
		waveFile = LoadWaveFile(fileName);
	}
	else
	{
		throw ErrorToLog("Unknown extension for sound for file: "+tostr(fileName));
	}

	
	SoundMap[soundName].soundBuffer = CreateDirectSound8InterfaceBuffer(waveFile.WaveHeader.DataSize, waveFile.WaveHeader.SamplesPerSec);

	cardinal bufferSize;

	unsigned char *bufferPtr;

	result = SoundMap[soundName].soundBuffer->Lock(0, waveFile.WaveHeader.DataSize, reinterpret_cast<void**>(&bufferPtr), reinterpret_cast<DWORD*>(&bufferSize), NULL, 0, 0);
	
	if(FAILED(result))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}
 
	// Copy the wave data into the buffer.
	memcpy(bufferPtr, &(waveFile.WaveData[0]), waveFile.WaveHeader.DataSize);
 
	// Unlock the secondary buffer after the data has been written to it.
	result = SoundMap[soundName].soundBuffer->Unlock(reinterpret_cast<void*>(bufferPtr), bufferSize, NULL, 0);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}

}


void TSoundManagerWindows::PlaySound(const std::string& soundName)
{

	if (SoundMap.count(soundName) == 0)
	{
		return;
	}

	HRESULT result;


	// Set position at the beginning of the sound buffer.
	result = SoundMap[soundName].soundBuffer->SetCurrentPosition(0);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}
 
	// Set volume of the buffer to 100%.
	result = SoundMap[soundName].soundBuffer->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}
 
	// Play the contents of the secondary sound buffer.
	result = SoundMap[soundName].soundBuffer->Play(0, 0, 0);
	if(FAILED(result))
	{
		throw ErrorToLog("Error in LoadAndPlaySound");
	}
}


void TSoundManagerWindows::PlayMusic(const std::string& musicName)
{
	if (StreamMap.count(musicName) != 0)
	{
		StreamMap[musicName]->Play();
	}
}

void TSoundManagerWindows::PlayMusicLooped(const std::string& musicName)
{
	if (StreamMap.count(musicName) != 0)
	{
		StreamMap[musicName]->PlayLooped();
	}
}

void TSoundManagerWindows::LoadMusic(const std::string& musicFileName)
{
	std::string musicName = std::string(GetFileName(musicFileName.c_str()));

	if (StreamMap.count(musicName) == 0)
	{

		boost::shared_ptr<TMusicStreamAncestor> ptr;

		if (std::string(GetFileExt(musicName.c_str())) == ".wav")
		{
			ptr = boost::shared_ptr<TMusicStreamAncestor>(new TWaveStream);
		}
		else if (std::string(GetFileExt(musicName.c_str())) == ".ogg")
		{
			ptr = boost::shared_ptr<TMusicStreamAncestor>(new TOggMusicStream);
		}
		
		StreamMap[musicName] = ptr;

		StreamMap[musicName]->Load(ST::PathToResources + musicFileName);
	}
}

void TSoundManagerWindows::StopMusic(const std::string& musicName)
{
	if (StreamMap.count(musicName) != 0)
	{
		StreamMap[musicName]->Stop();
	}
}

void TSoundManagerWindows::StopAllMusic()
{
	for (std::map<std::string, boost::shared_ptr<TMusicStreamAncestor> >::iterator i = StreamMap.begin(); i != StreamMap.end(); ++i)
	{
		i->second->Stop();
	}
}

void TSoundManagerWindows::TryStopAndPlayMusicLooped(const std::string& musicName)
{
	for (std::map<std::string, boost::shared_ptr<TMusicStreamAncestor> >::iterator i = StreamMap.begin(); i != StreamMap.end(); ++i)
	{
		if (i->first != musicName)
		{
			i->second->Stop();
		}
	}

	if (StreamMap.count(musicName) != 0)
	{
		StreamMap[musicName]->PlayLooped();
	}


	
}

DWORD GetMaxWriteSize(cardinal m_cbBufOffset, cardinal m_cbBufSize, TDirectSoundStruct& soundStruct)
{
  DWORD dwWriteCursor, dwPlayCursor, dwMaxSize;

  // Get current play position
  if (soundStruct.soundBuffer->GetCurrentPosition (&dwPlayCursor, &dwWriteCursor) == DS_OK)
  {
        if (m_cbBufOffset <= dwPlayCursor)
        {
        // Our write position trails play cursor
        dwMaxSize = dwPlayCursor - m_cbBufOffset;
        }

        else // (m_cbBufOffset > dwPlayCursor)
        {
        // Play cursor has wrapped
        dwMaxSize = m_cbBufSize - m_cbBufOffset + dwPlayCursor;
        }
  }
  else
  {
        throw ErrorToLog("Error in GetMaxWriteSize");
        dwMaxSize = 0;
  }
  return dwMaxSize;
}

void FillMemoryWithZero(char* bufferPtr, cardinal bufferSize)
{
	for (cardinal i=0; i<bufferSize; i++)
	{
		bufferPtr[i] = 0x00;
	}
}


void TSoundManagerWindows::Update(cardinal dt)
{
	std::map<std::string, boost::shared_ptr<TMusicStreamAncestor> >::iterator i;

	for (i = StreamMap.begin(); i != StreamMap.end(); ++i)
	{
		i->second->Update();
	}
}


//================================================================
//================================================================
//================================================================

TWaveStream::TWaveStream()
{
	ResetDataCursor();
}


TWaveStream::~TWaveStream()
{
	Clear();
}

int TWaveStream::InnerReadDataToBuffer(char* buffer, int size, int& bytesRead)
{
	int result = 1;

	if (size + MusicFile.WaveDataCursor > MusicFile.WaveHeader.DataSize)
	{
		bytesRead = MusicFile.WaveHeader.DataSize - MusicFile.WaveDataCursor;
	}
	else
	{
		bytesRead = size;
	}

	memcpy(buffer, &(MusicFile.WaveData[MusicFile.WaveDataCursor]), bytesRead);

	MusicFile.WaveDataCursor += size;

	while (MusicFile.WaveDataCursor >= MusicFile.WaveHeader.DataSize)
	{
		MusicFile.WaveDataCursor -= MusicFile.WaveHeader.DataSize;
		result = -1;
	}

	return result;


}

void TWaveStream::InnerResetDataCursor()
{
	MusicFile.WaveDataCursor = 0;
}

void TWaveStream::ClearSoundData()
{
	MusicFile.WaveData = TByteArrPtr(NULL);
	MusicFile.WaveHeader.DataSize = 0;
	MusicFile.WaveDataCursor = 0;
}

void TWaveStream::LoadSoundData(const std::string& fileName)
{
	MusicFile = LoadWaveFile(fileName);
}

bool TWaveStream::CheckIfSoundIsOver()
{
	return (TotalPlayCursorPos >= MusicFile.WaveHeader.DataSize);
}

cardinal TWaveStream::GetSamplesPerSec()
{
	return MusicFile.WaveHeader.SamplesPerSec;
}


//=============================================
//=============================================
//=============================================


TOggMusicStream::TOggMusicStream()
{
	IsInited = false;
	ResetDataCursor();
}


TOggMusicStream::~TOggMusicStream()
{
	Clear();
}

int TOggMusicStream::InnerReadDataToBuffer(char* buffer, int size, int& bytesRead)
{
	int result;
    int  section;
    
	bytesRead = ov_read(&OggFileData, buffer, size, 0, CONST_BPS, 1, &section);

	if (bytesRead == -1)
	{
		throw ErrorToLog("Error!");
	}
	else if (bytesRead == 0)
	{

		if (ov_raw_seek(&(OggFileData), 0))
        {
			throw ErrorToLog("Error on call ov_raw_seek!");
        }

		result = -1;
	}
	else
	{
		result = 1;
	}


	return result;
	
}

void TOggMusicStream::InnerResetDataCursor()
{
	if (!IsInited)
	{
		return;
	}

	if (ov_raw_seek(&(OggFileData), 0))
    {
		throw ErrorToLog("Error on call ov_raw_seek!");
    }

}

void TOggMusicStream::ClearSoundData()
{
	
	FileBuffer = TOggCallbackFileBuffer();
   
	ov_clear(&OggFileData);


    VorbisInfo = NULL;

	IsInited = false;

}

void TOggMusicStream::LoadSoundData(const std::string& fileName)
{
    cardinal fileSize;
    
    TByteArrPtr fileArr = CreateMemFromFile<char>(fileName, fileSize);
    
    FileBuffer = TOggCallbackFileBuffer(fileArr, fileSize);

    if(ov_open_callbacks(&FileBuffer, &OggFileData, NULL, 0, OggCallbacks) < 0) 
        throw ErrorToLog("Error! Input does not appear to be an Ogg bitstream: "+fileName);
    
    VorbisInfo = ov_info(&OggFileData, -1);

	IsInited = true;
}

bool TOggMusicStream::CheckIfSoundIsOver()
{
	cardinal r = static_cast<cardinal>(2 * CONST_BPS * ov_pcm_total(&OggFileData, -1));
	if (OV_EINVAL == r)
	{
		throw ErrorToLog("Ogg error!");
	}


	return (TotalPlayCursorPos >= r);
}


cardinal TOggMusicStream::GetSamplesPerSec()
{
	return VorbisInfo->rate;
}


#endif //NOSOUND


} //namespace SE