//
//  MainMenuScene.h
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef Kitmania_MainMenuScene_h
#define Kitemania_MainMenuScene_h

#include "cocos2d.h"

USING_NS_CC;

class MainMenuScene : public CCLayerColor
{
	public:
		MainMenuScene();
		~MainMenuScene();

		// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
		virtual bool init();  

		virtual void onEnter();
		virtual void onExit();

		// there's no 'id' in cpp, so we recommend to return the class instance pointer
		static CCScene* scene();

		// preprocessor macro for "static create()" constructor ( node() deprecated )
		CREATE_FUNC(MainMenuScene);

	private:
		void initScene(CCObject *pSender);
		virtual void keyBackClicked();

		void addGameMenuMethod();	
		
		void playGameMenuMethod(CCObject* sender);
		void langMenuMethod(CCObject* sender);
		void modeMenuMethod(CCObject* sender);
		void soundMenuMethod(CCObject* sender);
		void helpMenuMethod(CCObject *sender);
		void exitMenuMethod(CCObject *sender);

		CCSize screenSize;
		CCLabelBMFont *mStateLbl, *sStateLbl, *lStateLbl;

		int pLangId;
};

#endif // __Kitmania_MainMenuScene_h__
