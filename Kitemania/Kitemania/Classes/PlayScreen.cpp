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

#define kMaxCloudMoveDuration 36.0f
#define kMinCloudMoveDuration 24.0f
#define kMaxBirdMoveDuration  10
#define kMinBirdMoveDuration  4
#define kBallMinMoveDuration 14
#define kBallMaxMoveDuration 20
#define kMaxAirHotMoveDuration 25
#define kMinAirHotMoveDuration 15

enum {
	kTagScores = 30,
	kTagTimers,
	kTagSpriteBallLbl,
    kTagKite,
	kTagPowerKite,
	kTagBonusWord,
	kTagMenus,
	kTagScorePanel,
};

enum {
    kObstacleAirhot = 40,
    kObstacleBird,
    kObstacleThunder,
};

USING_NS_CC;

void PlayScreen::keyBackClicked()
{
	if (m_GameState == kStateGamePlay) 
	{
		showScoreBoard(this);
		return;
	}
	else if (m_GameState == kStateQuitMenu){
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
	screenSize = CCDirector::sharedDirector()->getWinSize();

	m_GameLevel = kGameLevelBasic;
	m_GameState = kStateGamePlay;

	m_time = 90;
	m_score = 0;

	_leftKites = new CCArray();
	_colWord = new CCArray();
	_obstacles = new CCArray();
	_bubbles = new CCArray();
	_pBubbles = new CCArray();
	
	mSbmtAnimatWat = dictWordSeq = 0;
	dictWord = bonusWord = dictWordList = NULL;
	
	isEnableFlicker = true;
	charCompMoved = 0;
	
	kite = NULL;
	optionalBubble = NULL;
	wordChar[0] = '\0';

	//-------Add Background Image
	CCSprite *bgImage = CCSprite::create(PLAY_BG_IMAGE);
    bgImage->setScale(GlobalClass::getScaleByScreen());
	bgImage->setPosition(GlobalClass::getCenterPointOfSize(screenSize));
	this->addChild(bgImage);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(PLAY_BG_IMAGE);
	
	//-------Add Kite wall
	CCSprite *kiteWall = CCSprite::create(KITE_WALL);
	kiteWall->setScale(GlobalClass::getScaleByScreen());
	kiteWall->setAnchorPoint(ccp(0.5, 0));
	kiteWall->setPosition( ccp(screenSize.width*0.5, 0));
	this->addChild(kiteWall, 5);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(KITE_WALL);

	//-------Add Top bar
	CCSprite *topBar = CCSprite::create(TOP_BAR);
	topBar->setScale(GlobalClass::getScaleByScreen());
	topBar->setAnchorPoint(ccp(0.5, 1));
	topBar->setPosition( ccp(screenSize.width*0.5, screenSize.height));
	this->addChild(topBar, 6);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TOP_BAR);

	//------Add submit
	CCMenuItemSprite *mItmSubmit = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(SUBMIT_UP), CCSprite::createWithSpriteFrameName(SUBMIT_DOWN), this, menu_selector(PlayScreen::clickOnSubmitBtn));
	mItmSubmit->setPosition( ccp(screenSize.width*0.5f, screenSize.width*0.05f));
	mItmSubmit->setScale(GlobalClass::getScaleBySprite());
	
	//-------Back to Back menu
	CCMenuItemSprite *mItmBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(PAUSE_UP), CCSprite::createWithSpriteFrameName(PAUSE_UP), this, menu_selector(PlayScreen::showScoreBoard));
	mItmBack->setPosition( ccp(screenSize.width*0.945f, screenSize.height - screenSize.width * 0.14f));
	mItmBack->setScale(GlobalClass::getScaleBySprite());
	
    CCMenu *mMenus = CCMenu::create(mItmSubmit, mItmBack, NULL);
    mMenus->setPosition(CCPointZero);
    this->addChild(mMenus, 6, kTagMenus);

	//------Add flaying kite kRed.png 
	kite = CCSprite::createWithSpriteFrameName(GREEN_KITE);
	kite->setScale(GlobalClass::getScaleByScreen());
	kite->setPosition( ccp(screenSize.width*0.5f, screenSize.width*0.38f));
	this->addChild(kite, 4, kTagKite);
	
	//Add score label method
	addGameScores();
	
	// Load alphabet and dictionary file and then show suggested bonus word 
	loadAlphabetDictionary(this);

	//Add bonus word
	addDictWord(3, 4);

	//-----call add cloud method
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*8 + 4), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveClouds)), NULL));

	//------call add balls
	if (m_GameLevel == kGameLevelBasic)
		this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*4 + 2), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveBalls)), NULL));
	else
		this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*5 + 0.3f), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveBalls)), NULL));
	
	if (GameSettings::sharedSetting()->getMode() == kTagClassic)
	{
		//----call add life method
		addGameLife();
		
		if (m_GameLevel == kGameLevelBonus) addObstacles(this);
	}
	else if (GameSettings::sharedSetting()->getMode() == kTagTimer)
	{
		//----call add timer method
		addGameTimer();
	}

	// it enable touch on screen and start a schedular for collision detection
	enableKiteFlying(this);
	
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
}

void PlayScreen::addObstacles(CCObject* pSender)
{
	//-----call add flaying birds
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*8 + 10), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveBirds)), NULL));
	
	//-----call add lights
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*10 + 30), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addThunderbolt)), NULL));
	
	//-----call add airhot
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*8 + 19), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveAirHot)), NULL));
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
	float offScreenXPosition = -cSize.width/2;
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
	this->addChild(airHot, CCRANDOM_0_1() * 2 + 1, kObstacleAirhot);

	CCSize aSize = airHot->boundingBox().size;
	
	//Get random position of X - airhot moving from left to right or right to left
	float offScreenXPosition = -aSize.width/2;
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
    this->addChild(bird, CCRANDOM_0_1() * 2 + 1, kObstacleBird);

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
	this->addChild(light, CCRANDOM_0_1() * 2 + 1, kObstacleThunder);

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
		CCSprite *life = CCSprite::createWithSpriteFrameName(GREEN_LIVE);
		life->setScale(GlobalClass::getScaleBySprite());
		life->setAnchorPoint(ccp(1, 0.5));
		life->setPosition( ccp(screenSize.width*0.965f - (life->boundingBox().size.width*1.1f*i), screenSize.height - screenSize.width*0.042f));
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
	scoreTitle->setPosition(ccp(screenSize.width*0.03f, screenSize.height - screenSize.width*0.042f));
	this->addChild(scoreTitle, 6);
    
	GameSettings *gSettings = GameSettings::sharedSetting();
	if (gSettings->getMode() == kTagClassic)
	{
		m_score = gSettings->getOldScore();
		gSettings->setOldScore(0);
	}
	else gSettings->setScore(0);
	
	char score[10] = {0};
	sprintf(score, "%d", m_score);
	
    CCLabelBMFont *scLabel = CCLabelBMFont::create(score, GlobalClass::getHelpMessageFont().c_str());
	scLabel->setPosition( ccp(scoreTitle->boundingBox().size.width + scoreTitle->boundingBox().origin.x, scoreTitle->getPositionY()));
	scLabel->setColor( ccc3(255, 255, 0));
	scLabel->setAnchorPoint( ccp(0, 0.5));
	this->addChild(scLabel, 6, kTagScores);
}

void PlayScreen::addGameTimer()
{
	char timer[3] = {0};
	sprintf(timer, "%.0f", m_time);

	CCLabelBMFont *timeLabel = CCLabelBMFont::create(timer, GlobalClass::getHelpMessageFont().c_str());
	timeLabel->setPosition(ccp(screenSize.width*0.97f, screenSize.height - screenSize.width*0.042f));
	timeLabel->setAnchorPoint(ccp(1, 0.5));
	this->addChild(timeLabel, 6, kTagTimers);
	
	//-------Add timer label
	CCLabelBMFont *timerTitle = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_TIMER), GlobalClass::getHelpMessageFont().c_str());
	timerTitle->setAnchorPoint(ccp(1, 0.5));
	timerTitle->setPosition(ccp(screenSize.width*0.97f - timeLabel->boundingBox().size.width, screenSize.height - screenSize.width*0.042f));
	this->addChild(timerTitle, 6);
}

void PlayScreen::addMoveBalls(float t)
{
	//-----Get image of ball.png that have in GameScreen_Ass.plist file
	CCSprite* ball = CCSprite::createWithSpriteFrameName(BALL_IMAGE);
	ball->setScale(GlobalClass::getScaleByScreen());
	
	//get random ASCII char value
	int n = getRandomLetter(m_GameLevel == kGameLevelBasic);
	ball->setTag(n);
	
    char alphabet[1] = {0};
    sprintf(alphabet,"%c", n);
    
	CCLabelTTF *alphabetLabel = CCLabelTTF::create(CCApplication::sharedApplication()->getUTF8String(alphabet), FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setColor( ccc3(0, 0, 0));
    alphabetLabel->setPosition(GlobalClass::getCenterPointOfSize(ball->getContentSize()));
	ball->addChild(alphabetLabel, 1, kTagSpriteBallLbl);

	CCSize bSize = ball->boundingBox().size;

	//Get random x position
    float xPosition = screenSize.width - bSize.width/2;
	xPosition = CCRANDOM_0_1()*(xPosition - bSize.width/2) + bSize.width/2;
	ball->setPosition( ccp(xPosition, screenSize.height + bSize.height*0.5f));
	this->addChild(ball, 3);
    
	//Get random speed
    float moveDuration = CCRANDOM_0_1() * (kBallMaxMoveDuration - kBallMinMoveDuration) + kBallMinMoveDuration;
    
    ball->runAction(CCSequence::create(CCMoveTo::create(moveDuration, ccp(xPosition, -bSize.height*0.5f)), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroyBalls)), NULL));
    
	if (m_GameLevel == kGameLevelBonus)
	{
		// Set some ball with random optional charactor - character will changed every 2 sec.
		if (CCRANDOM_0_1()*4 < 2)
		{
			if (optionalBubble == NULL)
			{
				optionalBubble = ball;
				schedule(schedule_selector(PlayScreen::changeOptionalBallChar), 2.0f);
			}
		}
	}
	
    _bubbles->addObject(ball);
	if (m_GameLevel == kGameLevelBasic)
		this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*4 + 2), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveBalls)), NULL));
	else
		this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*5 + 0.3f), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addMoveBalls)), NULL));
}

void PlayScreen::addPowerBalls(float t)
{
	//-----Get image of ball.png that have in GameScreen_Ass.plist file
	CCSprite* ball = CCSprite::createWithSpriteFrameName(POWER_BALL);
	ball->setScale(GlobalClass::getScaleByScreen());
	
	CCLabelTTF *alphabetLabel = CCLabelTTF::create("2X", FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setColor( ccc3(0, 0, 0));
    alphabetLabel->setPosition(GlobalClass::getCenterPointOfSize(ball->getContentSize()));
	ball->addChild(alphabetLabel, 1, kTagSpriteBallLbl);
	
	CCSize bSize = ball->boundingBox().size;
	
	//Get random x position
    float xPosition = screenSize.width - bSize.width/2;
	xPosition = CCRANDOM_0_1()*(xPosition - bSize.width/2) + bSize.width/2;
	ball->setPosition( ccp(xPosition, screenSize.height + bSize.height*0.5f));
	this->addChild(ball, 3, 1000);
    
	//Get random speed
	float moveDuration = CCRANDOM_0_1() * (kBallMaxMoveDuration - kBallMinMoveDuration) + kBallMinMoveDuration;
    
	_pBubbles->addObject(ball);
    ball->runAction(CCSequence::create(CCMoveTo::create(moveDuration, ccp(xPosition, -bSize.height*0.5f)), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroyBalls)), NULL));
	
	this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*30 + 100.0f), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addPowerBalls)), NULL));
}

void PlayScreen::changeOptionalBallChar(float t)
{
	if (optionalBubble)
	{
		int n = getRandomLetter(false);
		optionalBubble->setTag(n);
		
		char alphabet[1] = {0};
		sprintf(alphabet,"%c", n);

		CCLabelTTF *alphabetLabel = (CCLabelTTF*)optionalBubble->getChildByTag(kTagSpriteBallLbl);
		alphabetLabel->setString(CCApplication::sharedApplication()->getUTF8String(alphabet));
	}
}

void PlayScreen::tick(float dt)
{
	if (GameSettings::sharedSetting()->getMode() == kTagTimer)
	{
		m_time -= dt;
		if (m_time <= 0)
		{
			clickOnYesButton(this);
			return;
		}
		
		char timer[3] = {0};
		sprintf(timer, "%.0f", m_time);

		CCLabelBMFont *tLbl = (CCLabelBMFont*)this->getChildByTag(kTagTimers);
		tLbl->setString(timer);
	}
		
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
					if (strlen(wordChar) < 8)
					{
						//play letter sound
						if(GameSettings::sharedSetting()->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_WORD_PICK);
						
						_colWord->addObject(bubble);
						if (wordChar[0] == '\0') sprintf(wordChar, "%c", bubble->getTag());
						else sprintf(wordChar , "%s%c", wordChar, bubble->getTag());

						bubble->stopAllActions();
						_bubbles->removeObject(bubble);
						
						if (bubble==optionalBubble)
						{
							optionalBubble = NULL;
							unschedule(schedule_selector(PlayScreen::changeOptionalBallChar));
						}
						
						float bSizeW = bubble->boundingBox().size.width * 1.035f;
						this->moveBubble(bubble->boundingBox().origin, ccp(bSizeW*0.51f + (_colWord->count()-1) * bSizeW, screenSize.width * 0.174f), bubble);
					}
					break;
				}
			}
		
			//----2x ball collision
			for (int i=0; i<_pBubbles->count(); i++)
			{
				CCSprite *bubble = (CCSprite*)_pBubbles->objectAtIndex(i);
			
				decWidth = bubble->boundingBox().size.width * 0.1f;
				CCRect bubbleRect = CCRect(bubble->boundingBox().origin.x + decWidth, bubble->boundingBox().origin.y + decWidth, bubble->boundingBox().size.width - decWidth * 2, bubble->boundingBox().size.height - decWidth * 2);
			
				if (bubbleRect.intersectsRect(kiteRect))
				{
					//play bird-hit sound
					if(GameSettings::sharedSetting()->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_WORD_PICK);
					kite->setTag(kTagPowerKite);
				
					bubble->stopAllActions();
					_pBubbles->removeObject(bubble);

					reorderChild(bubble, 5);
					bubble->runAction( CCSequence::create(CCScaleTo::create(1.0f, GlobalClass::getScaleByScreen()*1.1f), CCScaleTo::create(1.0f, GlobalClass::getScaleByScreen()), NULL));
					bubble->runAction( CCSequence::create(CCMoveTo::create(bubble->getPositionY()/400, ccp(screenSize.width*0.97f, screenSize.width*0.05f)), NULL));
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

					showKiteCollisionResetAnimation(this);
					lostGameLife(this);
                    break;
				}
			}
		}
	}
}

void PlayScreen::loadAlphabetDictionary(CCNode* sender)
{
	char lanName[30] = {0};
	
	//load all word for select language fron its dictionary string
	sprintf(lanName, "%s.txt", GlobalClass::getCurrentLanguageName().c_str());
	
	CCString* rData = CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(lanName).c_str());
	dictWordList = new CCString(*rData);
}

void PlayScreen::addBonusPointsWord(CCObject *pSender)
{	
	if (dictWordList->length() < 1)
	{
		loadAlphabetDictionary(NULL);
		return;
	}
	
    while (true)
	{
        int sPos = CCRANDOM_0_1() * dictWordList->length();
        int s = dictWordList->m_sString.find(",", sPos);
        int e = dictWordList->m_sString.find(",", s + 1);
        
        if (e-s >= 5 && e-s <= 7)
		{
			if (bonusWord) CC_SAFE_RELEASE_NULL(bonusWord);
			bonusWord = new CCString(dictWordList->m_sString.substr(s+1, e-s-1));
            break;
		}
	}
	
	CCLabelTTF *bonusLbl = (CCLabelTTF*)this->getChildByTag(kTagBonusWord);
	if (!bonusLbl)
	{
		int fontSize = 15;
		if (CCDirector::sharedDirector()->getWinSizeInPixels().width > 320)	fontSize = 20;
		
		bonusLbl = CCLabelTTF::create(bonusWord->getCString(), FONT_SEGOECBD, fontSize);
		bonusLbl->setPosition( ccp(screenSize.width*0.5f, screenSize.height - screenSize.width*0.042f));
		bonusLbl->setColor( ccc3(255, 255, 255));
		this->addChild(bonusLbl, 7, kTagBonusWord);
	
		CCActionInterval* action = CCFadeOut::create(2.0f);
		bonusLbl->runAction(CCRepeatForever::create(CCSequence::create(action, action->reverse(), NULL)));
	}
	else
	{
		bonusLbl->setString(bonusWord->getCString());
	}
}

void PlayScreen::addDictWord(int minCharLen, int maxCharLen)
{
	if (dictWordList->length() < 1)
	{
		loadAlphabetDictionary(NULL);
		return;
	}
	
    while (true)
	{
		int sPos = CCRANDOM_0_1() * dictWordList->length();
		int s = dictWordList->m_sString.find(",", sPos);
        int e = dictWordList->m_sString.find(",", s + 1);
    
		if (minCharLen > 0 && maxCharLen > 0)
		{
			if (e-s-1 >= minCharLen && e-s-1 <= maxCharLen)
			{
				if (dictWord) CC_SAFE_RELEASE_NULL(dictWord);
				dictWord = new CCString(dictWordList->m_sString.substr(s+1, e-s-1));
				break;
			}
		}
		else if (e-s > 0)
		{
			if (dictWord) CC_SAFE_RELEASE_NULL(dictWord);
			dictWord = new CCString(dictWordList->m_sString.substr(s+1, e-s-1));
			break;
		}
	}
	
	CCLOG("dict word - %s", dictWord->getCString());
}

int PlayScreen::getRandomLetter(bool isSequential)
{
	int asciiChar = 65;
	
	if (isSequential)	//	true means showing sequential form suggested word
	{
		asciiChar = dictWord->getCString()[dictWordSeq];
		dictWordSeq++;
	
		if (dictWordSeq >= dictWord->length()) dictWordSeq = 0;
	}
	else				//	false means showing random only form suggested word
	{
		if (dictWord->length() == 0) addDictWord(0, 0);

		int asciiIndex = CCRANDOM_0_1() * dictWord->length();
		asciiChar = dictWord->getCString()[asciiIndex];
		dictWord->m_sString.erase(asciiIndex, 1);
	}
	return asciiChar;
}

bool PlayScreen::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void PlayScreen::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if (isEnableFlicker && _colWord->count() != charCompMoved) return;
	
    CCPoint location = touch->getLocationInView();
    location =CCDirector::sharedDirector()->convertToGL(location);
    CCRect pointRect = CCRectMake(location.x, location.y, 1, 1);
	
    for (int i=0; i < _colWord->count(); i++)
	{
		CCSprite *temSprite = (CCSprite*)_colWord->objectAtIndex(i);
		
        if (pointRect.intersectsRect(temSprite->boundingBox()))
		{
			isEnableFlicker = false;
			
			//flick sound
			if(GameSettings::sharedSetting()->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_WORK_FLICK);
			
			_colWord->removeObject(temSprite);
			temSprite->runAction( CCSequence::create( CCMoveTo::create(0.1 , CCPointMake(temSprite->getPosition().x, temSprite->getPosition().y + 20)), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::destroySprites)), NULL));
			
			int count = _colWord->count();
			float bSizeW = temSprite->boundingBox().size.width * 1.035f;

            for (int j=i; j<count; j++)
			{
                wordChar[j] = wordChar[j+1];
				temSprite = (CCSprite*)_colWord->objectAtIndex(j);;
				temSprite->runAction( CCSequence::create( CCMoveTo::create(0.1 , CCPointMake(bSizeW*0.51f + (_colWord->count()-1) * bSizeW, temSprite->getPositionY())), NULL));
			}

			wordChar[count] = '\0';
			charCompMoved = count;
			isEnableFlicker = true;
            break;
		}
	}
}

void PlayScreen::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint location = pTouch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	
	if (location.y < screenSize.width*0.239f || location.y > screenSize.height - screenSize.width*0.089f) return;
	
	kite->stopAllActions();
    
	if (location.x < 0) location.x = 0;
    
    float o		= location.x - kite->getPosition().x;
	float a		= location.y - kite->getPosition().y;
	float at	= (float) CC_RADIANS_TO_DEGREES(atanf( o/a));
    
	if(a < 0)
	{
		if(o < 0)	at = 180 + fabs(at);
		else		at = 180 - fabs(at);
	}
    
    o = abs((int)o);
    a = abs((int)a);
	
	double area = sqrt(o*o+a*a);
	if (area < 0 && area > 1000 ) area = 0.1;
	
	if (o == 0 && a == 0)
		kite->runAction( CCSequence::create(CCMoveTo::create(location.y*0.02f, ccp(location.x, 0)), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::showKiteCollisionResetAnimation)), NULL));
	else
		kite->runAction( CCSequence::create( CCRotateTo::create(0.2, at), CCMoveTo::create(area/400, location), CCMoveTo::create(location.y*0.02f, ccp(location.x, 0)), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::showKiteCollisionResetAnimation)), NULL));
}

void PlayScreen::showKiteCollisionResetAnimation(CCNode *sender)
{
	unschedule(schedule_selector(PlayScreen::tick));

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	kite->stopAllActions();
    kite->runAction(CCSequence::create(CCBlink::create(0.5f, 3), CCMoveTo::create(0, ccp(screenSize.width*0.5f, screenSize.width*0.38f)), CCRotateTo::create(0, 0), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::enableKiteFlying)), NULL));
}

void PlayScreen::enableKiteFlying(CCNode* sender)
{
	schedule( schedule_selector(PlayScreen::tick));
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void PlayScreen::lostGameLife(CCNode* sender)
{
	if (_leftKites)
	{
		if (_leftKites->count( )> 1)
		{
			this->removeChild((CCSprite*)_leftKites->lastObject(), true);
			_leftKites->removeLastObject();
		}
		else
		{
			this->removeAllChildrenWithCleanup(true);
                
			if (GameSettings::sharedSetting()->getScore() > 0)
				CCDirector::sharedDirector()->replaceScene(InputUserName::scene());
			else
				CCDirector::sharedDirector()->popScene();
		}
	}
}

void PlayScreen::clickOnSubmitBtn(cocos2d::CCObject* sender)
{
//	if (_colWord->count() != charCompMoved) return;
	
    if(strlen(wordChar) == 0 || mSbmtAnimatWat != 0) {}
    else if (strlen(wordChar) == 1) 
	{
		showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_INCOORECT), false);
		showKiteCollisionResetAnimation(this);
		lostGameLife(this);
	}
	else 
	{
		GameSettings *gSettings = GameSettings::sharedSetting();
		int	wordCount = strlen(wordChar);
        
        if (m_GameLevel == kGameLevelBonus && strcmp(bonusWord->getCString(), wordChar) == 0)
        {
			m_score = wordCount * 20;
			updateScores();

            //play correct word sound
            if(gSettings->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_SUBMIT_CORRECT);
        
			addBonusPointsWord(this);
			showCollectWordAnimation(this);
			showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_BONUS_POINT), true);
			return;
		}
    
		sprintf(wordChar, "%s,", wordChar);
		wordChar[wordCount] = ',';
		wordChar[wordCount+1] = '\0';
	
		if (dictWordList->length() > 0)
		{
			if (dictWordList->m_sString.find(wordChar, 0) != std::string::npos)
			{
				m_score = wordCount * 10;
				updateScores();

				if (m_GameLevel == kGameLevelBasic)
				{
					if ( gSettings->getScore() >= 200)
					{
						addObstacles(this);
						addBonusPointsWord(this);
						m_GameLevel = kGameLevelBonus;
						addDictWord(0, 0);
					
						this->runAction(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*30 + 100.0f), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::addPowerBalls)), NULL));
					}
					else {
						addDictWord(3, 4);
					}
				}
				else {
					addDictWord(0, 0);
				}
			
				//Update bonus word
				dictWordSeq = 0;

				//play correct word sound
				if(gSettings->getMusic() == kTagSoundOn) Music::sharedMusic()->playEffectSound(SFX_SUBMIT_CORRECT);
			
				showCollectWordAnimation(this);
			}
			else
			{
				showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_PLAY_INCOORECT), false);
				showKiteCollisionResetAnimation(this);
				lostGameLife(this);
			}
		}
	}
}

void PlayScreen::canAbleToSubmit(CCObject *pSender)
{
	int	wordCount = strlen(wordChar);
	char tempWordChar[10] = {0};
	
	sprintf(tempWordChar, "%s,", wordChar);
	tempWordChar[wordCount] = ',';
	tempWordChar[wordCount+1] = '\0';
	
	if (dictWordList->length() > 0)
	{
		if (dictWordList->m_sString.find(wordChar, 0) != std::string::npos)
		{
			CCLOG("able to submition");
		}
	}
}
		
void PlayScreen::updateScores()
{
	if (kite->getTag() == kTagPowerKite)
	{
		kite->setTag(kTagKite);
		m_score*=2;
	}
	GameSettings *gSettings = GameSettings::sharedSetting();
	gSettings->setScore( gSettings->getScore() + m_score);

	char score[10] = {0};
	sprintf(score, "%d", gSettings->getScore());
	
	CCLabelBMFont *scoreLabel = (CCLabelBMFont *)this->getChildByTag(kTagScores);
	scoreLabel->setString(score);
}

void PlayScreen::showCollectWordAnimation(CCNode* sender)
{
	CCSprite *submit = CCSprite::createWithSpriteFrameName(SUBMIT_RIGHT);
	submit->setScale(GlobalClass::getScaleBySprite());
	submit->setPosition( ccp(screenSize.width*0.5f, screenSize.width*0.05f));
	this->addChild(submit, 6);
	
	submit->runAction(CCSequence::create(CCDelayTime::create(3), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroySprites)), NULL));
	
	if (mSbmtAnimatWat <_colWord->count())
	{
		CCSprite *nod = (CCSprite*)_colWord->objectAtIndex(mSbmtAnimatWat);
		
		//---Add Circle animation with kite
		CCArray* animFrames = CCArray::createWithCapacity(15);
		CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		
		CCSprite *spark = CCSprite::createWithSpriteFrameName("effect1.png");
		spark->setPosition( ccp(nod->getContentSize().width/2, nod->getContentSize().height/2));
		spark->setScale(GlobalClass::getScaleBySprite()*1.7);
		nod->addChild(spark, 1);
		
		char buff[40] = {0};
		for(int i=1; i<=15; i++)
		{
			sprintf(buff, "effect%d.png", i);
			animFrames->addObject(cache->spriteFrameByName(buff));
		}
		
		CCAnimation *sparkAnimation;
		//set animation object here
		if (animFrames->count()!=0)
		{
			sparkAnimation = CCAnimation::createWithSpriteFrames(animFrames, 0.02f);
		}
		
		spark->runAction( CCSequence::create( CCAnimate::create(sparkAnimation), NULL));
		nod->runAction( CCSequence::create( CCDelayTime::create(0.3f), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroySprites)), NULL));
		this->runAction( CCSequence::create( CCDelayTime::create(0.15f), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::showCollectWordAnimation)), NULL));
		CC_SAFE_RELEASE(animFrames);
		
		mSbmtAnimatWat++;
	}
    else
	{
		mSbmtAnimatWat = 0;
		this->runAction( CCSequence::create( CCCallFuncN::create(this, callfuncN_selector(PlayScreen::showAnimatedScore)),NULL));
	}
}

void PlayScreen::showAnimatedScore(CCNode* sender)
{
	float bSizeW = ((CCSprite*)_colWord->lastObject())->boundingBox().size.width * 1.105;
	
	char score[10] = {0};
	sprintf(score, "%d", m_score);
	
    CCLabelBMFont *lbl = CCLabelBMFont::create(score, GlobalClass::getMessageFont().c_str());
    lbl->setPosition( ccp(bSizeW*0.51f + (charCompMoved-1) * bSizeW, screenSize.width*0.174f));
    lbl->setColor( ccc3(255, 255, 255));
    this->addChild(lbl, 7);
    lbl->setScale(0.1);
	
	lbl->runAction( CCSequence::create(CCScaleTo::create(1.0f, 0.7f), CCFadeOut::create(1.0f), NULL));
    lbl->runAction( CCSequence::create(CCMoveTo::create(2.0f, CCPoint(lbl->getPositionX(), screenSize.height/2)), CCCallFuncN::create(this,callfuncN_selector(PlayScreen::destroySprites)), NULL));
	
    _colWord->removeAllObjects();
    wordChar[0] = '\0';
	charCompMoved = 0;
}

void PlayScreen::moveBubble(CCPoint source, CCPoint destination , CCNode* sender)
{    	
	ccBezierConfig bezier;
	bezier.controlPoint_1 = ccp(source.x/2, source.y);
	bezier.endPosition = ccp(destination.x, destination.y);

	this->reorderChild(sender, 5);
	
	sender->runAction(CCSequence::create( CCScaleTo::create(0.1, GlobalClass::getScaleByScreen()*1.1f), CCScaleTo::create(0.1, GlobalClass::getScaleByScreen()), CCBezierTo::create(sender->getPositionY()/400, bezier), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::ballCompletelyMoved)), NULL));
}

void PlayScreen::ballCompletelyMoved(CCNode* sender)
{
	charCompMoved++;
	canAbleToSubmit(this);
//	isEnableFlicker = true;
}

void PlayScreen::showMessage(std::string msg, bool status)
{
    CCLabelBMFont *lbl = CCLabelBMFont::create(msg.c_str(), GlobalClass::getMessageFont().c_str());
    lbl->setPosition( ccp(screenSize.width/2,screenSize.height/2));
    lbl->setColor( ccc3(255, 255, 255));
    this->addChild(lbl,7);
    
    CCActionInterval*  action = CCFadeOut::create(2.0f);
    lbl->runAction( CCSequence::create(action, CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroySprites)),NULL));
    
	if (status == false)
	{
		CCSprite *submit = CCSprite::createWithSpriteFrameName(SUBMIT_WRONG);
		submit->setScale(GlobalClass::getScaleBySprite());
		submit->setPosition( ccp(screenSize.width*0.5f, screenSize.width*0.05f));
		this->addChild(submit, 6);
		submit->runAction(CCSequence::create(CCDelayTime::create(3), CCCallFuncN::create(this, callfuncN_selector(PlayScreen::destroySprites)), NULL));
		
		for (unsigned int i=0 ; i<_colWord->count(); i++)
		{
			this->removeChild((CCSprite*)_colWord->objectAtIndex(i), true);
		}
		
		_colWord->removeAllObjects();
		wordChar[0] = '\0';
		charCompMoved = 0;
	}
}

void PlayScreen::showScoreBoard(cocos2d::CCObject* sender)
{
	m_GameState = kStateQuitMenu;
	pauseGame();
	
	//-------Add Sscorepanel
	CCSprite *scorepanel = CCSprite::createWithSpriteFrameName(SCROE_PANEL);
	scorepanel->setScale(GlobalClass::getScaleBySprite());
	scorepanel->setPosition( ccp(screenSize.width*0.5, screenSize.height*0.5f));
	this->addChild(scorepanel, 10, kTagScorePanel);
	
	CCLabelBMFont *msgLbl = CCLabelBMFont::create(TranslateScreen::sharedTranslate()->localeString(TEXT_SAVE_GAME), FONT_TW40);
	msgLbl->setPosition( ccp(scorepanel->getContentSize().width*0.5f, scorepanel->getContentSize().height*0.7f));
	scorepanel->addChild(msgLbl, 2);
	
	//------Add Yes 
	CCMenuItemSprite *yesBtn = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(BTN_ON_UP), CCSprite::createWithSpriteFrameName(BTN_ON_DOWN), this, menu_selector(PlayScreen::clickOnYesButton));
	
	CCLabelTTF * label = CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_YES), FONT_SEGOECBD, GlobalClass::getScaledFont(30));
	label->setPosition( ccp(yesBtn->getContentSize().width * 0.5f, yesBtn->getContentSize().height * 0.5f));
	yesBtn->addChild(label, 0);
	
	//---Add NO
	CCMenuItemSprite *noBtn = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(BTN_ON_UP), CCSprite::createWithSpriteFrameName(BTN_ON_DOWN), this, menu_selector(PlayScreen::clickOnNoButton));
	
	label = CCLabelTTF::create(TranslateScreen::sharedTranslate()->localeString(TEXT_NO), FONT_SEGOECBD, GlobalClass::getScaledFont(30));
	label->setPosition( ccp(noBtn->getContentSize().width * 0.5f, noBtn->getContentSize().height * 0.5f));
	noBtn->addChild(label, 0);
		
	CCMenu *mMenus = NULL;
	if (GameSettings::sharedSetting()->getMode() == kTagClassic)
	{
		CCMenuItemSprite *canBtn = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(BTN_ON_UP), CCSprite::createWithSpriteFrameName(BTN_ON_DOWN), this, menu_selector(PlayScreen::clickOnCancelButton));
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
    m_GameState = kStateGamePlay;
	CCNode* nod = (CCNode*)this->getChildByTag(kTagScorePanel);
	this->removeChild(nod,true);
    resumeGame();
}

void PlayScreen::destroyBalls(CCNode* pSender)
{
    if (pSender == optionalBubble)
	{
        optionalBubble = NULL;
        unschedule(schedule_selector(PlayScreen::changeOptionalBallChar));
	}
    
	if (pSender->getTag() == 1000) _pBubbles->removeObject(pSender);
	else _bubbles->removeObject(pSender);
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
