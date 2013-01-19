#ifndef SOUND_MANAGER_WINDOWS_H_INCLUDED
#define SOUND_MANAGER_WINDOWS_H_INCLUDED

#include "include/SoundManager/SoundManagerDataTypes.h"
#include "include/SoundManager/SoundManagerInterface.h"
#include "include/Utils/Utils.h"


#ifndef NOSOUND
#include "audiodefs.h"
#include "dsound.h"

namespace SE
{


const cardinal CONST_DIRECTSOUND_BUFFER_SIZE = 4096*16;

const cardinal CONST_BPS = 2;

struct TDirectSoundStruct
{
	IDirectSoundBuffer8* soundBuffer;

	TDirectSoundStruct() : soundBuffer(NULL) { }

	~TDirectSoundStruct()
	{
		if (soundBuffer != NULL)
		{
			soundBuffer->Release();
			soundBuffer = NULL;
		}
	}
};


struct TWaveFile
{
	TBasicWAVEHeader WaveHeader;
	TByteArrPtr WaveData;
	cardinal WaveDataCursor;
};


TWaveFile OggSoundFromFile(const std::string& fileName);
TWaveFile LoadWaveFile(const std::string& fileName);

DSBUFFERDESC FillPrimaryWaveBufferDescription();

WAVEFORMATEX FillPrimaryWaveFormat(cardinal samplesPerSec);

DSBUFFERDESC FillSecondaryWaveBufferDescription(cardinal bufferSize, WAVEFORMATEX* pWaveFormat);

WAVEFORMATEX FillSecondaryWaveFormat(cardinal bufferSize);


class TMusicDataAccessInterface
{
public:

	TMusicDataAccessInterface() : goneOverEnd(false) { }

protected:

	bool goneOverEnd;

	void ResetDataCursor();

	virtual int InnerReadDataToBuffer(char* buffer, int size, int& bytesRead) = 0;

	virtual void InnerResetDataCursor() = 0;

	virtual void LoadSoundData(const std::string& fileName) = 0;

	virtual void ClearSoundData() = 0;

	virtual bool CheckIfSoundIsOver() = 0;

	virtual cardinal GetSamplesPerSec() = 0;

	void ReadDataToBuffer_Looped(char* buffer, int size);

	int ReadDataToBuffer(char* buffer, int size);

};


class TMusicStreamAncestor : public TMusicDataAccessInterface
{
public:

	TMusicStreamAncestor();
	~TMusicStreamAncestor();

	void Load(const std::string& musicFileName);
	void Clear();

	void Update();

	void Play();
	void PlayLooped();

	void Stop();

protected:
	void InnerPlay();
	void InitialFillBuffer();


	bool IsPlaying;
	bool Looped;
	bool IsLoaded;

	TDirectSoundStruct DirectSoundBuffer;

	cardinal BufferCursorPos;
	cardinal PlayCursorPos;

	cardinal TotalPlayCursorPos;
	
};



class TWaveStream : public TMusicStreamAncestor
{
public:
	
	TWaveStream();
	~TWaveStream();

protected:
	
	virtual int InnerReadDataToBuffer(char* buffer, int size, int& bytesRead);
	virtual void InnerResetDataCursor();
	virtual void ClearSoundData();
	virtual void LoadSoundData(const std::string& fileName);
	virtual bool CheckIfSoundIsOver();

	virtual cardinal GetSamplesPerSec();

	TWaveFile MusicFile;

};


class TOggMusicStream : public TMusicStreamAncestor
{
public:
	
	TOggMusicStream();
	~TOggMusicStream();

protected:

	bool IsInited;
	
	virtual int InnerReadDataToBuffer(char* buffer, int size, int& bytesRead);
	virtual void InnerResetDataCursor();
	virtual void ClearSoundData();
	virtual void LoadSoundData(const std::string& fileName);
	virtual bool CheckIfSoundIsOver();
	virtual cardinal GetSamplesPerSec();

	TOggCallbackFileBuffer FileBuffer;
    OggVorbis_File OggFileData;
    vorbis_info* VorbisInfo;

};





DWORD GetMaxWriteSize(cardinal m_cbBufOffset, cardinal m_cbBufSize, TDirectSoundStruct& soundStruct);

void FillMemoryWithZero(char* bufferPtr, cardinal bufferSize);


class TSoundManagerWindows : public TSoundManagerInterface
{
private:

	std::map<std::string, TDirectSoundStruct> SoundMap;
	std::map<std::string, boost::shared_ptr<TMusicStreamAncestor> > StreamMap;

	void InitDirectSound();
	void ShutdownDirectSound();

	

public:
	TSoundManagerWindows();
	~TSoundManagerWindows();
	
	virtual void LoadSound(const std::string& soundFileName);
	virtual void PlaySound(const std::string& soundName);

	virtual void LoadMusic(const std::string& musicFileName);
	virtual void PlayMusic(const std::string& musicName);
	virtual void PlayMusicLooped(const std::string& musicName);
	virtual void StopMusic(const std::string& musicName);

	virtual void StopAllMusic();
	virtual void TryStopAndPlayMusicLooped(const std::string& musicName);

	virtual void Update(cardinal dt);

	IDirectSoundBuffer8* CreateDirectSound8InterfaceBuffer(cardinal dataSize, cardinal samplesPerSec);

	IDirectSound8* DirectSound;
	IDirectSoundBuffer* PrimaryBuffer;

};

} //namespace SE

#else

namespace SE
{


//NOSOUND version - just dummy
class TSoundManagerWindows : public TSoundManagerInterface
{
private:

public:

	virtual void LoadSound(const std::string& soundFileName) { }
	virtual void PlaySound(const std::string& soundName) { }

	virtual void LoadMusic(const std::string& musicFileName) { }
	virtual void PlayMusic(const std::string& musicName) { }
	virtual void PlayMusicLooped(const std::string& musicName) { }
	virtual void StopMusic(const std::string& musicName) { }
	
};


} //namespace SE


#endif

#endif