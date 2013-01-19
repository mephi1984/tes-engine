#ifndef SOUND_MANAGER_ANDROID_INCLUDED
#define SOUND_MANAGER_ANDROID_INCLUDED

#include "include/Utils/DataTypes/DataTypes.h"
#include "include/SoundManager/SoundManagerInterface.h"

namespace SE
{

class TSoundManagerAndroid : public TSoundManagerInterface
{
public:
	TSoundManagerAndroid();
	~TSoundManagerAndroid();


	virtual void LoadSound(const std::string& soundFileName);
	virtual void PlaySound(const std::string& soundName);

	virtual void LoadMusic(const std::string& musicFileName);
	virtual void PlayMusic(const std::string& musicName);
	virtual void PlayMusicLooped(const std::string& musicName);
	virtual void StopMusic(const std::string& musicName);


protected:

};


#endif

} //namespace SE