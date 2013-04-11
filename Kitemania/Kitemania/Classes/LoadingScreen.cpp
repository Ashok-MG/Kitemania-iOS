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

//	if (CC_GLVIEW::sharedOpenGLView().isIpad()) {
//		KMLogo->setPosition( ccp(bgImage->getContentSize().width*0.5f, bgImage->getContentSize().height*0.85f));
//	}
	
	//-------Add loading label
//	CCLabelBMFont * ldgLabel = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_LOADING), Settings::getMenuFont().c_str());
//	ldgLabel->setPosition(GlobalClass::getCenterPointOfSize(screenSize));
//	this->addChild(ldgLabel,1);
	
    this->runAction( CCSequence::create(CCDelayTime::create(0.1), CCCallFuncN::create(this,callfuncN_selector(LoadingScreen::loadGameAssestes)), NULL));
}

void LoadingScreen::loadGameAssestes(CCNode* sender)
{
//    Music *gMusic=Music::getInstance();
//	gMusic->preLoadBackgroundMusic(); 
//	
//	GameSettings *gSettings = GameSettings::getInstance();
//    if(!gSettings->isStorageSet())
//	{
//        gSettings->setMode(0); // classic
//        gSettings->setMusic(0); //musin on
//        
//        int currentLang = 0;
//        switch (CCApplication::getCurrentLanguage()) {
//            case 0://English
//                currentLang = 0;
//                break;
//            case 2://French
//                currentLang = 3;
//                break;
//            case 4://German
//                currentLang = 4;
//                break;
//            case 5://Spanish
//                currentLang = 1;
//                break;
//            case 6://Russian
//                currentLang = 2;
//                break;
//            default:
//                currentLang = 0;
//                break;
//		}
//	}
//		
//	gSettings->setScore(0);
//	gSettings->setTempData();
////	Settings::loadFontFiles();
//
//	CCSprite *sp = CCSprite::spriteWithFile(TRANSPARENT_IMAGE);
//	sp = CCSprite::spriteWithFile(KITEMANIA_LOGO);
//	sp = CCSprite::spriteWithFile(PLAY_BG_IMAGE);
	
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_MENU, IMAGE_PNG_MENU);
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_GAME, IMAGE_PNG_GAME);
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_LIGHT, IMAGE_PNG_LIGHT);
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_OVER, IMAGE_PNG_OVER);
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_CIRCEL, IMAGE_PNG_CIRCEL);
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_EFFECT, IMAGE_PNG_EFFECT);
//	CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();

	this->runAction( CCSequence::create(CCDelayTime::create(0.1), CCCallFuncN::create(this, callfuncN_selector(LoadingScreen::startGame)), NULL));
}

void LoadingScreen::startGame(CCNode* sender)
{
	CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
}

