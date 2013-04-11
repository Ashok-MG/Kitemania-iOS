//
//  GameSettings.h
//  Kitemania
//
//  Created by Vishal Golia on 26/11/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef Kitemania_GameSettings_h
#define Kitemania_GameSettings_h

#include "cocos2d.h"

struct UsersInfo
{
    char userName[15];
    unsigned int score;
};

struct SavedData
{
    int sound;
    unsigned int score;
    int mode;
    int langType;
	
	unsigned int oldScore;
    int oldLife;
	
    char userName[15];
    UsersInfo users[10];
    UsersInfo endlessUsers[10];
    UsersInfo timerUsers[10]; 
};

class GameSettings
{
	private: 
		SavedData temp;
		void saveData();
		
	public :
		/** returns a shared instance of the Translate */
		static GameSettings* sharedSetting(void);

		GameSettings();
		~GameSettings();
		
		//setters
		void setMode(int mode);        
		void setMusic(int status);
		void setScore(int score);
		
		int getMinMaxScore(int gVal);
		int getMinMaxScoreEndless(int gVal);
		int getMinMaxScoreTimer(int gVal);
		
		void setLangugaeType(int lang);
		void setUserName(char name[]);
		
		void addUser(const char *name, int score);
		void addUserEndless(const char *name, int score);
		void addUserTimer(const char *name,int score);
		
		void setDummyUsers();
		void setOldScore(int sco);
		void setOldLife(int lif);
		
		//getters 
		int getOldScore();
		int getOldLife();
		int getMusic();
		int getScore();
		int getMode();
		int getLanguageType();
		
		char* getUserName();
		bool isStorageSet();
		
		//-------
		UsersInfo* topTenUsers();
		UsersInfo* topTenUsersEndless();
		UsersInfo* topTenUsersTimer();
		//-------
		
		void setTempData();
		void saveTempData();
};

#endif	//	__Kitemania_GameSettings_h__
