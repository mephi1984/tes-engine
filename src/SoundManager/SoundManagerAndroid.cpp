#include "include/Engine.h"

namespace SE
{

TSoundManagerAndroid::TSoundManagerAndroid()
{
}

TSoundManagerAndroid::~TSoundManagerAndroid()
{
	JniReleaseAllSoundsAndMusic();
}


void TSoundManagerAndroid::LoadSound(const std::string& soundFileName)
{
	JniLoadSound(soundFileName);
}

void TSoundManagerAndroid::PlaySound(const std::string& soundName)
{
	JniPlaySound(soundName);
}


void TSoundManagerAndroid::LoadMusic(const std::string& musicFileName)
{
	//dummy. Not needed
}

void TSoundManagerAndroid::PlayMusic(const std::string& musicName)
{
	JniPlayMusic(musicName);
}

void TSoundManagerAndroid::PlayMusicLooped(const std::string& musicName)
{
	JniPlayMusicLooped(musicName);
}

void TSoundManagerAndroid::StopMusic(const std::string& musicName)
{
	JniStopMusic();
}


} //namespace SE