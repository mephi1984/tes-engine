//
//  SoundManagerIos.mm
//  Halibut Engine
//
//  Created by vvv ооо on 21.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <string>
#include <map>

#import <Foundation/Foundation.h>
#import "PASoundMgr.h"
#import "PASoundListener.h"
#import "PASoundSource.h"

#include "include/Utils/Utils.h"

#include "include/SoundManager/SoundManagerIos.h"

namespace SE
{

std::map<std::string, PASoundSource*> AudioSourceMap;

TSoundManagerIos::TSoundManagerIos()
{
    [PASoundMgr sharedSoundManager];
    [[[PASoundMgr sharedSoundManager] listener] setPosition:CGPointMake(0, 0)];
}

TSoundManagerIos::~TSoundManagerIos()
{
    for (std::map<std::string, PASoundSource*>::iterator i = AudioSourceMap.begin(); i != AudioSourceMap.end(); ++i)
    {
        //[i->second release];
    }
    AudioSourceMap.clear();
}

void TSoundManagerIos::LoadSound(const std::string& soundFileName)
{
    LoadMusic(soundFileName);
}


void TSoundManagerIos::PlaySound(const std::string& soundName)
{
    PlayMusic(soundName);
}

void TSoundManagerIos::LoadMusicLooped(const std::string& musicFileName)
{
    //Hack for ios, see below
    std::string musicName = GetFileName(musicFileName);
    
    std::string fullFileName = GetPathToResources() + musicFileName;
    
    NSString* nsFileName = [NSString stringWithCString:fullFileName.c_str() encoding:NSUTF8StringEncoding];
    
    NSString* nsFileNameWithoutExt = [nsFileName stringByDeletingPathExtension];
    
    NSString* nsFileExt = [nsFileName pathExtension];
    
    if (AudioSourceMap.count(musicName) > 0)
    {
        //[AudioSourceMap[musicName] release];
    }
    
    /*AudioSourceMap[musicFileName] = [[[PASoundMgr sharedSoundManager] addSound:@"trance-loop" withExtension:@"ogg" position:CGPointMake(0, 0) looped:NO] retain];*/
    AudioSourceMap[musicName] = [[PASoundMgr sharedSoundManager] addSound:nsFileNameWithoutExt withExtension:nsFileExt position:CGPointMake(0, 0) looped:YES];
}
    
void TSoundManagerIos::LoadMusic(const std::string& musicFileName)
{
    std::string musicName = GetFileName(musicFileName);
    
    std::string fullFileName = GetPathToResources() + musicFileName;
    
    NSString* nsFileName = [NSString stringWithCString:fullFileName.c_str() encoding:NSUTF8StringEncoding];
    
    NSString* nsFileNameWithoutExt = [nsFileName stringByDeletingPathExtension];
    
    NSString* nsFileExt = [nsFileName pathExtension];
    
    if (AudioSourceMap.count(musicName) > 0)
    {
        //[AudioSourceMap[musicName] release];
    }
    
    /*AudioSourceMap[musicFileName] = [[[PASoundMgr sharedSoundManager] addSound:@"trance-loop" withExtension:@"ogg" position:CGPointMake(0, 0) looped:NO] retain];*/
    AudioSourceMap[musicName] = [[PASoundMgr sharedSoundManager] addSound:nsFileNameWithoutExt withExtension:nsFileExt position:CGPointMake(0, 0) looped:NO];
}


void TSoundManagerIos::PlayMusic(const std::string& musicName)
{
    if (AudioSourceMap.count(musicName) == 0)
    {
        throw ErrorToLog("Sound or music not exists: "+musicName);
    }
    
    AudioSourceMap[musicName].looped = false;
    
    [AudioSourceMap[musicName] playAtListenerPosition];
    
    //AudioSourceMap[musicName].isPlaying = true;
}

void TSoundManagerIos::PlayMusicLooped(const std::string& musicName)
{
    if (AudioSourceMap.count(musicName) == 0)
    {
        throw ErrorToLog("Sound or music not exists: "+musicName);
    }
    
    AudioSourceMap[musicName].looped = true;
    
    [AudioSourceMap[musicName] playAtListenerPosition];
    
    //AudioSourceMap[musicName].isPlaying = true;
}

void TSoundManagerIos::StopMusic(const std::string& musicName)
{
    if (AudioSourceMap.count(musicName) == 0)
    {
        throw ErrorToLog("Sound or music not exists: "+musicName);
    }
    
    [AudioSourceMap[musicName] stop];
    //AudioSourceMap[musicName].isPlaying = false;
}

void TSoundManagerIos::StopAllMusic()
{
    for (auto i = AudioSourceMap.begin(); i != AudioSourceMap.end(); ++i)
    {
        PASoundSource* s = i->second;
        if (s.isPlaying)
        {
            [s stop];
            //s.isPlaying = false;
        }
    }
}
    
void TSoundManagerIos::TryStopAndPlayMusicLooped(const std::string& musicName)
{
    if (AudioSourceMap.count(musicName) == 0)
    {
        throw ErrorToLog("Sound or music not exists: "+musicName);
    }
    
    for (auto i = AudioSourceMap.begin(); i != AudioSourceMap.end(); ++i)
    {
        if (i->first != musicName)
        {
            PASoundSource* s = i->second;
            if (s.isPlaying)
            {
                [s stop];
                //s.isPlaying = false;
            }
        }
    }
    
    PASoundSource* s = AudioSourceMap[musicName];
    
    if (!s.isPlaying)
    {
        AudioSourceMap[musicName].looped = true;
        
        //AudioSourceMap[musicName].isPlaying = true;
        
        [AudioSourceMap[musicName] playAtListenerPosition];
    }
}

} //namespace SE