#ifndef SOUND_MANAGER_INTERFACE_H_INCLUDED
#define SOUND_MANAGER_INTERFACE_H_INCLUDED
#include "include/Utils/DataTypes/DataTypes.h"

namespace SE
{

class TSoundManagerInterface
{
public:

	virtual void LoadSound(const std::string& soundFileName) = 0;
	virtual void PlaySound(const std::string& soundName) = 0;

	virtual void LoadMusic(const std::string& musicFileName) = 0;
	virtual void PlayMusic(const std::string& musicName) = 0;
	virtual void PlayMusicLooped(const std::string& musicName) = 0;
	virtual void StopMusic(const std::string& musicName) = 0;

	virtual void StopAllMusic() { }

	virtual void TryStopAndPlayMusicLooped(const std::string& musicName) { }

	virtual void Update(cardinal dt) { };


protected:

};

} //namespace SE


#endif