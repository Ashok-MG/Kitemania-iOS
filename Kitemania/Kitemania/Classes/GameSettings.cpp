//
//  GameSettings.cpp
//  Kitemania
//
//  Created by Vishal Golia on 26/11/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "GameSettings.h"

USING_NS_CC;

// singleton stuff
static GameSettings *s_SharedSetting = NULL;

GameSettings* GameSettings::sharedSetting()
{
    if (!s_SharedSetting)
	{
        s_SharedSetting = new GameSettings();
	}
	
    return s_SharedSetting;
}

GameSettings::GameSettings() {}

GameSettings::~GameSettings() {}

//getters
int GameSettings::getMode() {
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("mode", 0);
}

int GameSettings::getScore() {
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("score", 0);
}

int GameSettings::getMusic() {
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("sound", 0);
}

int GameSettings::getLanguageType() {
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("langType", 0);
}

char* GameSettings::getUserName() {
	std::string uName=CCUserDefault::sharedUserDefault()->getStringForKey("userName", "");
    return const_cast<char*>(uName.c_str());
}

int GameSettings::getOldScore() {
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("oldScore", 0);
}

int GameSettings::getOldLife() {
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("oldLife", 0);
}

//setters
void GameSettings::setOldScore(int sco) {
    CCUserDefault::sharedUserDefault()->setIntegerForKey("oldScore", sco);
    saveData();
}

void GameSettings::setOldLife(int lif) {
    CCUserDefault::sharedUserDefault()->setIntegerForKey("oldLife", lif);
    saveData();
}

void GameSettings::setMusic(int status) {
    CCUserDefault::sharedUserDefault()->setIntegerForKey("sound", status);
    saveData();
}

void GameSettings::setMode(int mode) {
    CCUserDefault::sharedUserDefault()->setIntegerForKey("mode", mode);
    saveData();
}

void GameSettings::setScore(int score) {
    CCUserDefault::sharedUserDefault()->setIntegerForKey("score", score);
    saveData(); 
}

void GameSettings::setLanguageType(int lang) {
    CCUserDefault::sharedUserDefault()->setIntegerForKey("langType", lang);
    saveData(); 
}

void GameSettings::setUserName(char name[]) {
    CCUserDefault::sharedUserDefault()->setStringForKey("userName", name);
    saveData(); 
}

void GameSettings::saveData() {
    CCLOG("DATA SAVED>>>>>");
    CCUserDefault::sharedUserDefault()->flush();
}

bool GameSettings::isStorageSet() {
    if (CCUserDefault::sharedUserDefault()->getIntegerForKey("sound", 120) == 120) return false;
    else return true;
}

int GameSettings::getMinMaxScore(int gVal)
{
    int totalUsersAdded=sizeof(temp.users)/sizeof(temp.users[0]);
    int minScore=0, maxScore=0,i=0;
    
    if(totalUsersAdded>0)
	{
        CCLOG("USERS>0");
        minScore=temp.users[0].score;       
        maxScore=temp.users[0].score;
        
        for (i = 0; i < totalUsersAdded; i++)
		{
            if(temp.users[i].score>maxScore) {
                maxScore=temp.users[i].score;
            } else if(temp.users[i].score<minScore) {
                minScore=temp.users[i].score;
            }
        }
    }
    
    if (gVal == 1) {
        return maxScore;
    } else if (gVal == 2) {
        return minScore;
    }
    return 0;
}

int GameSettings::getMinMaxScoreEndless(int gVal)
{
    int totalUsersAdded=sizeof(temp.endlessUsers)/sizeof(temp.endlessUsers[0]);
    int minScore=0, maxScore=0,i=0;
    
    if(totalUsersAdded>0)
	{
        CCLOG("USERS>0");
        minScore=temp.endlessUsers[0].score;       
        maxScore=temp.endlessUsers[0].score;
        
        for (i = 0; i < totalUsersAdded; i++)
		{
            if(temp.endlessUsers[i].score>maxScore) {
                maxScore=temp.endlessUsers[i].score;
            } else if(temp.endlessUsers[i].score<minScore) {
                minScore=temp.endlessUsers[i].score;
            }
        }
    }
    
    if (gVal == 1) {
        return maxScore;
    } else if (gVal == 2) {
        return minScore;
    }
    return 0;
}

int GameSettings::getMinMaxScoreTimer(int gVal)
{
    int totalUsersAdded=sizeof(temp.timerUsers)/sizeof(temp.timerUsers[0]);
    int minScore=0, maxScore=0,i=0;
    
    if(totalUsersAdded>0)
	{
        CCLOG("USERS>0");
        minScore=temp.timerUsers[0].score;       
        maxScore=temp.timerUsers[0].score;
        
        for (i = 0; i < totalUsersAdded; i++)
		{
            if(temp.timerUsers[i].score>maxScore) {
                maxScore=temp.timerUsers[i].score;
            } else if(temp.timerUsers[i].score<minScore) {
                minScore=temp.timerUsers[i].score;
            }
        }
    }
    
    if (gVal == 1) {
        return maxScore;
    } else if (gVal == 2) {
        return minScore;
    }
    return 0;
}

void GameSettings::addUser(const char *name, int score)
{
    int totalUsersAdded=sizeof(temp.users)/sizeof(temp.users[0]);
    int minScore=0, maxScore=0,i=0;
    
	//find the max and min scores
    CCLOG("USERS --- %d   score --- %d  name -- %s ",totalUsersAdded,score,name);
    
    if(totalUsersAdded>0)
	{
        CCLOG("USERS>0");
        minScore=temp.users[0].score;       
        maxScore=temp.users[0].score;
        
        for (i = 0; i < totalUsersAdded; i++)
		{
            if(temp.users[i].score>maxScore) {
                maxScore=temp.users[i].score;
            } else if(temp.users[i].score<minScore) {
                minScore=temp.users[i].score;
            }
        }
    }
    
    CCLOG("Min score-- %d  max score -- %d  ",minScore, maxScore);
    
	//check whether new user is between max and min
    if(score>=maxScore)
	{
		//add them to top, but check that only 10 users score can be added, so minimum has to be deleted
        CCLOG("score>=maxScore");
        
        for(i=totalUsersAdded-1;i>0;i--)
		{
			//shift positions
            CCLOG("shifting positions (score>=maxScore) ---->>>>");
            temp.users[i].score=temp.users[i-1].score;
            strcpy(temp.users[i].userName, temp.users[i-1].userName);
        }
        
        temp.users[0].score=score;
        strcpy(temp.users[0].userName,name);
		CCLOG("DATA goining to saved when score>=maxScore = %d %s ",temp.users[0].score,temp.users[0].userName);
    }
	else if (score<=minScore)
	{
        CCLOG("score<=minScore");
        
		//replace the earlier min score  with new min score if all positions are filled, otherwise add the min score in the end      
        if(totalUsersAdded<=10) {
            temp.users[9].score=score;
            strcpy(temp.users[9].userName,name);
        } else {
            temp.users[totalUsersAdded].score=score;
            strcpy(temp.users[totalUsersAdded].userName,name);
        }
    }
	else if (score>minScore && score<maxScore)
	{
		//put the new score in the list and shift scores
        CCLOG("score>minScore && score<maxScore"); 
        
        int j=0,pos=0;
        
        for(j=0;j<totalUsersAdded;j++)
		{
            if(score>temp.users[j].score)
			{
                pos=j;
                break;
            }
        }
        
        for(i=totalUsersAdded-1;i>pos;i--)
		{
			//shift positions
            temp.users[i].score=temp.users[i-1].score;
            strcpy(temp.users[i].userName, temp.users[i-1].userName);
        }
        
        temp.users[pos].score=score;
        strcpy(temp.users[pos].userName,name);
    }
    saveTempData();
    saveData();
}  

//users
void GameSettings::addUserEndless(const char *name, int score)
{
    int totalUsersAdded=sizeof(temp.endlessUsers)/sizeof(temp.endlessUsers[0]);
    int minScore=0, maxScore=0,i=0;
    
	//find the max and min scores
    CCLOG("USERS --- %d   score --- %d   name -- %s ",totalUsersAdded,score,name);
    
    if(totalUsersAdded>0)
	{
        CCLOG("USERS>0");
        minScore=temp.endlessUsers[0].score;       
        maxScore=temp.endlessUsers[0].score;
        
        for (i = 0; i < totalUsersAdded; i++) {
            if(temp.endlessUsers[i].score>maxScore) {
                maxScore=temp.endlessUsers[i].score;
            } else if(temp.endlessUsers[i].score<minScore) {
                minScore=temp.endlessUsers[i].score;
            }
        }
    }
    
    CCLOG("Min score-- %d  max score -- %d  ",minScore, maxScore);
    
	//check whether new user is between max and min
    if(score>=maxScore)
	{
		//add them to top, but check that only 10 users score can be added, so minimum has to be deleted
        CCLOG("score>=maxScore");
        
        for(i=totalUsersAdded-1;i>0;i--)
		{
			//shift positions
            CCLOG("shifting positions (score>=maxScore) ---->>>>");
            temp.endlessUsers[i].score=temp.endlessUsers[i-1].score;
            strcpy(temp.endlessUsers[i].userName, temp.endlessUsers[i-1].userName);
			
        }
        
        temp.endlessUsers[0].score=score;
        strcpy(temp.endlessUsers[0].userName,name);
		// CCLOG("DATA goining to saved when score>=maxScore = %d %s %s  ",temp.users[0].score,temp.users[0].userName,temp.users[i].dateString);
    }
	else if(score<=minScore)
	{
        CCLOG("score<=minScore");
		//replace the earlier min score  with new min score if all positions are filled, otherwise add the min score in the end
	
        if(totalUsersAdded<=10) {
            temp.endlessUsers[9].score=score;
            strcpy(temp.endlessUsers[9].userName,name);
        } else {
            temp.endlessUsers[totalUsersAdded].score=score;
            strcpy(temp.endlessUsers[totalUsersAdded].userName,name);
        }
    }
	else if(score>minScore && score<maxScore)
	{
		//put the new score in the list and shift scores
        CCLOG("score>minScore && score<maxScore"); 
        
        int j=0,pos=0;
        
        for(j=0;j<totalUsersAdded;j++)
		{
            if(score>temp.endlessUsers[j].score)
			{
                pos=j;
                break;
            }
        }
        
        for(i=totalUsersAdded-1;i>pos;i--)
		{
			//shift positions
            temp.endlessUsers[i].score=temp.endlessUsers[i-1].score;
            strcpy(temp.endlessUsers[i].userName, temp.endlessUsers[i-1].userName);
        }
        
        temp.endlessUsers[pos].score=score;
        strcpy(temp.endlessUsers[pos].userName,name);
    }
    saveTempData();
    saveData();
}

void GameSettings::addUserTimer(const char *name, int score)
{
    int totalUsersAdded=sizeof(temp.timerUsers)/sizeof(temp.timerUsers[0]);
    int minScore=0, maxScore=0,i=0;
    
	//find the max and min scores
    CCLOG("USERS --- %d   score --- %d   name -- %s ",totalUsersAdded,score,name);
    
    if(totalUsersAdded>0)
	{
        
        CCLOG("USERS>0");
        minScore=temp.timerUsers[0].score;       
        maxScore=temp.timerUsers[0].score;
        
        for (i = 0; i < totalUsersAdded; i++)
		{
            if(temp.timerUsers[i].score>maxScore) {
                maxScore=temp.timerUsers[i].score;
            } else if(temp.timerUsers[i].score<minScore) {
                minScore=temp.timerUsers[i].score;
            }
        }
    }
    
    CCLOG("Min score-- %d  max score -- %d  ",minScore, maxScore);
    
	//check whether new user is between max and min
    if(score>=maxScore)
	{
		//add them to top, but check that only 10 users score can be added, so minimum has to be deleted
        CCLOG("score>=maxScore");
        
        for(i=totalUsersAdded-1;i>0;i--)
		{
			//shift positions
            CCLOG("shifting positions (score>=maxScore) ---->>>>");
            temp.timerUsers[i].score=temp.timerUsers[i-1].score;
            strcpy(temp.timerUsers[i].userName, temp.timerUsers[i-1].userName);
        }
        
        temp.timerUsers[0].score=score;
        strcpy(temp.timerUsers[0].userName,name);
        
		// CCLOG("DATA goining to saved when score>=maxScore = %d %s %s  ",temp.users[0].score,temp.users[0].userName,temp.users[i].dateString);
    }
	else if(score<=minScore)
	{
        CCLOG("score<=minScore");
		//replace the earlier min score  with new min score if all positions are filled, otherwise add the min score in the end
	
        if(totalUsersAdded<=10) {
            temp.timerUsers[9].score=score;
            strcpy(temp.timerUsers[9].userName,name);
        } else {
            temp.timerUsers[totalUsersAdded].score=score;
            strcpy(temp.timerUsers[totalUsersAdded].userName,name);
        }
    }
	else if(score>minScore && score<maxScore)
	{
		//put the new score in the list and shift scores
        CCLOG("score>minScore && score<maxScore"); 
        
        int j=0,pos=0;
        
        for(j=0;j<totalUsersAdded;j++)
		{
            if(score>temp.timerUsers[j].score)
			{
                pos=j;
                break;
            }
        }
        
        for(i=totalUsersAdded-1;i>pos;i--)
		{
			//shift positions
            temp.timerUsers[i].score=temp.timerUsers[i-1].score;
            strcpy(temp.timerUsers[i].userName, temp.timerUsers[i-1].userName);
        }
	
        temp.timerUsers[pos].score=score;
        strcpy(temp.timerUsers[pos].userName,name);
    }
	saveTempData();
    saveData();
}

UsersInfo* GameSettings::topTenUsers() {
    return temp.users;
}

UsersInfo* GameSettings::topTenUsersEndless() {
    return temp.endlessUsers;
}

UsersInfo* GameSettings::topTenUsersTimer() {
	return temp.timerUsers;
}


void GameSettings::setDummyUsers()
{
    int i=0;
    for(i=0;i<10;i++)
	{
		//classic users        
        temp.users[i].score=0;
        strcpy(temp.users[i].userName," ");
		
		//endless
        temp.endlessUsers[i].score=0;
        strcpy(temp.endlessUsers[i].userName," ");
	
		//timer
        temp.timerUsers[i].score=0;
        strcpy(temp.timerUsers[i].userName," ");
    }
	saveData();
}

void GameSettings::setTempData()
{
    for(int i=0;i<10;i++)
	{
        char fieldName[40];
		
		//classic users       
        sprintf(fieldName, "users_score_%d",i);
        temp.users[i].score = CCUserDefault::sharedUserDefault()->getIntegerForKey(fieldName,0);
        sprintf(fieldName, "users_userName_%d",i);
        strcpy(temp.users[i].userName, CCUserDefault::sharedUserDefault()->getStringForKey(fieldName, " ").c_str());
        
		//endless
        sprintf(fieldName, "endlessUsers_score_%d",i);
        temp.endlessUsers[i].score=CCUserDefault::sharedUserDefault()->getIntegerForKey(fieldName,0);
        sprintf(fieldName, "endlessUsers_userName_%d",i);
        strcpy(temp.endlessUsers[i].userName, CCUserDefault::sharedUserDefault()->getStringForKey(fieldName, " ").c_str());
        
		//timer
        sprintf(fieldName, "timerUsers_score_%d",i);
        temp.timerUsers[i].score=CCUserDefault::sharedUserDefault()->getIntegerForKey(fieldName,0);
        sprintf(fieldName, "timerUsers_userName_%d",i);
        strcpy(temp.timerUsers[i].userName, CCUserDefault::sharedUserDefault()->getStringForKey(fieldName, " ").c_str());
    }
}

void GameSettings::saveTempData()
{
    for(int i=0;i<10;i++)
	{
        char fieldName[40];
        
		//classic users       
        sprintf(fieldName, "users_score_%d",i);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(fieldName, temp.users[i].score);
        sprintf(fieldName, "users_userName_%d",i);
        CCUserDefault::sharedUserDefault()->setStringForKey(fieldName, temp.users[i].userName);
        
		//endless
        sprintf(fieldName, "endlessUsers_score_%d",i);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(fieldName, temp.endlessUsers[i].score);
        sprintf(fieldName, "endlessUsers_userName_%d",i);
        CCUserDefault::sharedUserDefault()->setStringForKey(fieldName, temp.endlessUsers[i].userName);
        
		//timer
        sprintf(fieldName, "timerUsers_score_%d",i);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(fieldName, temp.timerUsers[i].score);
        sprintf(fieldName, "timerUsers_userName_%d",i);
        CCUserDefault::sharedUserDefault()->setStringForKey(fieldName, temp.timerUsers[i].userName);
    }
}
