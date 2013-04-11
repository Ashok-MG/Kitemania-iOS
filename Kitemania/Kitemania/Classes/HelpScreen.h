//
//  HelpScreen.h
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef KiteMania_HelpScreen_h
#define KiteMania_HelpScreen_h

#include "cocos2d.h"

USING_NS_CC;

class  HelpScreen : public CCLayerColor
{
	 private:
		CCSize screenSize;
		CCSprite *kite;
		CCSprite *ball;
		CCSprite *mItmSubmit;
		
	public:
		HelpScreen();
		~HelpScreen();

	    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
		virtual bool init();
		
		virtual void onEnter();
		virtual void onExit();

		// there's no 'id' in cpp, so we recommand to return the exactly class pointer
		static CCScene* scene();
		
		// preprocessor macro for "static create()" constructor ( node() deprecated )
		CREATE_FUNC(HelpScreen);
		
		void initScene(CCNode* sender);
		virtual void keyBackClicked();

		void kiteAnimation(CCNode* sender);
		void addMoveClouds(float t);
		void addMoveBall(float t);
		
		void addMoveBirds();
		void addAirHot();
		void addlighting();

		void flyKite(float t);
		void moveBubble(CCPoint source, CCPoint destination , CCNode* sender);
		void moveBallAnimation(CCNode* sender);
		
		void showMessage(std::string msg);
		void showSubmitInfo(CCNode* sender);
		void destroyButtonAnimation(CCNode* sender);
		void showBirdHitInfo(CCNode* sender);
		
		void destroyKiteAnimation(CCNode* sender);
		void flickWord(CCNode* sender);
};

#endif	//	__KiteMania_HelpScreen_h__
