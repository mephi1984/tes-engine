//
//  SoundManagerIos.h
//  Halibut Engine
//
//  Created by vvv ооо on 21.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef SOUND_MANAGER_IOS_H_INCLUDED
#define SOUND_MANAGER_IOS_H_INCLUDED

#include "include/SoundManager/SoundManagerInterface.h"

namespace SE
{

class TSoundManagerIos : public TSoundManagerInterface
{
public:
    
    TSoundManagerIos();
    ~TSoundManagerIos();

    virtual void LoadSound(const std::string& soundFileName);
	virtual void PlaySound(const std::string& soundName);
    
    void LoadMusicLooped(const std::string& musicFileName);
	virtual void LoadMusic(const std::string& musicFileName);
	virtual void PlayMusic(const std::string& musicName);
	virtual void PlayMusicLooped(const std::string& musicName);
	virtual void StopMusic(const std::string& musicName);
    
};


} //namespace SE


#endif
