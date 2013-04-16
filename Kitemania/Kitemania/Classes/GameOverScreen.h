//
//  GameOverScreen.h
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef KiteMania_GameOverScreen_h
#define KiteMania_GameOverScreen_h

#include "cocos2d.h"

USING_NS_CC;

class  GameOverScreen : public CCLayerColor
{
        CCSize screenSize;
		
    public:
		GameOverScreen();
		~GameOverScreen();

	    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	   virtual bool init();
	   
		virtual void onEnter();
		virtual void onExit();

		// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	   static CCScene* scene();
	   
		// implement the "static node()" method manually
	   CREATE_FUNC(GameOverScreen);
	   
	   void initScene(CCNode* sender);
	   virtual void keyBackClicked();

		virtual void backToMainMenu(CCObject* sender);
		virtual void backToPlay(CCObject* sender);
		
		void addScoreBar(int playerId, std::string playerName, int score, std::string date, bool isEmpty);
		void addPlayerBar();
        void androidBackButton(float t);
};

#endif	//	__KiteMania_GameOverScreen_h__


