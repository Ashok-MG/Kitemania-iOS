//
//  InputUserName.cpp
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "InputUserName.h"
#include "GlobalClass.h"
#include "Resources.h"
#include "TranslateScreen.h"
#include "GameSettings.h"
#include "GameOverScreen.h"

enum
{
    kTagTextField,
    kTagScorePanel,
};

USING_NS_CC;

void InputUserName::keyBackClicked()
{
	CCLOG("click on back button");
}

CCScene* InputUserName::scene()
{
	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
    InputUserName *layer = InputUserName::create();
	
	// add layer as a child to scene
    scene->addChild(layer);
	
	// return the scene
    return scene;
}

InputUserName::InputUserName() {}

InputUserName::~InputUserName()
{
	userTxt->detachWithIME();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	removeAllChildrenWithCleanup(true);
}

// on "init" you need to initialize your instance
bool InputUserName::init()
{
	//////////////////////////////
	// 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(37, 188, 241, 255)))
	{
        return false;
	}
	
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	this->setKeypadEnabled(true);

	initScene(NULL);

	return true;
}

void InputUserName::onExit()
{		
    CCLayer::onExit();
}

void InputUserName::onEnter()
{
	CCLayer::onEnter();
}

void InputUserName::initScene(CCNode* sender)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
	//-------Add Background Image
	CCSprite *bgImage = CCSprite::create(BACKGROUND_IMAGE);
    bgImage->setScale( GlobalClass::getScaleBySprite());
	bgImage->setPosition( GlobalClass::getCenterPointOfSize(screenSize));
	this->addChild(bgImage, 0);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(BACKGROUND_IMAGE);

	//-------Add Kite mania logo
	CCSprite *KMLogo = CCSprite::create(KITEMANIA_LOGO);
	KMLogo->setPosition( ccp(bgImage->getContentSize().width*0.5f, bgImage->getContentSize().height*0.9f));
	bgImage->addChild(KMLogo, 2);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(KITEMANIA_LOGO);

	//----Add Game over label
	CCLabelBMFont *topScrLbl = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_IU_GAME_OVER), GlobalClass::getMessageFont().c_str());
	topScrLbl->setPosition( ccp( screenSize.width * 0.5f, screenSize.height * 0.77f));
	this->addChild(topScrLbl, 1);
        
	//-------Add Sscorepanel
	CCSprite *scorepanel = CCSprite::createWithSpriteFrameName(SCROE_PANEL);
	scorepanel->setScale(GlobalClass::getScaleBySprite());
	scorepanel->setPosition( ccp(screenSize.width*0.5, screenSize.height*0.56f));
	this->addChild(scorepanel, 10, kTagScorePanel);

    //-------Add label
    CCLabelTTF *lbl=CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_IU_NAME), FONT_SEGOECRG, GlobalClass::getScaledFont(35));
    lbl->setAnchorPoint(ccp(0, 0.5));
    lbl->setPosition( ccp(scorepanel->getContentSize().width * 0.05f, scorepanel->getContentSize().height * 0.75f) );
    scorepanel->addChild(lbl, 2);
    
    //----Add text field
	userTxt = CCTextFieldTTF::textFieldWithPlaceHolder("Enter Name", CCSize(scorepanel->getContentSize().width*0.6, 0), kCCTextAlignmentLeft, FONT_SEGOECRG, GlobalClass::getScaledFont(35));
	userTxt->setPosition( ccp(lbl->getContentSize().width * 1.1f + lbl->getPosition().x, scorepanel->getContentSize().height * 0.75f));
    scorepanel->addChild(userTxt, 1, kTagTextField);
	userTxt->setAnchorPoint(ccp(0, 0.5));
	userTxt->setColor( ccc3(255, 255, 255));
	userTxt->setColorSpaceHolder( ccc3(149, 108, 80));
	userTxt->setDelegate(this);
	    
    char scorPoint[20];
    sprintf(scorPoint, "%s %d",TranslateScreen::sharedTranslate()->localeString(TEXT_IU_POINTS_SCORED), GameSettings::sharedSetting()->getScore());
    CCLabelTTF *pointsScored = CCLabelTTF::create(scorPoint, FONT_SEGOECRG, GlobalClass::getScaledFont(35));
    pointsScored->setAnchorPoint(ccp(0, 0.5));
    pointsScored->setPosition( ccp(scorepanel->getContentSize().width * 0.05f, scorepanel->getContentSize().height * 0.5f) );
    pointsScored->setColor( ccc3(122, 214, 253));
    scorepanel->addChild(pointsScored, 2);
    
    //------Add submit 
	CCMenuItemSprite *submitName = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(BTN_ON_UP), CCSprite::createWithSpriteFrameName(BTN_ON_DOWN), this, menu_selector(InputUserName::submitNameToGameOver));
	
	CCLabelTTF * label = CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_IU_OK), FONT_SEGOECBD, GlobalClass::getScaledFont(30));
	label->setPosition( ccp(submitName->getContentSize().width * 0.5f, submitName->getContentSize().height * 0.5f));
	submitName->addChild(label, 0);
    
	CCMenu *mMenus = CCMenu::create(submitName, NULL);
	mMenus->setPosition( ccp(scorepanel->getContentSize().width * 0.50f, scorepanel->getContentSize().height * 0.2f));
	mMenus->alignItemsHorizontallyWithPadding(screenSize.width * 0.01f);
	scorepanel->addChild(mMenus, 2);
}

void InputUserName::submitNameToGameOver(cocos2d::CCObject* sender)
{
    GameSettings *gSettings = GameSettings::sharedSetting();
	int scoreVal = gSettings->getScore();
	    
    CCSprite *sp = (CCSprite*)this->getChildByTag(kTagScorePanel);
    CCTextFieldTTF * cftField = (CCTextFieldTTF *) sp->getChildByTag(kTagTextField);
   
    std::string stringName = cftField->getString();
    int wCount = std::strlen(stringName.c_str());
    
    char str[15] = {0};
    for (int i = 0, k = 0; i < wCount; i++) 
    {
        if (stringName.at(i) != ' ' || k == 1) 
        {
            k = 1;
            str[std::strlen(str)] = stringName.at(i);
        }
    }
    
    if (std::strlen(str) == 0) 
	{
        sprintf(str,"%s",TranslateScreen::sharedTranslate()->localeString(TEXT_IU_GUEST));
    }
	
	gSettings->setUserName(str);
	
	if (gSettings->getMode() == kTagClassic) {     //classic
		if (gSettings->getMinMaxScore(2) <= scoreVal)
		{
			gSettings->setScore(0);
			gSettings->addUser(str, scoreVal);
		}
	}
	else if (gSettings->getMode() == kTagEndless)  //endless
	{
		if (gSettings->getMinMaxScoreEndless(2) <= scoreVal) 
		{
			gSettings->setScore(0);
			gSettings->addUserEndless(str, scoreVal);
		}	
	}
	else if (gSettings->getMode() == kTagTimer)   //timer
	{
		if (gSettings->getMinMaxScoreTimer(2) <= scoreVal) 
		{
			gSettings->setScore(0);
			gSettings->addUserTimer(str, scoreVal);
		}		
	}
    
    CCDirector::sharedDirector()->replaceScene(GameOverScreen::scene());
}

bool InputUserName::onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen)
{
		// if insert enter, treat as default to detach with ime
    if ('\n' == *text)
	{
        return false;
	}
    
	// if the textfield's char count more than m_nCharLimit, doesn't insert text anymore.
	int m_nCharLimit = 9;
    if (CCDirector::sharedDirector()->getWinSize().width <= 240) {
        m_nCharLimit = 8;
    }

    if (pSender->getCharCount() >= m_nCharLimit)
	{
        return true;
	}
	return false;
}

bool InputUserName::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_beginPos = pTouch->getLocationInView();	
    m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);
	return true;
}

void InputUserName::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) 
{
    CCPoint endPos = pTouch->getLocationInView();	
    endPos = CCDirector::sharedDirector()->convertToGL(endPos);
	
    float delta = 5.0f;
    if (::abs(endPos.x - m_beginPos.x) > delta || ::abs(endPos.y - m_beginPos.y) > delta) {
		// not click
        m_beginPos.x = m_beginPos.y = -1;
        return;
	}
	
	endPos = userTxt->getParent()->convertToNodeSpace(endPos);
	CCLOG("point x %f, y %f ", endPos.x, endPos.y);
	
	// decide the trackNode is clicked.
	if (userTxt->boundingBox().containsPoint(endPos))
	{
		userTxt->attachWithIME();
	}
}
