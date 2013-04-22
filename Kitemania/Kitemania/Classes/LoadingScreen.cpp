//
//  LoadingScreen.cpp
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "LoadingScreen.h"
#include "MainMenuScene.h"
#include "GlobalClass.h"
#include "Resources.h"
#include "TranslateScreen.h"
#include "GameSettings.h"
#include "Music.h"

USING_NS_CC;

CCScene* LoadingScreen::scene()
{
	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
    LoadingScreen *layer = LoadingScreen::create();

	// add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

LoadingScreen::LoadingScreen() {}

LoadingScreen::~LoadingScreen() {}

// on "init" you need to initialize your instance
bool LoadingScreen::init()
{
	//////////////////////////////
	// 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(37, 188, 241, 255)))
	{
        return false;
	}
	
	return true;
}

void LoadingScreen::onEnter()
{
	CCLayer::onEnter();
	initScene(NULL);
}

void LoadingScreen::onExit()
{
    CCLayer::onExit();
}

// on "initScene" you need to initialize your instance
void LoadingScreen::initScene(CCObject *pSender)
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	//-------Add Background Image
	CCSprite *bgImage = CCSprite::create(BACKGROUND_IMAGE);
    bgImage->setScale(GlobalClass::getScaleBySprite());
	bgImage->setPosition(GlobalClass::getCenterPointOfSize(screenSize));
	this->addChild(bgImage, 0);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(BACKGROUND_IMAGE);
	
	//-------Add Kite mania logo
	CCSprite *KMLogo = CCSprite::create(KITEMANIA_LOGO);
	KMLogo->setPosition( ccp(bgImage->getContentSize().width*0.5f, bgImage->getContentSize().height*0.9f));
	bgImage->addChild(KMLogo, 2);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(KITEMANIA_LOGO);

	//-------Add loading label
	CCLabelBMFont * ldgLabel = CCLabelBMFont::create("Loading...", GlobalClass::getMenuFont().c_str());
	ldgLabel->setPosition(GlobalClass::getCenterPointOfSize(screenSize));
	this->addChild(ldgLabel,1);
	
    this->runAction( CCSequence::create(CCDelayTime::create(0.1), CCCallFuncN::create(this,callfuncN_selector(LoadingScreen::loadGameAssestes)), NULL));
}

void LoadingScreen::loadGameAssestes(CCNode* sender)
{
    Music::sharedMusic()->preLoadBackgroundMusic();
	
	GameSettings *gSettings = GameSettings::sharedSetting();
    if(!gSettings->isStorageSet())
	{
		// Set the default classic mode
        gSettings->setMode(kTagClassic);
	
		// Set the default sound setting to ON
        gSettings->setMusic(kTagSoundOn);
    
		// Set the default language same as device if game not support device lang then we set english is default language
		switch (CCApplication::sharedApplication()->getCurrentLanguage())
		{
			case kLanguageEnglish:		gSettings->setLanguageType(kTagEnglish); break;
			case kLanguageSpanish:		gSettings->setLanguageType(kTagSpanish); break;
			case kLanguagePortuguese:	gSettings->setLanguageType(kTagPortuguese); break;
			case kLanguageFrench:		gSettings->setLanguageType(kTagFrench); break;
			case kLanguageGerman:		gSettings->setLanguageType(kTagGerman); break;
			default:					gSettings->setLanguageType(kTagEnglish); break;
		}
	}

	// Set the default game score
	gSettings->setScore(0);
	gSettings->setTempData();
//	Settings::loadFontFiles();
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_PROMPT, IMAGE_PNG_PROMPT);
	CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
//
	this->runAction( CCSequence::create(CCDelayTime::create(0.1), CCCallFuncN::create(this, callfuncN_selector(LoadingScreen::startGame)), NULL));
}

void LoadingScreen::startGame(CCNode* sender)
{
	CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
}

