//  GlobalClass.cpp
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "GlobalClass.h"
#include "GameSettings.h"
#include "Resources.h"

//Use for font scaling according to screen resolution -- CC_CONTENT_SCALE_FACTOR = 1 means non-retina and 2 means retina
float GlobalClass::getScaledFont(int fontSize)
{
	return fontSize / CC_CONTENT_SCALE_FACTOR();
}

//use for control scaling depands on retina
float GlobalClass::getScaleBySprite()
{
	if ( CCDirector::sharedDirector()->getWinSizeInPixels().height > 960) 
		return CCDirector::sharedDirector()->getWinSizeInPixels().height/960;
	else
		return CCDirector::sharedDirector()->getWinSizeInPixels().width/640;
}

float GlobalClass::getScaleByScreen()
{
	return CCDirector::sharedDirector()->getWinSizeInPixels().width/640;
}

//use for geting center point
CCPoint GlobalClass::getCenterPointOfSize(CCSize size)
{
	return CCPointMake(size.width/2, size.height/2);
}

std::string GlobalClass::getSmallFont(void)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    std::string m_nameStr;
    
	if (screenSize.width <= 240) {
        m_nameStr = "Segoe16.fnt";
    } else if (screenSize.width <= 320) {
        m_nameStr = "Segoe20.fnt";
    } else if (screenSize.width <= 600) {
        m_nameStr = "Segoe30.fnt";
    } else{
        m_nameStr = "Segoe42.fnt";
    }
	return m_nameStr;
}

std::string GlobalClass::getMessageFont(void)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    std::string m_nameStr;
    
    if (screenSize.width <= 240) {
        m_nameStr = "Tw26.fnt";
    } else if (screenSize.width <= 320) {
        m_nameStr = "Tw40.fnt";
    } else if (screenSize.width <= 600) {
        m_nameStr = "Tw56.fnt";
    } else{
        m_nameStr = "Tw70.fnt";
    }
	return m_nameStr;
}

std::string GlobalClass::getMenuFont(void)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    std::string m_nameStr;
    
    if (screenSize.width <= 240) {
        m_nameStr = "Tw20.fnt";
    } else if (screenSize.width <= 320) {
        m_nameStr = "Tw26.fnt";
    } else if (screenSize.width <= 600) {
        m_nameStr = "Tw40.fnt";
    } else{
        m_nameStr = "Tw56.fnt";
    }
	return m_nameStr;
}

std::string GlobalClass::getHelpMessageFont(void)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    std::string m_nameStr;
    	
    if (screenSize.width <= 240) {
        m_nameStr = "Tw16.fnt";
    } else if (screenSize.width <= 320) {
        m_nameStr = "Tw20.fnt";
    } else if (screenSize.width <= 600) {
        m_nameStr = "Tw30.fnt";
    } else {
        m_nameStr = "Tw40.fnt";
    }
	return m_nameStr;
}

std::string GlobalClass::getWordLatterFont(void)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    std::string m_nameStr;
    
    if (screenSize.width <= 240) {
        m_nameStr = "Segoe20_bold.fnt";
    } else if (screenSize.width <= 320) {
        m_nameStr = "Segoe26_bold.fnt";
    } else if (screenSize.width <= 600) {
        m_nameStr = "Segoe36_bold.fnt";
    } else {
        m_nameStr = "Segoe54_bold.fnt";
    }
	return m_nameStr;
}

void GlobalClass::loadFontFiles()
{
//	CCLabelBMFont *scLabel = CCLabelBMFont::labelWithString("0", GlobalClass::getSmallFont().c_str());
//	scLabel = CCLabelBMFont::labelWithString("0", GlobalClass::getMessageFont().c_str());
////        scLabel = CCLabelBMFont::labelWithString("0", GlobalClass::getMenuFont().c_str());
//	scLabel = CCLabelBMFont::labelWithString("0", GlobalClass::getHelpMessageFont().c_str());
//	scLabel = CCLabelBMFont::labelWithString("0", GlobalClass::getWordLatterFont().c_str());
}

std::string GlobalClass::getCurrentLanguageName()
{
    switch (GameSettings::sharedSetting()->getLanguageType())
    {
        case kTagEnglish:		return "English";
		case kTagSpanish:		return "Spanish";
		case kTagPortuguese:	return "Portuguese";
        case kTagFrench:		return "French";
        case kTagGerman:		return "German";
    }
    return "";
}

std::string GlobalClass::AMLocalization(std::string key, std::string dValue)
{
//	if (!dict) {
//		std::string m_sPlistFile = CCFileUtils::fullPathFromRelativePath("language.plist");
//		dict = CCFileUtils::dictionaryWithContentsOfFile(m_sPlistFile.c_str());
//		CCAssert( dict != NULL, "Level: file not found");
//		dict->retain();
//	}
//	
//    CCDictionary<std::string, CCObject*> *LangsDict = (CCDictionary<std::string, CCObject*>*)dict->objectForKey(getCurrentLanguageName().c_str());
//	
//    LangsDict->begin();
//    CCString *pString = (CCString*)LangsDict->objectForKey(key);
//    dValue = pString ? pString->m_sString : dValue;
    return dValue;
}


/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;
	
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void GlobalClass::itoa(int n, char s[])
{
    int i, sign;
	
    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
