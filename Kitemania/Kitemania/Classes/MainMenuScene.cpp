//
//  MainMenuScene.cpp
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "MainMenuScene.h"
#include "Resources.h"
#include "GlobalClass.h"
#include "TranslateScreen.h"
#include "GameSettings.h"
#include "HelpScreen.h"
#include "PlayScreen.h"
#include "Music.h"

enum
{
	kTagMainMenu,
    kTagPlayMenu,
    kTagLangMenu,
    kTagModeMenu,
    kTagSoundMenu,
    kTagHelpMenu,
    kTagExitMenu,
    
    kTagPlayLbl,
    kTagLangLbl,
    kTagModeLbl,
    kTagSoundLbl,
    kTagHelpLbl,
    kTagExitLbl,
};

USING_NS_CC;

void MainMenuScene::keyBackClicked()
{
	CCLOG("click on back button");
	exitMenuMethod(NULL);
}

CCScene* MainMenuScene::scene()
{
	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
    MainMenuScene *layer = MainMenuScene::create();
	
	// add layer as a child to scene
    scene->addChild(layer);
	
	// return the scene
    return scene;
}

MainMenuScene::MainMenuScene() {}

MainMenuScene::~MainMenuScene()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(IMAGE_PLIST_MENU);
	removeAllChildrenWithCleanup(true);
}

bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(37, 188, 241, 255)))
	{
        return false;
	}

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_MENU, IMAGE_PNG_MENU);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(IMAGE_PNG_MENU);
	initScene(NULL);
	
	this->setKeypadEnabled(true);
	return true;
}

void MainMenuScene::onEnter()
{
	CCLayer::onEnter();
}

void MainMenuScene::onExit()
{
    CCLayer::onExit();
}

// on "init" you need to initialize your instance
void MainMenuScene::initScene(CCObject *pSender)
{
	screenSize = CCDirector::sharedDirector()->getWinSize();
	
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
    KMLogo->setScale(0.1f);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(KITEMANIA_LOGO);

	//----Add zoom animation with kite-mania logo
	KMLogo->runAction(CCScaleTo::create(0.5, 1.0));
	
	addGameMenuMethod();
    if(GameSettings::sharedSetting()->getMusic() == kTagSoundOn) Music::sharedMusic()->playBackgroundSound();
}

void MainMenuScene::addGameMenuMethod()
{
	//------------------play menu
	CCMenuItemSprite* mItmPlay = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(MENU_PLAY_UP), CCSprite::createWithSpriteFrameName(MENU_PLAY_DOWN), this, menu_selector(MainMenuScene::playGameMenuMethod));
    mItmPlay->setTag(kTagPlayMenu);
	mItmPlay->setScale(GlobalClass::getScaleBySprite());
	mItmPlay->setPosition( ccp(-screenSize.width*0.5f, screenSize.height*0.75f));
	
    CCLabelBMFont* label = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_PLAY), FONT_TW40);
    label->setPosition( ccp(mItmPlay->getContentSize().width * 0.15f, mItmPlay->getContentSize().height * 0.47f));
	label->setAnchorPoint( ccp(0, 0.5));
	mItmPlay->addChild(label, 0, kTagPlayLbl);
	
	//------------------Language menu
	CCMenuItemSprite* mItmLang = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(MENU_LANGUAGE_UP), CCSprite::createWithSpriteFrameName(MENU_LANGUAGE_DOWN), this, menu_selector(MainMenuScene::langMenuMethod));
	mItmLang->setTag(kTagLangMenu);
	mItmLang->setScale(GlobalClass::getScaleBySprite());
	mItmLang->setPosition( ccp(-screenSize.width*0.5f, screenSize.height*0.64f));

    label = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_LANGUAGE), FONT_TW40);
    label->setPosition( ccp(mItmLang->getContentSize().width * 0.15f, mItmLang->getContentSize().height * 0.47f));
    label->setAnchorPoint( ccp(0, 0.5));
	mItmLang->addChild(label, 0, kTagLangLbl);
	
	lStateLbl = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_LANG_VAL), FONT_TW40);
	lStateLbl->setPosition( ccp(mItmLang->getContentSize().width * 0.95f, mItmLang->getContentSize().height * 0.47f));
	lStateLbl->setAnchorPoint( ccp(1, 0.5));
	mItmLang->addChild(lStateLbl, 0);
	
	//------------------Mode menu
	CCMenuItemSprite* mItmMode = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(MENU_MODE_UP), CCSprite::createWithSpriteFrameName(MENU_MODE_DOWN), this, menu_selector(MainMenuScene::modeMenuMethod));
	mItmMode->setTag(kTagModeMenu);
	mItmMode->setScale(GlobalClass::getScaleBySprite());
	mItmMode->setPosition( ccp(-screenSize.width*0.5f, screenSize.height*0.53f));

    label = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE), FONT_TW40);
    label->setPosition( ccp(mItmMode->getContentSize().width * 0.15f, mItmMode->getContentSize().height * 0.47f));
    label->setAnchorPoint( ccp(0, 0.5));
	mItmMode->addChild(label, 0, kTagModeLbl);
	
    mStateLbl = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE_CLASSIC), FONT_TW40);
	mStateLbl->setPosition( ccp(mItmMode->getContentSize().width * 0.95f, mItmMode->getContentSize().height * 0.47f));
	mStateLbl->setAnchorPoint( ccp(1, 0.5));
	mItmMode->addChild(mStateLbl, 0);
        
	if (GameSettings::sharedSetting()->getMode() == kTagEndless) mStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE_ENDLESS));
    else if (GameSettings::sharedSetting()->getMode() == kTagTimer) mStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE_TIMER));
	
	//------------------Sound menu
	CCMenuItemSprite* mItmSound = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(MENU_SOUND_UP), CCSprite::createWithSpriteFrameName(MENU_SOUND_DOWN), this, menu_selector(MainMenuScene::soundMenuMethod));
	mItmSound->setTag(kTagSoundMenu);
	mItmSound->setScale(GlobalClass::getScaleBySprite());
	mItmSound->setPosition( ccp(-screenSize.width*0.5f, screenSize.height*0.42f));

    label = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_SOUND), FONT_TW40);
    label->setPosition( ccp(mItmSound->getContentSize().width * 0.15f, mItmSound->getContentSize().height * 0.47f));
    label->setAnchorPoint( ccp(0, 0.5));
	mItmSound->addChild(label, 0, kTagSoundLbl);
	
    sStateLbl = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_SOUND_ON), FONT_TW40);
	sStateLbl->setPosition( ccp(mItmSound->getContentSize().width * 0.95f, mItmSound->getContentSize().height * 0.47f));
	sStateLbl->setAnchorPoint( ccp(1, 0.5));
	mItmSound->addChild(sStateLbl, 0);

	if (GameSettings::sharedSetting()->getMusic() == kTagSoundOff) sStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_SOUND_OFF));
	
	//------------------help menu
	CCMenuItemSprite* mItmHelp = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(MENU_HELP_UP), CCSprite::createWithSpriteFrameName(MENU_HELP_DOWN), this, menu_selector(MainMenuScene::helpMenuMethod));
	mItmHelp->setTag(kTagHelpMenu);
    mItmHelp->setScale(GlobalClass::getScaleBySprite());
	mItmHelp->setPosition( ccp(-screenSize.width*0.5f, screenSize.height*0.31f));

    label = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_HELP), FONT_TW40);
    label->setPosition( ccp(mItmHelp->getContentSize().width * 0.15f, mItmHelp->getContentSize().height * 0.47f));
	label->setAnchorPoint( ccp(0, 0.5));
	mItmHelp->addChild(label, 0, kTagHelpLbl);
	
	//------------Exit menu
	CCMenuItemSprite* mItmExit = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(MENU_EXIT_UP), CCSprite::createWithSpriteFrameName(MENU_EXIT_DOWN), this, menu_selector(MainMenuScene::exitMenuMethod));
	mItmExit->setTag(kTagExitMenu);
    mItmExit->setScale(GlobalClass::getScaleBySprite());
	mItmExit->setPosition( ccp(-screenSize.width*0.5f, screenSize.height*0.2f));

    label = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_EXIT), FONT_TW40);
    label->setPosition( ccp(mItmExit->getContentSize().width * 0.15f, mItmExit->getContentSize().height * 0.47f));
	label->setAnchorPoint( ccp(0, 0.5));
	mItmExit->addChild(label, 0, kTagExitLbl);
	
	//---------add all menus to menu---------
	CCMenu *gameMenu = CCMenu::create(mItmPlay, mItmLang, mItmMode, mItmSound, mItmHelp, mItmExit, NULL);
	gameMenu->setPosition( CCPointZero);
	this->addChild(gameMenu, 1, kTagMainMenu);

    //run action-------------------
    CCActionInterval* move = CCMoveBy::create(2, CCPointMake(screenSize.width ,0));
    mItmPlay->runAction(CCSequence::create( CCEaseElasticInOut::create((CCActionInterval*)(move->copy()->autorelease()), 0.5f), NULL));
	
	move = CCMoveBy::create(2.2, CCPointMake(screenSize.width ,0));
    mItmLang->runAction(CCSequence::create( CCEaseElasticInOut::create((CCActionInterval*)(move->copy()->autorelease()), 0.5f), NULL));
	
	move = CCMoveBy::create(2.4, CCPointMake(screenSize.width ,0));
    mItmMode->runAction(CCSequence::create( CCEaseElasticInOut::create((CCActionInterval*)(move->copy()->autorelease()), 0.5f), NULL));
	
	move = CCMoveBy::create(2.6, CCPointMake(screenSize.width ,0));
    mItmSound->runAction(CCSequence::create( CCEaseElasticInOut::create((CCActionInterval*)(move->copy()->autorelease()), 0.5f), NULL));
	
	move = CCMoveBy::create(2.8, CCPointMake(screenSize.width ,0));
    mItmHelp->runAction(CCSequence::create( CCEaseElasticInOut::create((CCActionInterval*)(move->copy()->autorelease()), 0.5f), NULL));
	
	move = CCMoveBy::create(3, CCPointMake(screenSize.width ,0));
    mItmExit->runAction(CCSequence::create( CCEaseElasticInOut::create((CCActionInterval*)(move->copy()->autorelease()), 0.5f), NULL));
}

//PLAY GAME MENU METHOD
void MainMenuScene::playGameMenuMethod(cocos2d::CCObject* sender)
{
	if (GameSettings::sharedSetting()->getLanguageType() != pLangId)
	{
		GameSettings::sharedSetting()->setOldScore(0);
		GameSettings::sharedSetting()->setScore(0);
		GameSettings::sharedSetting()->setOldLife(4);
	}
	CCDirector::sharedDirector()->pushScene(PlayScreen::scene());
}

//PLAY Language MENU METHOD
void MainMenuScene::langMenuMethod(cocos2d::CCObject* sender)
{
	GameSettings* settings = GameSettings::sharedSetting();

	switch (settings->getLanguageType()) 
    {
        case kTagEnglish:		settings->setLanguageType(kTagSpanish);
            break;
		case kTagSpanish:		settings->setLanguageType(kTagPortuguese);
            break;
		case kTagPortuguese:	settings->setLanguageType(kTagFrench);
            break;
        case kTagFrench:		settings->setLanguageType(kTagGerman);
            break;
        case kTagGerman:		settings->setLanguageType(kTagEnglish);
            break;
    }

    lStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_LANG_VAL));

    CCNode* node = (CCNode*)this->getChildByTag(kTagMainMenu);
    CCNode* mNode = (CCNode*)node->getChildByTag(kTagPlayMenu);
    CCLabelBMFont* lNode = (CCLabelBMFont*)mNode->getChildByTag(kTagPlayLbl);
    lNode->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_PLAY));
    
    mNode = (CCNode*)node->getChildByTag(kTagLangMenu);
    lNode = (CCLabelBMFont*)mNode->getChildByTag(kTagLangLbl);
    lNode->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_LANGUAGE));
    
    mNode = (CCNode*)node->getChildByTag(kTagModeMenu);
    lNode = (CCLabelBMFont*)mNode->getChildByTag(kTagModeLbl);
    lNode->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE));

    mNode = (CCNode*)node->getChildByTag(kTagSoundMenu);
    lNode = (CCLabelBMFont*)mNode->getChildByTag(kTagSoundLbl);
    lNode->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_SOUND));

    mNode = (CCNode*)node->getChildByTag(kTagHelpMenu);
    lNode = (CCLabelBMFont*)mNode->getChildByTag(kTagHelpLbl);
    lNode->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_HELP));

    mNode = (CCNode*)node->getChildByTag(kTagExitMenu);
    lNode = (CCLabelBMFont*)mNode->getChildByTag(kTagExitLbl);
    lNode->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_EXIT));

    if (settings->getMode() == kTagClassic) mStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE_CLASSIC));
	else if (settings->getMode() == kTagEndless) mStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE_ENDLESS));
	else if (settings->getMode() == kTagTimer) mStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE_TIMER));
    
    if (settings->getMusic() == kTagSoundOn) sStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_SOUND_ON));
	else sStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_SOUND_OFF));
}

//PLAY Sound MENU METHOD
void MainMenuScene::modeMenuMethod(cocos2d::CCObject* sender)
{
	GameSettings* settings = GameSettings::sharedSetting();
	if (settings->getMode() == kTagClassic)
	{
		settings->setMode(kTagEndless);
		mStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE_ENDLESS));
	}
	else if (settings->getMode() == kTagEndless)
	{
		settings->setMode(kTagTimer);
		mStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE_TIMER));
	}
	else if (settings->getMode() == kTagTimer)
	{
		settings->setMode(kTagClassic);
		mStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_MODE_CLASSIC));
    }
}

//PLAY Sound MENU METHOD
void MainMenuScene::soundMenuMethod(cocos2d::CCObject* sender)
{
    GameSettings* settings = GameSettings::sharedSetting();
	if (settings->getMusic() == kTagSoundOn)
	{
		settings->setMusic(kTagSoundOff);
		sStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_SOUND_OFF));
		Music::sharedMusic()->muteGameSound();
	}
	else if (settings->getMusic() == kTagSoundOff)
	{
		settings->setMusic(kTagSoundOn);
		sStateLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_MENU_SOUND_ON));
		Music::sharedMusic()->resumeGameSound();
	}
}

//PLAY HELP MENU METHOD
void MainMenuScene::helpMenuMethod(cocos2d::CCObject *sender)
{
    CCDirector::sharedDirector()->pushScene(HelpScreen::scene());
}

void MainMenuScene::exitMenuMethod(cocos2d::CCObject *sender)
{	
	Music* gameMusic = Music::sharedMusic();
	CC_SAFE_DELETE(gameMusic);

    CCDirector::sharedDirector()->end();
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	exit(0); 
}
