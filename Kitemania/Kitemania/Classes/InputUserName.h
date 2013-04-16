//
//  InputUserName.h
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef KiteMania_InputUserName_h
#define KiteMania_InputUserName_h

#include "cocos2d.h"

USING_NS_CC;

class  InputUserName : public CCLayerColor, public CCTextFieldDelegate
{
		CCPoint  m_beginPos;
		CCTextFieldTTF *userTxt;
	
    public:
		InputUserName();
		~InputUserName();

	    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
		virtual bool init();
		
		virtual void onEnter();
		virtual void onExit();

		// there's no 'id' in cpp, so we recommand to return the exactly class pointer
		static CCScene* scene();
		
		// implement the "static node()" method manually
		CREATE_FUNC(InputUserName);
		
		void initScene(CCNode* sender);
		virtual void keyBackClicked();

        void submitNameToGameOver(CCObject* sender);
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
		virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
};

#endif	//	__KiteMania_InputUserName_h__
