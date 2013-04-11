//
//  Music.h
//  Kitemania
//
//  Created by Vishal Golia on 31/10/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef Kite_Mania_Music_h
#define Kite_Mania_Music_h

#include "cocos2d.h"

class Music
{
	public:
		Music();
		~Music();

		/** returns a shared instance of the Music */
		static Music* sharedMusic(void);
		
		//background sound
		void preLoadBackgroundMusic();
		void playBackgroundSound();
		void pauseBackgroundSound();
		void resumeBackgroundSound();
		
		void preloadEffectSound(const char* sName);
		void playEffectSound(const char* sName);
		void stopaAllSoundEffects();

		//mute and resume all game sounds 
		void muteGameSound();
		void resumeGameSound(); 
		
};

#endif	//	__Kite_Mania_Music_h__
