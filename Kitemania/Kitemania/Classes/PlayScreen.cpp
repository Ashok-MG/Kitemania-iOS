//
//  PlayScreen.cpp
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "PlayScreen.h"
#include "GlobalClass.h"
#include "Resources.h"
#include "TranslateScreen.h"
#include "InputUserName.h"
#include "GameSettings.h"
#include "Music.h"
#include <fstream>

#define kMaxCloudMoveDuration 36.0f
#define kMinCloudMoveDuration 24.0f
#define kMaxBirdMoveDuration  10
#define kMinBirdMoveDuration  4
#define kBallMinMoveDuration 14
#define kBallMaxMoveDuration 20
#define kMaxAirHotMoveDuration 25
#define kMinAirHotMoveDuration 15

enum
{
	kTagSpriteScore = 30,
	kTagSpriteScoreLbl,
	kTagSpriteSubmit,
    kTagSpriteBack,
    kTagSpriteKite,    
    kTagSpriteBirds,
    kTagSpriteBall,
    kTagBgImage,
	kTagTimeLbl,
	kTagMenus,
	kTagScorePanel,
    kTagBonusPointWord,
};

enum {
    kObstacleAirhot = 10,
    kObstacleBird,
    kObstacleThunder,
};

enum {
    kStateGamePlay = 0,
    kStateQuitMenu,
    kStateShowScoreBoard,
}; 

USING_NS_CC;

void PlayScreen::keyBackClicked()
{
	if (backButtonState == kStateGamePlay) 
	{
		showScoreBoard(this);
		return;
	}
	else if (backButtonState == kStateQuitMenu){
		clickOnCancelButton(this);
		return;
	}
}

CCScene* PlayScreen::scene()
{
	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
    PlayScreen *layer = PlayScreen::create();

	// add layer as a child to scene
    scene->addChild(layer);
	
	// return the scene
    return scene;
}

PlayScreen::PlayScreen(){}

PlayScreen::~PlayScreen()
{
	if (_leftKites) CC_SAFE_RELEASE_NULL(_leftKites);
	if (_colWord) CC_SAFE_RELEASE_NULL(_colWord);
	if (_bubbles) CC_SAFE_RELEASE_NULL(_bubbles);
	if (_obstacles)	CC_SAFE_RELEASE_NULL(_obstacles);
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(IMAGE_PLIST_GAME);
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(IMAGE_PLIST_LIGHT);
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(IMAGE_PLIST_EFFECT);
	
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    unscheduleAllSelectors();
	removeAllChildrenWithCleanup(true);
}

// on "init" you need to initialize your instance
bool PlayScreen::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(50, 169, 234, 255)))
	{
		return false;
	}
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_GAME, IMAGE_PNG_GAME);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(IMAGE_PNG_GAME);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_LIGHT, IMAGE_PNG_LIGHT);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(IMAGE_PNG_LIGHT);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_EFFECT, IMAGE_PNG_EFFECT);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(IMAGE_PNG_EFFECT);
	
	this->setKeypadEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);

	initScene(this);

	return true;
}

void PlayScreen::onExit()
{
    CCLayer::onExit();
}

void PlayScreen::onEnter()
{
	CCLayer::onEnter();
}

void PlayScreen::initScene(CCNode* sender)
{
	//-----
	_leftKites = new CCArray();
	_colWord = new CCArray();
	_obstacles = new CCArray();
	_bubbles = new CCArray();
	
//	wordList = NULL;

	wordChar[0] = '\0';
	isRunningTick = false;
	isFlicking = false;
	movedBallCounter = 0;
	ci = 0;

	screenSize = CCDirector::sharedDirector()->getWinSize();

	//-------Add Background Image
	CCSprite *bgImage = CCSprite::create(PLAY_BG_IMAGE);
    bgImage->setScale(GlobalClass::getScaleByScreen());
	bgImage->setAnchorPoint(ccp(0.5, 0));
	bgImage->setPosition( ccp(screenSize.width*0.5, 0));
	this->addChild(bgImage, 0, kTagBgImage);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(PLAY_BG_IMAGE);
	
	//-------Add Kite wall
	CCSprite *kiteWall = CCSprite::createWithSpriteFrameName(KITE_WALL);
	kiteWall->setScale(GlobalClass::getScaleByScreen());
	kiteWall->setAnchorPoint(ccp(0.5, 0));
	kiteWall->setPosition( ccp(screenSize.width*0.5, 0));
	this->addChild(kiteWall, 5);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(KITE_WALL);

	//----------Back to Main menu
	CCMenuItemSprite *mItmBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(BACK_UP), CCSprite::createWithSpriteFrameName(BACK_DOWN), this, menu_selector(PlayScreen::showScoreBoard));
	mItmBack->setPosition( ccp(screenSize.width * 0.065f, screenSize.width * 0.05f));
	mItmBack->setScale(GlobalClass::getScaleBySprite());
	
	//------Add submit 
	CCMenuItemSprite *mItmSubmit = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(SUBMIT_UP), CCSprite::createWithSpriteFrameName(SUBMIT_DOWN), this, menu_selector(PlayScreen::clickOnSubmitBtn));
	mItmSubmit->setPosition( ccp(screenSize.width * 0.935f, screenSize.width * 0.05f));
	mItmSubmit->setScale(GlobalClass::getScaleBySprite());

    CCMenu *mMenus = CCMenu::create(mItmBack, mItmSubmit, NULL);
    mMenus->setPosition(CCPointZero);
    this->addChild(mMenus, 6, kTagMenus);
	
	//------Add flaying kite kRed.png 
	kite = CCSprite::createWithSpriteFrameName("kRed.png");
	kite->setScale(GlobalClass::getScaleByScreen());
	kite->setPosition( ccp(screenSize.width * 0.5f, screenSize.width * 0.31f));
	this->addChild(kite, 4, kTagSpriteKite);
	
	//-----call add score method
	addGameScores();
	
	//-----call add cloud method
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*8 + 4), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveClouds)), NULL));

	if (GameSettings::sharedSetting()->getMode() == kTagClassic)
	{
		//----call add life method
		addGameLife();
		
		//-----call add flaying birds
		this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*8 + 10), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveBirds)), NULL));
	
		//-----call add lights
		this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*10 + 30), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addThunderbolt)), NULL));
	
		//-----call add airhot
		this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*8 + 19), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveAirHot)), NULL));
	} 
	else if (GameSettings::sharedSetting()->getMode() == kTagTimer)
	{
		//----call add timer method
		addGameTimer();
	}
    
    this->runAction(CCSequence::create(CCDelayTime::create(0.1), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::getAllAlpha)), CCDelayTime::create(0.1), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::getAllWordFromDictionary)), CCDelayTime::create(0.1), NULL));
				
//	//------call add balls
//	schedule(schedule_selector(PlayScreen::addMoveBalls), 2.5f); 
//
//	//-----chicking collision
//	schedule(schedule_selector(PlayScreen::tick));
	
	Music *gMusic = Music::sharedMusic();
	if(GameSettings::sharedSetting()->getMusic() == kTagSoundOn)
	{
		gMusic->preloadEffectSound(SFX_BIRD_HIT);
		gMusic->preloadEffectSound(SFX_SUBMIT_CORRECT);
		gMusic->preloadEffectSound(SFX_WORK_FLICK);
		gMusic->preloadEffectSound(SFX_WORD_PICK);
		gMusic->preloadEffectSound(SFX_AIRHOT_HIT);
		gMusic->preloadEffectSound(SFX_LIGHT_HIT);
	}
    
    optionalBubble = NULL;
    optionalLabel = NULL;
    backButtonState = kStateGamePlay;
}

void PlayScreen::addMoveClouds(float t)
{	
	//Get random clouds - there are three clouds
    int cloudToDraw = CCRANDOM_0_1() * 3 + 1;

	char cloudName[20];
	sprintf(cloudName, "cloud%d.png", cloudToDraw);
	CCSprite *cloud = CCSprite::createWithSpriteFrameName(cloudName);
	cloud->setScale(GlobalClass::getScaleBySprite());
    this->addChild(cloud, CCRANDOM_0_1() * 2 + 1); //Get random anchor point for clouds

	CCSize cSize = cloud->boundingBox().size;
	
	//Get random position of X - cloud moving from left to right or right to left
	float offScreenXPosition = cSize.width/2 * -1;
    float xPosition = screenSize.width + cSize.width/2;
    if (CCRANDOM_0_1() * 2 < 1) CC_SWAP(offScreenXPosition, xPosition, float);
	
	//Get random position of Y - excluding the bottom wall area
    int yPosition = CCRANDOM_0_1() * (screenSize.height*0.7f - cSize.height) + screenSize.height*0.3f + cSize.height/2;
    
	//Get random speed
    float moveDuration = CCRANDOM_0_1() * (kMaxCloudMoveDuration - kMinCloudMoveDuration) + kMinCloudMoveDuration;
	
	cloud->setPosition(ccp(xPosition, yPosition));
    cloud->runAction( CCSequence::create( CCMoveTo::create(moveDuration, ccp(offScreenXPosition, cloud->getPositionY())), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroySprites)), NULL));
	
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*8 + 4), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::addMoveClouds)), NULL));
}

void PlayScreen::addMoveAirHot(float t)
{	
    CCSprite *airHot = CCSprite::createWithSpriteFrameName(HOT_BALLON);
    airHot->setScale(GlobalClass::getScaleBySprite());
	airHot->setTag(kObstacleAirhot);
	this->addChild(airHot, CCRANDOM_0_1() * 2 + 1);

	CCSize aSize = airHot->boundingBox().size;
	
	//Get random position of X - airhot moving from left to right or right to left
	float offScreenXPosition = aSize.width/2 * -1;
    float xPosition = screenSize.width + aSize.width/2;
    if (CCRANDOM_0_1() * 2 < 1) CC_SWAP(offScreenXPosition, xPosition, float);

	//Get random position of Y - excluding the bottom wall area
    int yPosition = CCRANDOM_0_1() * (screenSize.height*0.8f - aSize.height*1.5f) + screenSize.height*0.2f + aSize.height/2;
    
	//Get random speed
    float moveDuration = CCRANDOM_0_1() * (kMaxAirHotMoveDuration - kMinAirHotMoveDuration) + kMinAirHotMoveDuration;
    
    airHot->setPosition(ccp(xPosition, yPosition));
    airHot->runAction( CCSequence::create( CCMoveTo::create(moveDuration, ccp(offScreenXPosition, airHot->getPositionY() + airHot->boundingBox().size.height)), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::destroyObstacles)), NULL ) );
	
	_obstacles->addObject(airHot);
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*8 + 19), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::addMoveAirHot)), NULL));
}

void PlayScreen::addMoveBirds(float t)
{
	CCArray* flyFrames = CCArray::createWithCapacity(4);
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    CCSprite *bird = CCSprite::createWithSpriteFrameName("bird1.png");
	bird->setScale(GlobalClass::getScaleBySprite());
	bird->setTag(kObstacleBird);
    this->addChild(bird, CCRANDOM_0_1() * 2 + 1, kTagSpriteBirds);

    char buff[20] = {0};
	for(int i=1; i<=4; i++)
	{
        sprintf(buff, "bird%d.png", i);
        flyFrames->addObject( cache->spriteFrameByName(buff));
	}
    
	//set animation object here
    CCAnimation *animation = NULL;
    if (flyFrames->count() > 0) animation = CCAnimation::createWithSpriteFrames(flyFrames, 0.2);
	
	CCSize bSize = bird->boundingBox().size;
	
	//Get random position of X - birds moving from left to right or right to left
	float offScreenXPosition = bSize.width/2 * -1;
    float xPosition = screenSize.width + bSize.width/2;
    if (CCRANDOM_0_1() * 2 < 1)
	{
		CC_SWAP(offScreenXPosition, xPosition, float);
		bird->setFlipX(true);
	}
    
	//Get random position of Y - excluding the bottom wall area
    int yPosition = CCRANDOM_0_1() * (screenSize.height*0.8f - bSize.height) + screenSize.height*0.2f + bSize.height/2;
    
	//Get random speed
    float moveDuration = CCRANDOM_0_1() * (kMaxBirdMoveDuration - kMinBirdMoveDuration) + kMinBirdMoveDuration;

    bird->setPosition(ccp(xPosition, yPosition));
    bird->runAction( CCRepeatForever::create( CCAnimate::create(animation)));
    bird->runAction( CCSequence::create( CCMoveTo::create(moveDuration, ccp(offScreenXPosition, bird->getPositionY())), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroyObstacles)), NULL));
    
	CC_SAFE_RELEASE_NULL(flyFrames);
    _obstacles->addObject(bird);
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*8 + 10), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::addMoveBirds)), NULL));
}

void PlayScreen::addThunderbolt(float t)
{
	CCArray* lightFrames = CCArray::createWithCapacity(17);
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    CCSprite *light = CCSprite::createWithSpriteFrameName("lighting1.png");
	light->setScale(GlobalClass::getScaleBySprite());
	light->setTag(kObstacleThunder);
	this->addChild(light, CCRANDOM_0_1() * 2 + 1);

    char buff[25] = {0};
	for(int i=1; i<=17; i++)
	{
        sprintf(buff, "lighting%d.png", i);
        lightFrames->addObject( cache->spriteFrameByName(buff));
	}
    
	//set animation object here
	CCAnimation *animation;
    if (lightFrames->count() > 0)
	{
        animation = CCAnimation::createWithSpriteFrames(lightFrames, 0.1);
	}
		
	//Get random position of X 
	int xPosition = CCRANDOM_0_1() * (screenSize.width - light->boundingBox().size.width*2) + light->boundingBox().size.width;
	
	//Get random position of Y
	int yPosition = CCRANDOM_0_1() * (screenSize.height*0.5f) + screenSize.height*0.5f - light->boundingBox().size.height;
	
    light->setPosition(ccp(xPosition, yPosition));
    light->runAction( CCSequence::create( CCAnimate::create(animation), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroyObstacles)), NULL));
	
    CC_SAFE_RELEASE_NULL(lightFrames);
	_obstacles->addObject(light);
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*10 + 30), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::addThunderbolt)), NULL));
}

void PlayScreen::addGameLife()
{
	//-----Add 4 life line
    for (int i=0; i < GameSettings::sharedSetting()->getOldLife(); i++)
	{
		CCSprite *life = CCSprite::createWithSpriteFrameName(GAME_KITE);
		life->setScale(GlobalClass::getScaleBySprite());
		life->setAnchorPoint(ccp(1, 0.5));
		life->setPosition( ccp(screenSize.width*0.965f - (life->boundingBox().size.width*1.1f*i), screenSize.height*0.95f));
        this->addChild(life, 6);
		
        _leftKites->addObject(life);
	}
	GameSettings::sharedSetting()->setOldLife(4);
}

void PlayScreen::addGameScores()
{
	//-------Add score label
	CCLabelBMFont *scoreTitle = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_SCORE), GlobalClass::getHelpMessageFont().c_str());
	scoreTitle->setAnchorPoint(ccp(0, 0.5));
	scoreTitle->setPosition(ccp(screenSize.width*0.03f, screenSize.height*0.95f));
	this->addChild(scoreTitle, 6, kTagSpriteScoreLbl);
    
	char scr[10] = "0";
	GameSettings *gSettings = GameSettings::sharedSetting();
	if (gSettings->getMode() == kTagClassic)
	{
		sprintf(scr, "%d", gSettings->getOldScore());
		gSettings->setOldScore(0);
	}
	else gSettings->setScore(0);
	
    CCLabelBMFont *scLabel = CCLabelBMFont::create(scr, GlobalClass::getHelpMessageFont().c_str());
	scLabel->setPosition( ccp(scoreTitle->boundingBox().size.width + scoreTitle->boundingBox().origin.x, screenSize.height*0.95f));
	scLabel->setColor( ccc3(255, 255, 0));
	scLabel->setAnchorPoint( ccp(0, 0.5));
	this->addChild(scLabel, 6, kTagSpriteScore);
}

void PlayScreen::addGameTimer()
{
	m_time = 90;
	char timer[10] = {0};
	sprintf(timer, "%s%.0f", TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_TIMER), m_time);

	CCLabelBMFont *timeLabel = CCLabelBMFont::create(timer, GlobalClass::getHelpMessageFont().c_str());
	timeLabel->setPosition(ccp(screenSize.width*0.97f, screenSize.height*0.95f));
	timeLabel->setAnchorPoint(ccp(1, 0.5));
	this->addChild(timeLabel, 6, kTagTimeLbl);
}

void PlayScreen::addMoveBalls(float t)
{
		//-----Get image of ball.png that have in GameScreen_Ass.plist file
	CCSprite* ball = CCSprite::createWithSpriteFrameName(BALL_IMAGE);
	ball->setScale( GlobalClass::getScaleByScreen()*0.82f);
    
	CCSprite* ball2 = CCSprite::createWithSpriteFrameName(BALL_IMAGE);
	ball2->setScale( GlobalClass::getScaleByScreen()*0.82f);
    
	CCSize bSize = ball->boundingBox().size;
	
		////ball 111
    int n = getRandomLettor();
	
    ball->setTag(n);
    char alphabet[1];
    sprintf(alphabet,"%c", n);
    
	CCLabelTTF *alphabetLabel = CCLabelTTF::create(CCApplication::sharedApplication()->getUTF8String(alphabet), FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setColor( ccc3(0, 0, 0));
		//------------------------------------------
    
		//ball2
    srand((unsigned int)(time(NULL)+n));
    n = getRandomLettor();
	
	ball2->setTag(n);
    sprintf(alphabet,"%c",n);
	CCLabelTTF *alphabetLabel2 = CCLabelTTF::create(CCApplication::sharedApplication()->getUTF8String(alphabet), FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel2->setColor( ccc3(0, 0, 0));
    
		//---------------------------------------
    
    float minX = bSize.width/2;
    float maxX = screenSize.width - (bSize.width/2);
	
    ball->addChild(alphabetLabel);
    ball2->addChild(alphabetLabel2);
    
    alphabetLabel->setPosition( ccp(ball->getContentSize().width/2, ball->getContentSize().height/2));
    alphabetLabel2->setPosition( ccp(ball2->getContentSize().width/2, ball2->getContentSize().height/2));
    
    int rangeX=(int)(maxX - minX);
	int actualX = rand()%rangeX + minX;
    
	ball->setPosition( ccp(actualX,screenSize.height + (bSize.height/2)));
	this->addChild(ball, 3);
    
    
    srand((unsigned int)(time(NULL)+n+rangeX+actualX));
    
	int moveDuration=rand()%kBallMaxMoveDuration+1;
    if (moveDuration < kBallMinMoveDuration) {
        moveDuration = kBallMinMoveDuration;
    }
    
    srand((unsigned int)(time(NULL)+n));
	int actualX2 = rand()%rangeX + minX;
    
    float diff = abs(actualX2-actualX);
	
    if(diff < bSize.width)
		{
        actualX2 = actualX2 + 2*bSize.width;
        if(maxX <= actualX2) {
            actualX2 = actualX2 - 2*bSize.width;
        }
		}
    
	ball2->setPosition( ccp(actualX2,screenSize.height + (bSize.height/2)));
	this->addChild(ball2, 3);
    
    
    srand((unsigned int)(time(NULL)+n+rangeX+actualX2));
    
	int moveDuration2=rand()%kBallMaxMoveDuration+1;
    if (moveDuration2 < kBallMinMoveDuration) {
        moveDuration2 = kBallMinMoveDuration;
    }
    
    if(moveDuration2==moveDuration) {
        moveDuration2=moveDuration2-4;
    }
    
    CCFiniteTimeAction* actionMove = CCMoveTo::create(moveDuration,ccp(actualX, 0 - bSize.height/2) );
    ball->runAction(CCSequence::create(actionMove, CCCallFuncN::create(this,callfuncN_selector(PlayScreen::destroyBalls)), NULL));
    
    CCFiniteTimeAction* actionMove2 = CCMoveTo::create(moveDuration2,ccp(actualX2, 0 - bSize.height/2) );
    ball2->runAction(CCSequence::create(actionMove2, CCCallFuncN::create(this,callfuncN_selector(PlayScreen::destroyBalls)), NULL));
    
    srand((unsigned int)(time(NULL))/4);
    if (rand()%4 == 2) {
        if (optionalBubble == NULL)
			{
            optionalBubble = ball;
            optionalLabel = alphabetLabel;
            schedule(schedule_selector(PlayScreen::changeOptionalBallChar), 1.0f);
			}
	}
    
    _bubbles->addObject(ball);
    _bubbles->addObject(ball2);
}

void PlayScreen::changeOptionalBallChar(float t)
{
	int n = getRandomLettor();
	
	char alphabet[1];
	optionalBubble->setTag(n);
	sprintf(alphabet,"%c",n);
    optionalLabel->setString(CCApplication::sharedApplication()->getUTF8String(alphabet));
}

void PlayScreen::tick(float dt)
{
	if (GameSettings::sharedSetting()->getMode() == kTagTimer)
	{
		m_time -= dt;
		
		if (m_time <= 0) {
			clickOnYesButton(this);
			return;
		}
	
		char timer[10] = {0};
		sprintf(timer, "%s%.0f", TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_TIMER), m_time);

		CCLabelBMFont *tLbl = (CCLabelBMFont*)this->getChildByTag(kTagTimeLbl);
		tLbl->setString(timer);
	}
	
	if (isRunningTick == true) return;
	
	if (kite)
	{
		if (kite->getPosition().y > 0)
		{
			float decWidth = kite->boundingBox().size.width * 0.1f;
			CCRect kiteRect = CCRect(kite->boundingBox().origin.x + decWidth, kite->boundingBox().origin.y + decWidth, kite->boundingBox().size.width - decWidth * 2, kite->boundingBox().size.height - decWidth * 2);
			
			//---Bubble collosion
			for (int i=0; i<_bubbles->count(); i++)
			{
                CCSprite *bubble = (CCSprite*)_bubbles->objectAtIndex(i);
				decWidth = bubble->boundingBox().size.width * 0.1f;
				CCRect bubbleRect = CCRect(bubble->boundingBox().origin.x + decWidth, bubble->boundingBox().origin.y + decWidth, bubble->boundingBox().size.width - decWidth * 2, bubble->boundingBox().size.height - decWidth * 2);
				
                if (bubbleRect.intersectsRect(kiteRect))
				{
					unsigned int wordCount = strlen(wordChar);
					
					if (wordCount < 8)
					{
						float bSizeW = bubble->boundingBox().size.width * 1.105;
						isFlicking = true;
						movedBallCounter++;
						
						//play letter sound
						if(GameSettings::sharedSetting()->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_WORD_PICK);
						
						_colWord->addObject(bubble);
						if (wordChar[0]=='\0') sprintf(wordChar, "%c",bubble->getTag());
						else sprintf(wordChar ,"%s%c",wordChar,bubble->getTag());
						
						bubble->stopAllActions();
						_bubbles->removeObject(bubble);
						
						if (bubble==optionalBubble)
						{
							optionalBubble = NULL;
							optionalLabel = NULL;
							unschedule(schedule_selector(PlayScreen::changeOptionalBallChar));
						}
						
						this->moveBubble(bubble->boundingBox().origin, ccp(screenSize.width * 0.089f + _colWord->count() * bSizeW, screenSize.width * 0.052f), bubble);
					}
					break;
				}
			}
            
			//----Obstacles collision
            for (int i=0; i<_obstacles->count(); i++)
			{
				CCSprite *birds = (CCSprite*)_obstacles->objectAtIndex(i);
				
				decWidth = birds->boundingBox().size.width * 0.2f;
				CCRect birdsRect = CCRect(birds->boundingBox().origin.x + decWidth, birds->boundingBox().origin.y + decWidth, birds->boundingBox().size.width - decWidth * 2, birds->boundingBox().size.height - decWidth * 2);
				
                if (birdsRect.intersectsRect(kiteRect))
				{
					//play bird-hit sound
					if(GameSettings::sharedSetting()->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_BIRD_HIT);
					resetKitePosition(this);
                    break;
				}
			}
		}
	}
}


bool PlayScreen::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void PlayScreen::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint location = pTouch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	
	if (location.y < screenSize.height * 0.166f || isRunningTick == true) {
		return;
	}
	
	kite->stopAllActions();
    
	if (location.x<0) {
        location.x=0;
    }
    
    float o = location.x - kite->getPosition().x;
	float a = location.y - kite->getPosition().y;
	float at = (float) CC_RADIANS_TO_DEGREES( atanf( o/a) );
    
	if( a < 0 ) 
	{
		if(  o < 0 )
			at = 180 + fabs(at);
		else
			at = 180 - fabs(at);	
	}
    
    o=abs((int)o);
    a=abs((int)a);
    
    
    double area=sqrt(o*o+a*a);
    
    if (area>0 && area <1000 ) {
        
    }
    else
	{
        area= 0.1;
	}
	
//	CCLOG("o %f a %f",o,a);
	if (o == 0 && a == 0) 
	{
		kite->runAction( CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(PlayScreen::kiteFall)), NULL));
		return;
	}
	
    kite->runAction( CCSequence::create(CCRotateTo::create(0.2, at), CCMoveTo::create(area/400, location), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::kiteFall)), NULL));
} 

void PlayScreen::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint location = touch->getLocationInView();
    location =CCDirector::sharedDirector()->convertToGL(location);

	
    CCRect pointRect = CCRectMake(location.x, location.y, 1, 1);
	    
	if (isFlicking == true) return;
	
	int count= _colWord->count();
    for (int i=0; i < count; i++)
	{
		CCNode *temSprite=(CCNode *)_colWord->objectAtIndex(i);
		
        if (pointRect.intersectsRect(temSprite->boundingBox()))
		{
            //flick sound
			if(GameSettings::sharedSetting()->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_WORK_FLICK);

			_colWord->removeObject((CCSprite*)temSprite);
			temSprite->runAction( CCSequence::create( CCMoveTo::create(0.1 , CCPointMake(temSprite->getPosition().x, temSprite->getPosition().y + 20) ), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::destroySprites)), NULL));
		
			count= _colWord->count();
			float bSizeW = temSprite->boundingBox().size.width * 1.105;

            for (int j=i; j<count; j++) 
			{
				isRunningTick = true;

                wordChar[j] = wordChar[j+1];
				temSprite = (CCNode *) _colWord->objectAtIndex(j);
				temSprite->runAction( CCSequence::create( CCMoveTo::create(0.1 , CCPointMake(screenSize.width * 0.089f + ((j+1) * bSizeW), temSprite->getPositionY())), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::setIsKiteBlinkingEnd)), NULL));
            }
			wordChar[count] = '\0';
			
            break;
		}
	}
}

//touch events methods ended
void PlayScreen::kiteFall(CCNode* sender)
{
	//compare co-ordinates
	float sp = kite->getPositionY() / 50;
	CCActionInterval *move = CCMoveTo::create(sp, ccp(kite->getPositionX(), 0));
	kite->runAction( CCSequence::create(move, CCCallFuncN::create(this, callfuncN_selector(PlayScreen::resetKitePosition)), NULL));
}

void PlayScreen::resetKitePosition(CCNode *sender)
{
	kite->stopAllActions();
	isRunningTick = true;
	
    kite->runAction( CCSequence::create(CCBlink::create(0.5, 3), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::setIsKiteBlinking)), NULL));
}

void PlayScreen::setIsKiteBlinking(CCNode* sender)
{
	kite->setPosition( ccp(screenSize.width*0.5f, screenSize.width*0.31f));
	kite->setRotation(0);
	kite->runAction( CCSequence::create( CCFadeIn::create(1), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::setIsKiteBlinkingEnd)), NULL));

    if (GameSettings::sharedSetting()->getMode() == kTagClassic)
	{
		if (_leftKites) 
		{
			if (_leftKites->count() > 1) 
			{
				this->removeChild((CCSprite*)_leftKites->lastObject(), true);
				_leftKites->removeLastObject();
			}
			else
			{
			    if (GameSettings::sharedSetting()->getScore() > 0)
					CCDirector::sharedDirector()->replaceScene(InputUserName::scene());
				else
                    CCDirector::sharedDirector()->popScene();
            }
		}
	}
}

void PlayScreen::setIsKiteBlinkingEnd(CCNode* sender)
{
	isRunningTick = false;
}

void PlayScreen::showMessage(std::string msg, bool status)
{
    CCLabelBMFont *lbl = CCLabelBMFont::create(msg.c_str(), GlobalClass::getMessageFont().c_str());
    lbl->setPosition( ccp(screenSize.width/2,screenSize.height/2));
    lbl->setColor( ccc3(255, 255, 255));
    this->addChild(lbl,7);
    
    CCActionInterval*  action = CCFadeOut::create(2.0f);
    lbl->runAction( CCSequence::create(action, CCCallFuncN::create(this,callfuncN_selector(PlayScreen::destroySprites)),NULL));
    
	if (status == false) 
	{
		this->runAction(CCSequence::create(CCCallFuncN::create(this,callfuncN_selector(PlayScreen::addBonusPointsWord)), NULL));
	
		CCSprite *submit = CCSprite::createWithSpriteFrameName(SUBMIT_WRONG);
		submit->setScale(GlobalClass::getScaleBySprite());
		submit->setPosition( ccp(screenSize.width * 0.935f, screenSize.width * 0.05f));
		this->addChild(submit, 6);
		submit->runAction(CCSequence::create(CCDelayTime::create(3), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroySprites)), NULL));
	
		for (unsigned int i=0 ; i<_colWord->count(); i++)
		{
			this->removeChild((CCSprite*)_colWord->objectAtIndex(i), true);
		}
		
		movedBallCounter = 0;
		isFlicking = false;
		_colWord->removeAllObjects();
		wordChar[0]='\0';
	}
}

void PlayScreen::showCollectWordAnimation(CCNode* sender)
{	
	this->runAction(CCSequence::create(CCCallFuncN::create(this,callfuncN_selector(PlayScreen::addBonusPointsWord)), NULL));
	
	CCSprite *submit = CCSprite::createWithSpriteFrameName(SUBMIT_RIGHT);
	submit->setScale(GlobalClass::getScaleBySprite());
	submit->setPosition( ccp(screenSize.width * 0.935f, screenSize.width * 0.05f));
	this->addChild(submit, 6);

	submit->runAction(CCSequence::create(CCDelayTime::create(3), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroySprites)), NULL));

	if (ci <_colWord->count())
	{
		CCSprite *nod = (CCSprite*)_colWord->objectAtIndex(ci);
	
		//---Add Circle animation with kite
		CCArray* animFrames = CCArray::createWithCapacity(15);
		CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

		CCSprite *spark = CCSprite::createWithSpriteFrameName("effect1.png");
		spark->setPosition( ccp(nod->getContentSize().width/2, nod->getContentSize().height/2));  
		spark->setScale(GlobalClass::getScaleBySprite()*1.7);
		nod->addChild(spark, 1);

		char buff[40];
		for(int i=1;i<=15;i++)
		{
			sprintf(buff,"effect%d.png",i);
			animFrames->addObject(cache->spriteFrameByName(buff));
		}
	
		CCAnimation *sparkAnimation;
		//set animation object here
		if (animFrames->count()!=0) 
		{
			sparkAnimation=CCAnimation::createWithSpriteFrames(animFrames, 0.02f);
		}
		
		spark->runAction(CCSequence::create(CCAnimate::create(sparkAnimation), NULL));
		nod->runAction(CCSequence::create(CCDelayTime::create(0.3f), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::destroySprites)), NULL));
		this->runAction(CCSequence::create(CCDelayTime::create(0.15f), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::showCollectWordAnimation)), NULL));
		CC_SAFE_RELEASE(animFrames);
    
		ci++;
	}
    else 
	{
		ci = 0;
		this->runAction( CCSequence::create(CCCallFuncN::create(this,callfuncN_selector(PlayScreen::showAnimatedScore)),NULL));
	}
}

void PlayScreen::showAnimatedScore(CCNode* sender)
{
	this->runAction(CCSequence::create(CCCallFuncN::create(this,callfuncN_selector(PlayScreen::addBonusPointsWord)), NULL ) );

	char sc[10];
	sprintf(sc, "%d",cScore);
	
	
	float bSizeW = ((CCSprite*)_colWord->lastObject())->boundingBox().size.width * 1.105;
	
    CCLabelBMFont *lbl = CCLabelBMFont::create(sc, GlobalClass::getMessageFont().c_str());
    lbl->setPosition( ccp(screenSize.width * 0.089f + _colWord->count() * bSizeW, screenSize.width * 0.052f));
    lbl->setColor( ccc3(255, 255, 255));
    this->addChild(lbl,7);
    lbl->setScale(0.1);
	
	CCActionInterval*  fAction = CCFadeOut::create(1.0f);
	CCActionInterval*  sAction = CCScaleTo::create(1.0f, 0.7f);
	lbl->runAction( CCSequence::create(sAction, fAction, NULL));

	CCActionInterval*  mAction = CCMoveTo::create(2.0f, CCPoint(screenSize.width * 0.089f + _colWord->count() * bSizeW, screenSize.height/2));
    lbl->runAction( CCSequence::create(mAction, CCCallFuncN::create(this,callfuncN_selector(PlayScreen::destroySprites)), NULL));

	movedBallCounter = 0;
	isFlicking = false;
    _colWord->removeAllObjects();
    wordChar[0]='\0';
}

void PlayScreen::clickOnSubmitBtn(cocos2d::CCObject* sender)
{
    if(strlen(wordChar) == 0 || ci != 0) 
	{
        return;
	}
    else if (strlen(wordChar) == 1) 
	{
		resetKitePosition(this);
		showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_INCOORECT), false);
		
		return;
	}
	else 
	{
		GameSettings *gSettings = GameSettings::sharedSetting();
		int wordCount;
        unsigned int offset; // where it was found (or not (-1))
        
        if (strcmp(bonusPointWord.c_str(), wordChar) == 0)// (offset = bonusPointWord.find(wordChar, 0)) != string::npos)
        {
            wordCount = strlen(wordChar);
        
            //play correct word sound
            if(gSettings->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_SUBMIT_CORRECT);
            
			showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_BONUS_POINT), true);
            wordCount =(wordCount * 2) + 1;
		
			cScore = (wordCount-1) * 10;
			showCollectWordAnimation(this);
		}
        else
        {
            sprintf(wordChar,"%s,",wordChar);
            wordCount = strlen(wordChar);
            
            for (int i = wordCount; i >= 0; i--) {
                wordChar[i+1] = wordChar[i];
            }
            wordChar[0]=',';
            
            if (strlen(wordList.c_str()) > 0) 
            {
                
                if ((offset = wordList.find(wordChar, 0)) != std::string::npos)  // arg 2 (0) specifies the offset from where to begin seach
                {
                    //play correct word sound
                    if(gSettings->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_SUBMIT_CORRECT);
				
					cScore = (wordCount-1) * 10;
					showCollectWordAnimation(this);
                }
                else
                {
                    showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_INCOORECT), false);
                    resetKitePosition(this);
                    return;
                }
            }
            else {
                getAllWordFromDictionary(this);
            }
        }
            
		char scoarr[20];
		gSettings->setScore(gSettings->getScore() + (wordCount-1) * 10);
		sprintf(scoarr,"%d",gSettings->getScore());
		CCLabelBMFont *scoreLabel = (CCLabelBMFont *)this->getChildByTag(kTagSpriteScore);
		scoreLabel->setString(scoarr);    
    }
}

void PlayScreen::getAllAlpha(CCNode* sender)
{
	//load all word for select language fron its dictionary string
	char lanName[30] = {0};
	sprintf(lanName, "%s_Alp.txt", GlobalClass::getCurrentLanguageName().c_str());
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile("", lanName);
		
	CCString *pData = CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(lanName).c_str());
	alphabets = new CCString(*pData);
}

void PlayScreen::getAllWordFromDictionary(CCNode* sender)
{
	//load all word for select language fron its dictionary string
	char lanName[30] = {0};
	sprintf(lanName, "%s.txt",GlobalClass::getCurrentLanguageName().c_str());

	unsigned long size = 0;
    unsigned char* pData = 0;
    pData = CCFileUtils::sharedFileUtils()->getFileData(CCFileUtils::sharedFileUtils()->fullPathForFilename(lanName).c_str(), "rb", &size);
	wordList = (const char*)pData;
    CC_SAFE_DELETE_ARRAY(pData);
}

int PlayScreen::getRandomLettor()
{
	int randNum = (int)(rand() % alphabets->length());
	
    int letterASCII = 65;
    letterASCII = alphabets->getCString()[randNum];
	
	static int bonus = 0;
    bonus++;
	
    if (bonus == rand()%20)
	{
        bonus = 0;
		letterASCII = (int)bonusPointWord.at((int)(rand() % strlen(bonusPointWord.c_str())));
	}
	
	CCLOG("picked letter is %d", letterASCII);
    return letterASCII;     
}

void PlayScreen::addBonusPointsWord(CCNode * sender)
{
    CCLabelTTF *bonusLbl = (CCLabelTTF*)this->getChildByTag(kTagBonusPointWord);
    if (!bonusLbl) 
	{
		int fontSize = 15;
		if (CCDirector::sharedDirector()->getWinSizeInPixels().width > 320)	fontSize = 20;
	
        bonusLbl = CCLabelTTF::create("", FONT_SEGOECBD, fontSize);
        bonusLbl->setPosition( ccp(screenSize.width * 0.50f, screenSize.height * 0.95f));
        bonusLbl->setColor( ccc3(255, 255, 255));
        this->addChild(bonusLbl, 7, kTagBonusPointWord);
        
        CCActionInterval*  action = CCFadeOut::create(2.0f);
        CCFiniteTimeAction*  rep = CCSequence::create(action, action->reverse(), NULL);
        bonusLbl->runAction(CCRepeatForever::create((CCActionInterval*)rep));
    }
    bonusPointWord = "";
	
    int wordLen = strlen(wordList.c_str());
	if (wordLen <= 0) return;
	
    srand((unsigned int)(time(NULL))+wordLen);
    while (true)
	{
        int sPos = rand() % wordLen;
        int s = wordList.find(",", sPos);
        int e = wordList.find(",", s + 1);
        
        if (e - s >= 5 && e - s <= 7) {
            char newWord[8] = {0};
            
            for (int i = s+1, j = 0; i < e; i++, j++) {
                newWord[j] = wordList.at(i);
            }
            
            bonusPointWord = newWord;
            break;
        }                         
        
    }
    bonusLbl->setString(bonusPointWord.c_str());
}

void PlayScreen::moveBubble(CCPoint source, CCPoint destination , CCNode* sender)
{    	
	CCActionInterval*  action;
	float sp = sender->getPositionY();
	
	ccBezierConfig bezier;
	bezier.controlPoint_1 = ccp(source.x/2, source.y);
	bezier.endPosition = ccp(destination.x, destination.y);
	action = CCBezierTo::create(sp/400, bezier); 

	this->reorderChild(sender, 5);
	
	sender->runAction(CCSequence::create( CCScaleTo::create(0.1, GlobalClass::getScaleByScreen()*0.9), CCScaleTo::create(0.1, GlobalClass::getScaleByScreen()*0.82), action, CCCallFuncN::create(this,callfuncN_selector(PlayScreen::moveBubbleCompleted)), NULL));
}

void PlayScreen::moveBubbleCompleted(CCNode* sender)
{
    if (movedBallCounter > 0) {
        movedBallCounter--;
    }
	
    if (movedBallCounter == 0) {
        isFlicking = false;
    }
}

void PlayScreen::pauseGame()
{
    CCMenu* menu = (CCMenu*)this->getChildByTag(kTagMenus);
    menu->setTouchEnabled(false);
    this->pauseSchedulerAndActions();
    
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCArray* ar = this->getChildren();
    for(unsigned int i=0; i < ar->count() ; i++) {
        CCNode* no = (CCNode*)ar->objectAtIndex(i);
        no->pauseSchedulerAndActions();
    }
}

void PlayScreen::resumeGame()
{
    CCMenu* menu = (CCMenu*)this->getChildByTag(kTagMenus);
    menu->setTouchEnabled(true);
    this->resumeSchedulerAndActions();
    
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    
    CCArray* ar = this->getChildren();
    for(unsigned int i=0; i < ar->count() ; i++) {
        CCNode* no = (CCNode*)ar->objectAtIndex(i);
        no->resumeSchedulerAndActions();
    }
}

void PlayScreen::showScoreBoard(cocos2d::CCObject* sender)
{
	backButtonState = kStateQuitMenu;
	pauseGame();
	
	//-------Add Sscorepanel
	CCSprite *scorepanel = CCSprite::create(SCROE_PANEL);
	scorepanel->setScale(GlobalClass::getScaleBySprite());
	scorepanel->setPosition( ccp(screenSize.width*0.5, screenSize.height*0.5f));
	this->addChild(scorepanel, 10, kTagScorePanel);
	
	CCLabelBMFont *msgLbl = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_SAVE_GAME), FONT_TW40);
	msgLbl->setPosition( ccp(scorepanel->getContentSize().width*0.5f, scorepanel->getContentSize().height*0.7f));
	scorepanel->addChild(msgLbl, 2);
	
	//------Add Yes 
	CCMenuItemSprite *yesBtn = CCMenuItemSprite::create(CCSprite::create(BTN_ON_UP), CCSprite::create(BTN_ON_DOWN), this, menu_selector(PlayScreen::clickOnYesButton));
	
	CCLabelTTF * label = CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_YES), FONT_SEGOECBD, GlobalClass::getScaledFont(30));
	label->setPosition( ccp(yesBtn->getContentSize().width * 0.5f, yesBtn->getContentSize().height * 0.5f));
	yesBtn->addChild(label, 0);
	
	//---Add NO
	CCMenuItemSprite *noBtn = CCMenuItemSprite::create(CCSprite::create(BTN_ON_UP), CCSprite::create(BTN_ON_DOWN), this, menu_selector(PlayScreen::clickOnNoButton));
	
	label = CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_NO), FONT_SEGOECBD, GlobalClass::getScaledFont(30));
	label->setPosition( ccp(noBtn->getContentSize().width * 0.5f, noBtn->getContentSize().height * 0.5f));
	noBtn->addChild(label, 0);
		
	CCMenu *mMenus = NULL;
	if (GameSettings::sharedSetting()->getMode() == kTagClassic)
	{
		CCMenuItemSprite *canBtn = CCMenuItemSprite::create(CCSprite::create(BTN_ON_UP), CCSprite::create(BTN_ON_DOWN), this, menu_selector(PlayScreen::clickOnCancelButton));
		label = CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_CANCEL), FONT_SEGOECBD, GlobalClass::getScaledFont(30));
		label->setPosition( ccp(canBtn->getContentSize().width * 0.5f, canBtn->getContentSize().height * 0.5f));
		canBtn->addChild(label, 0);

		mMenus = CCMenu::create(yesBtn, noBtn, canBtn, NULL);
	}
	else {
		msgLbl->setString(TranslateScreen::sharedTranslate()->localeString(TEXT_END_GAME));
		mMenus = CCMenu::create(yesBtn, noBtn, NULL);
	}
	mMenus->setPosition( ccp(scorepanel->getContentSize().width * 0.50f, scorepanel->getContentSize().height * 0.2f));
	mMenus->alignItemsHorizontallyWithPadding(screenSize.width * 0.01f);
	scorepanel->addChild(mMenus, 2);
}

void PlayScreen::clickOnYesButton(cocos2d::CCObject* sender)
{
	GameSettings *gSettings = GameSettings::sharedSetting();
    if(gSettings->getMode() == kTagClassic)
	{
        gSettings->setOldLife(_leftKites->count());
        gSettings->setOldScore(gSettings->getScore());
        CCDirector::sharedDirector()->popScene();
    }
	else if(gSettings->getMode() == kTagEndless|| gSettings->getMode() == kTagTimer)
	{
        if (gSettings->getScore() > 0) CCDirector::sharedDirector()->replaceScene(InputUserName::scene());
		else CCDirector::sharedDirector()->popScene();
    }
}

void PlayScreen::clickOnNoButton(cocos2d::CCObject* sender)
{
	if(GameSettings::sharedSetting()->getMode() == kTagClassic)
	{
		if (GameSettings::sharedSetting()->getScore() > 0) CCDirector::sharedDirector()->replaceScene(InputUserName::scene());
		else CCDirector::sharedDirector()->popScene();
	}
	else
	{
		clickOnCancelButton(this);
	}
}

void PlayScreen::clickOnCancelButton(cocos2d::CCObject* sender)
{
    backButtonState = kStateGamePlay;
	CCNode* nod = (CCNode*)this->getChildByTag(kTagScorePanel);
	this->removeChild(nod,true);
    resumeGame();
}

void PlayScreen::destroyBalls(CCNode* pSender)
{
    if (pSender == optionalBubble)
	{
        optionalBubble = NULL;
        optionalLabel = NULL;
        unschedule(schedule_selector(PlayScreen::changeOptionalBallChar));
	}
    
	_bubbles->removeObject(pSender);
	destroySprites(pSender);
}

void PlayScreen::destroyObstacles(CCNode* pSender)
{
    _obstacles->removeObject(pSender);
	destroySprites(pSender);
}

void PlayScreen::destroySprites(CCNode* pSender)
{
	pSender->removeFromParentAndCleanup(true);
}
