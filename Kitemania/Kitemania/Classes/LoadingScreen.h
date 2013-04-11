//
//  LoadingScreen.h
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef Kitmania_LoadingScreen_h
#define Kitemania_LoadingScreen_h

#include "cocos2d.h"

USING_NS_CC;

class LoadingScreen : public CCLayerColor
{
    public:
         LoadingScreen();
        ~LoadingScreen();
		
	    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
		virtual bool init();
	
		virtual void onEnter();
		virtual void onExit();

		// there's no 'id' in cpp, so we recommend to return the class instance pointer
		static CCScene* scene();

		// preprocessor macro for "static create()" constructor ( node() deprecated )
		CREATE_FUNC(LoadingScreen);

	private:
		void initScene(CCObject *pSender);

		void loadGameAssestes(CCNode* sender);
	
		void startGame(CCNode* sender);
    };
    
#endif // Kitemania_LoadingScreen_h
