//
//  Music.cpp
//  Kitemania
//
//  Created by Vishal Golia on 31/10/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "Music.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Music::Music() {}
Music::~Music()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect("Balloon.m4a");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect("CORRECTWORD.m4a");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect("FLICK.m4a");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect("HIT.m4a");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect("LETTERPICK.m4a");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect("zap.m4a");
}

// singleton stuff
static Music *s_SharedMusic = NULL;

Music* Music::sharedMusic(void)
{
    if (!s_SharedMusic)
	{
        s_SharedMusic = new Music();
	}
    return s_SharedMusic;
}

/*
 * playBackgroundSound function plays game background sound
 */
void Music::playBackgroundSound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("BG.m4a", true);
}

//Play sfx sound
void Music::playEffectSound(const char* sName)
{
    stopaAllSoundEffects();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(sName);
}

/*
 * pauseBackgroundSound function pause game background sound
 */
void Music::pauseBackgroundSound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

/*
 * pauseBackgroundSound function resume game background sound
 */
void Music::resumeBackgroundSound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void Music::stopaAllSoundEffects()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

/*
 * muteGameSound function mute the game sound
 */
void Music::muteGameSound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

/*
 * resumeGameSound function mute the game sound
 */
void Music::resumeGameSound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

/*
 * preLoadBackgroundMusic function make buffer of background sound
 */
void Music::preLoadBackgroundMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("BG.m4af");
}

/*
 * preloadEffectSound function make buffer of SFX music
 */
void Music::preloadEffectSound(const char* sName)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(sName);
}
