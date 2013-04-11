//  GlobalClass.h
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef KiteMania_GlobalClass_h
#define KiteMania_GlobalClass_h

#include "cocos2d.h"

USING_NS_CC;

class GlobalClass
{
	public:
		static float getScaledFont(int fontSize);
		static float getScaleBySprite();
		static float getScaleByScreen();

		static CCPoint getCenterPointOfSize(CCSize size);
		
		static std::string getSmallFont(void);
		static std::string getMessageFont(void);
		static std::string getMenuFont(void);
		static std::string getWordLatterFont(void);
		static std::string getHelpMessageFont(void);
		static std::string getCurrentLanguageName();
		
		static void loadFontFiles();
};

#endif	//	__KiteMania_GlobalClass_h__
