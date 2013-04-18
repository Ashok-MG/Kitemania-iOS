//
//  GameOverScreen.cpp
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "GameOverScreen.h"
#include "GlobalClass.h"
#include "GameSettings.h"
#include "Resources.h"
#include "TranslateScreen.h"
#include "PlayScreen.h"

USING_NS_CC;

void GameOverScreen::keyBackClicked()
{
	CCLOG("click on back button");
	backToMainMenu(NULL);
}

CCScene* GameOverScreen::scene()
{
	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
    GameOverScreen *layer = GameOverScreen::create();
	
	// add layer as a child to scene
    scene->addChild(layer);
	
	// return the scene
    return scene;
}

GameOverScreen::GameOverScreen() {}

GameOverScreen::~GameOverScreen()
{
	removeAllChildrenWithCleanup(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(IMAGE_PLIST_OVER);
}

// on "init" you need to initialize your instance
bool GameOverScreen::init()
{
	//////////////////////////////
	// 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(37, 188, 241, 255)))
	{
        return false;
	}

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_OVER, IMAGE_PNG_OVER);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(IMAGE_PNG_OVER);

	this->setKeypadEnabled(true);
	
	initScene(NULL);

	return true;
}

void GameOverScreen::onExit()
{		
    CCLayer::onExit();
}

void GameOverScreen::onEnter()
{
    CCLayer::onEnter();
}

void GameOverScreen::initScene(CCNode* sender)
{
	screenSize = CCDirector::sharedDirector()->getWinSize();

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
		   
	//----Add top ten scores label
	CCLabelBMFont *topScrLbl = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_GO_TOP_TEN_SCORES), GlobalClass::getMessageFont().c_str());
	topScrLbl->setPosition( ccp( screenSize.width * 0.5f, screenSize.height * 0.79f));
	this->addChild(topScrLbl, 1);
    
	//-----Add score top heading bar
	addPlayerBar();
	
	GameSettings *gSettings = GameSettings::sharedSetting();
  
    UsersInfo *users = NULL;
    if(gSettings->getMode() == kTagClassic) 			//classic
        users = gSettings->topTenUsers();
	else if(gSettings->getMode() == kTagEndless) 		//endless
        users = gSettings->topTenUsersEndless();  
	else if(gSettings->getMode() == kTagTimer)			//timer mode
        users = gSettings->topTenUsersTimer(); 
          
    int totalUsers=sizeof(users)/sizeof(users[0]);
    CCLOG(" total users %d ",totalUsers);
        
	int count = 0;
    //------Add 10 player score board
    for(;count<10;count++)
	{
        if (users[count].score == 0) {
            addScoreBar(count+1, users[count].userName, users[count].score, " ", false); 
        }
		else{
            addScoreBar(count+1,users[count].userName, users[count].score, " ", true);
        }
    }    
	
	//----------Back to Main menu
	CCMenuItemSprite *mItmBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(LEADER_MENU_UP), CCSprite::createWithSpriteFrameName(LEADER_MENU_DOWN), this, menu_selector(GameOverScreen::backToMainMenu));
	mItmBack->setScale(GlobalClass::getScaleBySprite());

	//------Back to play
	CCMenuItemSprite *mItmPlay = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(LEADER_PLAY_UP), CCSprite::createWithSpriteFrameName(LEADER_PLAY_DOWN), this, menu_selector(GameOverScreen::backToPlay));
	mItmPlay->setScale(GlobalClass::getScaleBySprite());

    CCMenu *mMenus = CCMenu::create(mItmBack, mItmPlay, NULL);
	mMenus->alignItemsHorizontallyWithPadding(screenSize.width * 0.42f);
	mMenus->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.09f));
    this->addChild(mMenus, 1);

	if (gSettings->getScore() > 0 )
	{
		addScoreBar(count+1,gSettings->getUserName(), gSettings->getScore(), " ", true); 
		gSettings->setScore(0);
		mMenus->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.045f));
	}
}

void GameOverScreen::addPlayerBar()
{
	//-----Add player bar
	CCSprite *pBarBg = CCSprite::createWithSpriteFrameName(SCORE_HEAD_BAR);
	pBarBg->setPosition( ccp(screenSize.width * 0.5f, screenSize.height * 0.70f));
	pBarBg->setScale(GlobalClass::getScaleBySprite());
	this->addChild(pBarBg, 1);
    
	//add player label
	CCLabelTTF *playerLbl = CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_GO_RANK), FONT_SEGOECRG, GlobalClass::getScaledFont(30));
	playerLbl->setPosition( ccp( pBarBg->getContentSize().width * 0.11f, pBarBg->getContentSize().height * 0.5f));
	pBarBg->addChild(playerLbl, 0);
    
	//add Score label
	CCLabelTTF *scoreLbl = CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_GO_PLAYER), FONT_SEGOECRG, GlobalClass::getScaledFont(30));
	scoreLbl->setPosition( ccp( pBarBg->getContentSize().width * 0.28f, pBarBg->getContentSize().height * 0.5f));
    scoreLbl->setAnchorPoint( ccp(0, 0.5));
	pBarBg->addChild(scoreLbl, 0);

	//add Date label
	CCLabelTTF *dateLbl = CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_GO_SCORE), FONT_SEGOECRG, GlobalClass::getScaledFont(30));
	dateLbl->setPosition( ccp( pBarBg->getContentSize().width * 0.87f, pBarBg->getContentSize().height * 0.5f));
	pBarBg->addChild(dateLbl, 0);
}

void GameOverScreen::addScoreBar(int playerId, std::string playerName, int score, std::string date, bool isEmpty)
{
	//-----Add Score bar
	CCSprite *sBarBg = CCSprite::createWithSpriteFrameName(SCORE_DATA_BAR);
	sBarBg->setScale(GlobalClass::getScaleBySprite());
	sBarBg->setPosition( ccp(screenSize.width*0.5f, screenSize.height*0.7f - sBarBg->boundingBox().size.height*playerId));
	this->addChild(sBarBg, 1);

    if (isEmpty) 
	{
        //add rank
        char rnk[3];
	     if (playerId > 10) {
			 sprintf(rnk,"#");
		 }else{
			 sprintf(rnk,"%d",playerId);
		 }
        
		//add Rank label
		CCLabelTTF *rankLbl = CCLabelTTF::create(rnk, FONT_SEGOECRG, GlobalClass::getScaledFont(30));
		rankLbl->setPosition( ccp( sBarBg->getContentSize().width * 0.11f, sBarBg->getContentSize().height * 0.5f));
		rankLbl->setColor( ccc3(0,0,0));
		sBarBg->addChild(rankLbl, 0);

		//add player label
		CCLabelTTF *pNameLbl = CCLabelTTF::create(playerName.c_str(), FONT_SEGOECRG, GlobalClass::getScaledFont(30));
		pNameLbl->setPosition( ccp( sBarBg->getContentSize().width * 0.28f, sBarBg->getContentSize().height * 0.5f));
		pNameLbl->setColor( ccc3(0,0,0));
		pNameLbl->setAnchorPoint( ccp(0, 0.5));
		sBarBg->addChild(pNameLbl, 0);
        
		//add Score label
        char ch[10];
        sprintf(ch , "%d",score);
		CCLabelTTF *pScoreLbl = CCLabelTTF::create(ch, FONT_SEGOECRG, GlobalClass::getScaledFont(30));
		pScoreLbl->setPosition( ccp( sBarBg->getContentSize().width * 0.87f, sBarBg->getContentSize().height * 0.5f));
		pScoreLbl->setColor( ccc3(0,0,0));
		sBarBg->addChild(pScoreLbl, 0);
    }
}

void GameOverScreen::backToMainMenu(cocos2d::CCObject* sender)
{
    CCDirector::sharedDirector()->popScene();
}

void GameOverScreen::backToPlay(cocos2d::CCObject* sender)
{
    CCDirector::sharedDirector()->replaceScene(PlayScreen::scene());
}
