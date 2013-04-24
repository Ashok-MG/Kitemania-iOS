//
//  HelpScreen.cpp
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "HelpScreen.h"
#include "Resources.h"
#include "GlobalClass.h"
#include "Resources.h"
#include "TranslateScreen.h"

#define kMaxCloudMoveDuration 28
#define kMinCloudMoveDuration 24
#define kMaxBirdMoveDuration  10
#define kMinBirdMoveDuration  3
#define kBallMoveDuration 8

enum {
	kTagBall1,
	kTagBall2,
	kTagBall3,
};

USING_NS_CC;

void HelpScreen::keyBackClicked()
{
	CCLOG("click on back button");
    CCDirector::sharedDirector()->popScene();
}

CCScene* HelpScreen::scene()
{
	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
    HelpScreen *layer = HelpScreen::create();
	
	// add layer as a child to scene
    scene->addChild(layer);
	
	// return the scene
    return scene;
}

HelpScreen::HelpScreen() {}

HelpScreen::~HelpScreen()
{
	unscheduleAllSelectors();
	removeAllChildrenWithCleanup(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(IMAGE_PLIST_GAME);
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(IMAGE_PLIST_CIRCEL);
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(IMAGE_PLIST_LIGHT);
}

// on "init" you need to initialize your instance
bool HelpScreen::init()
{
	//////////////////////////////
	// 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(37, 188, 241, 255)))
	{
        return false;
	}
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_GAME, IMAGE_PNG_GAME);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(IMAGE_PNG_GAME);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_CIRCEL, IMAGE_PNG_CIRCEL);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(IMAGE_PNG_CIRCEL);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(IMAGE_PLIST_LIGHT, IMAGE_PNG_LIGHT);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(IMAGE_PNG_LIGHT);
	
	this->setKeypadEnabled(true);
	
	screenSize = CCDirector::sharedDirector()->getWinSize();
	initScene(NULL);

	return true;
}

void HelpScreen::onEnter()
{
    CCLayer::onEnter();
}

void HelpScreen::onExit()
{		
    CCLayer::onExit();
}

void HelpScreen::initScene(CCNode* sender)
{
	//-------Add Background Image
	CCSprite *bgImage = CCSprite::create(BACKGROUND_IMAGE);
    bgImage->setScale(GlobalClass::getScaleBySprite());
	bgImage->setPosition(GlobalClass::getCenterPointOfSize(screenSize));
	this->addChild(bgImage, 0);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(BACKGROUND_IMAGE);

    //-------Add Kite wall
	CCSprite *kiteWall = CCSprite::create(KITE_WALL);
	kiteWall->setScale(GlobalClass::getScaleByScreen());
	kiteWall->setAnchorPoint(ccp(0.5, 0));
	kiteWall->setPosition( ccp(screenSize.width*0.5, 0));
	this->addChild(kiteWall, 5);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(KITE_WALL);
	
	//-------Add TRANSPARENT_IMAGE Image
	CCLayerColor* TransBg = CCLayerColor::create( ccc4(0, 0, 0, 180), screenSize.width, screenSize.height);
	TransBg->setPosition(CCPointZero);
	this->addChild(TransBg, 7);

	//----------Back to Main menu
	CCMenuItemSprite *mItmBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(BACK_UP), CCSprite::createWithSpriteFrameName(BACK_DOWN), this, menu_selector(HelpScreen::keyBackClicked));
	mItmBack->setScale(GlobalClass::getScaleBySprite());
	mItmBack->setPosition( ccp(screenSize.width * 0.065f, screenSize.width * 0.05f));
	
    CCMenu *mMenus = CCMenu::create(mItmBack, NULL);
    mMenus->setPosition(CCPointZero);
    this->addChild(mMenus, 6);
     
	//------Add submit
	mItmSubmit = CCSprite::createWithSpriteFrameName(SUBMIT_UP); //submit_up.png
	mItmSubmit->setScale(GlobalClass::getScaleBySprite());
	mItmSubmit->setPosition( ccp(screenSize.width * 0.935f, screenSize.width * 0.05f));
	this->addChild(mItmSubmit, 6);
    
    //-------- add balls with labels K , i  ,t
	//KKKKKK 
    CCSprite *ball = CCSprite::createWithSpriteFrameName(BALL_IMAGE); //ball.png
	ball->setScale(GlobalClass::getScaleByScreen()*0.82);
	ball->setPosition( ccp((screenSize.width * 0.089f+1 * ball->boundingBox().size.width * 1.105) , screenSize.width * 0.052f));
    this->addChild(ball, 5);

    CCLabelTTF *alphabetLabel = CCLabelTTF::create("K", FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setColor(ccc3(0,0,0));
    alphabetLabel->setPosition(ccp(ball->getContentSize().width/2, ball->getContentSize().height/2)); 
    ball->addChild(alphabetLabel);

    //IIIIIIIIII
	ball = CCSprite::createWithSpriteFrameName(BALL_IMAGE); //ball.png
	ball->setScale(GlobalClass::getScaleByScreen()*0.82);
	ball->setPosition( ccp((screenSize.width * 0.089f+2 * ball->boundingBox().size.width * 1.105) , screenSize.width * 0.052f));
    this->addChild(ball, 5);
	
    alphabetLabel = CCLabelTTF::create("I", FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setColor(ccc3(0,0,0));
    alphabetLabel->setPosition(ccp(ball->getContentSize().width/2, ball->getContentSize().height/2)); 
    ball->addChild(alphabetLabel);
   
    ///TTTTTTTT
	ball = CCSprite::createWithSpriteFrameName(BALL_IMAGE); //ball.png
	ball->setScale(GlobalClass::getScaleByScreen()*0.82);
	ball->setPosition( ccp((screenSize.width * 0.089f+3 * ball->boundingBox().size.width * 1.105) , screenSize.width * 0.052f));
    this->addChild(ball, 5);
	
    alphabetLabel = CCLabelTTF::create("T", FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setColor(ccc3(0,0,0));
    alphabetLabel->setPosition(ccp(ball->getContentSize().width/2, ball->getContentSize().height/2)); 
    ball->addChild(alphabetLabel);
    
	//------Add flaying kite kRed.png 
	kite = CCSprite::createWithSpriteFrameName(RED_KITE);
	kite->setScale(GlobalClass::getScaleByScreen());
	kite->setPosition( ccp(screenSize.width * 0.5f, screenSize.width * 0.5f));
	this->addChild(kite, 8);

	//---Add Circle animation with kite
	CCArray* smallFrames = CCArray::createWithCapacity(38);
	CCSprite *circle = CCSprite::createWithSpriteFrameName("circle1.png");
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	char buff[15] = {0};
	for(int i=1;i<=38;i++)
	{
        sprintf(buff,"circle%d.png",i);
        smallFrames->addObject( cache->spriteFrameByName(buff));
	}
	
    CCAnimation *circleAnimation;
	//set animation object here
    if (smallFrames->count()!=0) 
	{
        circleAnimation = CCAnimation::createWithSpriteFrames(smallFrames, 0.2);
	}
	
    kite->addChild(circle, 1);
    circle->setPosition( ccp(kite->getContentSize().width/2, kite->getContentSize().height/2));  
	CCAnimate *circleAnimate = CCAnimate::create(circleAnimation);
    circle->runAction(CCSequence::create(circleAnimate, CCCallFuncN::create(this, callfuncN_selector(HelpScreen::destroyKiteAnimation)), NULL));
    CC_SAFE_RELEASE_NULL(smallFrames);
	
	//Start the Help animation with repeate forever
    this->runAction(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(HelpScreen::kiteAnimation)), NULL));    
}

void HelpScreen::kiteAnimation(CCNode* sender)
{
    CCLOG(">>>>>>>>>KITE-ANIMATIONS  <<<<<<<<<<");
    //------call add balls
    schedule(schedule_selector(HelpScreen::addMoveBall), 1.0f); 
    schedule(schedule_selector(HelpScreen::flyKite), 1.0f);
    schedule(schedule_selector(HelpScreen::addMoveClouds), 5.0f);
}

void HelpScreen::addMoveBall(float t)
{
    unschedule(schedule_selector(HelpScreen::addMoveBall));
           
	//ball 1 
	ball = CCSprite::createWithSpriteFrameName(BALL_IMAGE);
	ball->setScale( GlobalClass::getScaleByScreen()*0.82f);
	CCSize bSize = ball->boundingBox().size;
	ball->setPosition(ccp(screenSize.width/2,screenSize.height + (bSize.height/2)));
	this->addChild(ball, 9, kTagBall1);

	CCLabelTTF *alphabetLabel = CCLabelTTF::create("E", FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setTag(101);
    alphabetLabel->setColor(ccc3(0,0,0));
	alphabetLabel->setPosition(ccp(ball->getContentSize().width/2, ball->getContentSize().height/2)); 
    ball->addChild(alphabetLabel);

	CCFiniteTimeAction* actionMove = CCMoveTo::create((float)kBallMoveDuration-4,ccp(screenSize.width/2, screenSize.height-screenSize.height/4) );
    ball->runAction(CCSequence::create(actionMove, CCCallFuncN::create(this,callfuncN_selector(HelpScreen::moveBallAnimation)),NULL));

	//ball 2
	CCSprite* ball2 = CCSprite::createWithSpriteFrameName(BALL_IMAGE);
	ball2->setScale( GlobalClass::getScaleByScreen()*0.82f);
    ball2->setPosition(ccp(screenSize.width/3,screenSize.height + (bSize.height/2)));
	this->addChild(ball2, 3, kTagBall2);

	alphabetLabel = CCLabelTTF::create("H", FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setColor(ccc3(0,0,0));
	alphabetLabel->setPosition(ccp(ball2->getContentSize().width/2, ball2->getContentSize().height/2)); 
    ball2->addChild(alphabetLabel);
    
	actionMove = CCMoveTo::create((float)kBallMoveDuration+5,ccp(screenSize.width/3, 0 - bSize.height/2) );
    ball2->runAction(CCSequence::create(actionMove, CCCallFuncN::create(this,callfuncN_selector(HelpScreen::destroyKiteAnimation)), NULL));

	//ball 3
	CCSprite* ball3 = CCSprite::createWithSpriteFrameName(BALL_IMAGE);
	ball3->setScale( GlobalClass::getScaleByScreen()*0.82f);
	ball3->setPosition(ccp(screenSize.width/2+screenSize.width/3,screenSize.height + (bSize.height/2)));
	this->addChild(ball3, 3, kTagBall3);

	alphabetLabel = CCLabelTTF::create("R", FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setColor(ccc3(0,0,0));
	alphabetLabel->setPosition(ccp(ball3->getContentSize().width/2, ball3->getContentSize().height/2)); 
    ball3->addChild(alphabetLabel);

	actionMove = CCMoveTo::create((float)kBallMoveDuration+8,ccp(screenSize.width/2+screenSize.width/3, 0 - bSize.height/2) );
    ball3->runAction(CCSequence::create(actionMove, CCCallFuncN::create(this,callfuncN_selector(HelpScreen::destroyKiteAnimation)), NULL));
}  	

void HelpScreen::flyKite(float t)
{
	unschedule(schedule_selector(HelpScreen::flyKite));
            
    //SHOW MESSAGE HERE
    showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_HELP_TAP_LETTER));
          
  	ccBezierConfig bezier;
	bezier.controlPoint_1  = ccp(screenSize.width/6, screenSize.height/2);
    bezier.controlPoint_2  = ccp(screenSize.width- screenSize.width/6, screenSize.height-screenSize.height/2);
	bezier.endPosition = ccp(screenSize.width/2, screenSize.height-screenSize.height/4);
	CCActionInterval *flyKite = CCBezierTo::create(4.0, bezier); 
    
	float sp = kite->getPositionY()/40;
	CCActionInterval *move = CCMoveTo::create(sp, ccp(kite->getPosition().x, screenSize.height/2));
    CCActionInterval *move2= CCMoveTo::create(3.0, ccp(screenSize.width/4, screenSize.height/1.5));
    CCActionInterval *move3= CCMoveTo::create(7.8, ccp(screenSize.width/4, screenSize.height/3));        
       
    kite->runAction(CCSequence::create(flyKite, move, move2, move3, NULL));
}

void HelpScreen::addMoveClouds(float t)
{
    srand(time(NULL));
    int cloudToDraw=(rand()%3) +1;
	
	char cloudName[20] = {0};
	sprintf(cloudName, "cloud%d.png", cloudToDraw);
	CCSprite *cloud = CCSprite::createWithSpriteFrameName(cloudName);
	cloud->setScale(GlobalClass::getScaleBySprite());
    this->addChild(cloud, rand()%2+1);
    
    float xOffSet=cloud->boundingBox().size.width/2;
    int xPosition = screenSize.width + 1 + xOffSet; 
	
	CCSize cSize = cloud->boundingBox().size;
    srand(time(NULL));
    int yPosition=rand()%((int)screenSize.height) + (screenSize.height/5) - (cSize.height); 
    if(yPosition>(screenSize.height-20)){
		srand(time(NULL));
        yPosition=screenSize.height- (rand()/100)-(cSize.height);
    }
    
    if(yPosition<(screenSize.height/3)){
        yPosition+=yPosition;
    }
    
    int moveDuration=rand()%kMaxCloudMoveDuration+1;
    if (moveDuration < kMinCloudMoveDuration) {
        moveDuration = kMinCloudMoveDuration; 
    }    
    
    float offScreenXPosition = (xOffSet * -1) - 1;
    if(rand()%2==0) {
        float tem=offScreenXPosition;
        offScreenXPosition=xPosition;
        xPosition=tem;
    }
    
    cloud->setPosition(ccp(xPosition, yPosition));
    cloud->runAction(CCSequence::create(CCMoveTo::create(moveDuration,ccp(offScreenXPosition,cloud->getPosition().y)), CCCallFuncN::create(this, callfuncN_selector(HelpScreen::destroyKiteAnimation)), NULL));
}

void HelpScreen::showMessage(std::string msg)
{
    srand(time(NULL));
    float randHeight=(rand()% (int)(screenSize.height-screenSize.height/2.0))+ screenSize.height/2.5;
    
	CCLabelBMFont *lbl = CCLabelBMFont::create(msg.c_str(), GlobalClass::getHelpMessageFont().c_str());
    lbl->setPosition( ccp(screenSize.width/2, randHeight) );
    this->addChild(lbl,7);
    
	lbl->runAction(CCSequence::create(CCFadeOut::create(5.5f), CCCallFuncN::create(this, callfuncN_selector(HelpScreen::destroyKiteAnimation)), NULL));
}

void HelpScreen::moveBallAnimation(CCNode* sender)
{
    float bSizeW = ball->boundingBox().size.width * 1.105;
    this->moveBubble(ball->boundingBox().origin, ccp(screenSize.width * 0.089f + 4 * bSizeW, screenSize.width * 0.052f), ball);
}

void HelpScreen::moveBubble(CCPoint source, CCPoint destination , CCNode* sender)
{
	CCPoint convertedLocation = destination;
 	if (convertedLocation.x < 0) convertedLocation.x = 0;
    
    float o = convertedLocation.x - source.x;
	float a = convertedLocation.y - source.y;
	float at = (float) CC_RADIANS_TO_DEGREES( atanf( o/a) );
    
	if( a < 0 )
	{
		if(o < 0)	at = 180 + fabs(at);
		else		at = 180 - fabs(at);
	}
    
    o = abs((int)o);
    a = abs((int)a);
    
    double area=sqrt(o*o+a*a);
	if (area < 0 && area > 1000 ) area = 0.1;
   	
    ccBezierConfig bezier;
	bezier.controlPoint_1 = ccp(source.x/2, source.y);
	bezier.endPosition = ccp(convertedLocation.x,convertedLocation.y);    
	
	sender->runAction(CCSequence::create( CCScaleTo::create(0.1, GlobalClass::getScaleByScreen()*0.9), 
                                          CCScaleTo::create(0.1, GlobalClass::getScaleByScreen()*0.82),
                                          CCBezierTo::create(area/500,bezier),
                                          CCCallFuncN::create(this,callfuncN_selector(HelpScreen::showSubmitInfo)),
                                          NULL));
	this->reorderChild(sender, 8);    
}

void HelpScreen::showSubmitInfo(CCNode* sender)
{
	showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_HELP_TAP_SUBMIT));
	this->reorderChild(ball, 5);   

	//---Add Circle animation with kite
	CCArray* smallFrames = CCArray::createWithCapacity(38);
	CCSprite *circle = CCSprite::createWithSpriteFrameName("circle1.png");
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	circle->setPosition( ccp(mItmSubmit->getContentSize().width / 2, mItmSubmit->getContentSize().height / 2));  
	this->reorderChild(mItmSubmit, 8);
    mItmSubmit->addChild(circle, 1);

	char buff[40] = {0};
	for(int i=1;i<=38;i++)
	{
        sprintf(buff,"circle%d.png",i);
        smallFrames->addObject(cache->spriteFrameByName(buff));
	}
	
    CCAnimation *circleAnimation;
	//set animation object here
    if (smallFrames->count()!=0) 
	{
        circleAnimation=CCAnimation::createWithSpriteFrames(smallFrames, 0.05);
	}
	
    circle->runAction(CCSequence::create(CCAnimate::create(circleAnimation), CCCallFuncN::create(this,callfuncN_selector(HelpScreen::destroyButtonAnimation)), NULL));
    CC_SAFE_RELEASE_NULL(smallFrames);
	
    mItmSubmit->runAction(CCSequence::create(CCDelayTime::create(4), CCCallFuncN::create(this,callfuncN_selector(HelpScreen::showBirdHitInfo)), NULL));   	
}

void HelpScreen::showBirdHitInfo(CCNode* sender)
{
    showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_HELP_BEWARE));
    addMoveBirds();
	addlighting();
	addAirHot();
}

void HelpScreen::addMoveBirds()
{
	CCArray* flyFrames = CCArray::createWithCapacity(4);
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    CCSprite *bird = CCSprite::createWithSpriteFrameName("bird1.png");
    bird->setScale(GlobalClass::getScaleBySprite());
	this->addChild(bird, 9);

    char buff[25] = {0};
	for(int i=1; i<=4; i++)
	{
        sprintf(buff,"bird%d.png",i);
        flyFrames->addObject(cache->spriteFrameByName(buff));
	}
    
    CCAnimation *birdAnimation;
    //set animation object here
    if (flyFrames->count()!=0) 
	{
        birdAnimation = CCAnimation::createWithSpriteFrames(flyFrames,0.2);
	}
    
	CCSize bSize = bird->boundingBox().size;
    float xOffSet = bSize.width/2;
    int xPosition = screenSize.width + 1 + xOffSet; 
    int yPosition=(screenSize.height) - (screenSize.height/4)-bSize.height;
     
    int moveDuration=rand()%kMaxBirdMoveDuration+1;
    if (moveDuration < kMinBirdMoveDuration) {
        moveDuration = kMinBirdMoveDuration; 
    }    
    
    float offScreenXPosition = (xOffSet * -1) - 1;
    
    if (rand()%2==0) {
        float tem=offScreenXPosition;
        offScreenXPosition=xPosition;
        xPosition=tem;
        bird->setFlipX(true);
    }
    
    bird->setPosition(ccp(xPosition,yPosition));
	
    CCRepeatForever *birdAction = CCRepeatForever::create(CCAnimate::create(birdAnimation));
    bird->runAction(birdAction);
	
    bird->runAction(CCSequence::create( CCMoveTo::create(moveDuration,ccp(offScreenXPosition, bird->getPosition().y)),
                                        CCCallFuncN::create(this,callfuncN_selector(HelpScreen::destroyKiteAnimation)),
                                        CCCallFuncN::create(this,callfuncN_selector(HelpScreen::flickWord)),
                                        NULL));
    
	CC_SAFE_RELEASE_NULL(flyFrames);
	
	//---Add Circle animation with birds
	CCArray* smallFrames = CCArray::createWithCapacity(25);

	CCSprite *circle = CCSprite::createWithSpriteFrameName("circle1.png");
	bird->addChild(circle,1);
    circle->setPosition( ccp(bird->getContentSize().width/2,bird->getContentSize().height/2));  

	for(int i=1;i<=25;i++)
	{
        sprintf(buff,"circle%d.png",i);
        smallFrames->addObject(cache->spriteFrameByName(buff));
	}
    CCAnimation *circleAnimation;
	//set animation object here
    if (smallFrames->count()!=0) 
	{
        circleAnimation=CCAnimation::createWithSpriteFrames(smallFrames, 0.05);
	}
	
    circle->runAction(CCRepeatForever::create(CCAnimate::create(circleAnimation)));
	CC_SAFE_RELEASE_NULL(smallFrames);
}

void HelpScreen::addAirHot()
{
    CCSprite *airHot = CCSprite::createWithSpriteFrameName(HOT_BALLON);
    airHot->setScale(GlobalClass::getScaleBySprite());
    this->addChild(airHot, 8);
    
	//---------
	CCSize bSize = airHot->boundingBox().size;
    float xOffSet = bSize.width/2;
    int xPosition = screenSize.width + 1 + xOffSet; 
    int yPosition=(screenSize.height) - (screenSize.height/4)-bSize.height;
	
    int moveDuration=rand()%kMaxBirdMoveDuration+1;
    if (moveDuration < kMinBirdMoveDuration) {
        moveDuration = kMinBirdMoveDuration; 
    }    
    
    float offScreenXPosition = (xOffSet * -1) - 1;
    
    if (rand()%2==0) {
        float tem=offScreenXPosition;
        offScreenXPosition=xPosition;
        xPosition=tem;
    }
    
    airHot->setPosition(ccp(xPosition,yPosition));
		
    airHot->runAction(CCSequence::create( CCMoveTo::create(moveDuration,ccp(offScreenXPosition,airHot->getPosition().y + airHot->boundingBox().size.height)), CCCallFuncN::create(this,callfuncN_selector(HelpScreen::destroyKiteAnimation)), NULL));
    	
	
	//---Add Circle animation with birds
	CCArray* smallFrames = CCArray::createWithCapacity(25);
	CCSprite *circle = CCSprite::createWithSpriteFrameName("circle1.png");
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	circle->setPosition( ccp(airHot->getContentSize().width/2,airHot->getContentSize().height/2));  
	circle->setScale(2.0);
	airHot->addChild(circle,1);

	char buff[25] = {0};
	for(int i=1;i<=25;i++)
	{
        sprintf(buff,"circle%d.png",i);
        smallFrames->addObject(cache->spriteFrameByName(buff));
	}
	
    CCAnimation *circleAnimation;
	//set animation object here
    if (smallFrames->count()!=0) 
	{
        circleAnimation = CCAnimation::createWithSpriteFrames(smallFrames, 0.05);
	}
	
    circle->runAction(CCRepeatForever::create(CCAnimate::create(circleAnimation)));
    CC_SAFE_RELEASE_NULL(smallFrames);
}

void HelpScreen::addlighting()
{
	CCArray* lightFrames = CCArray::createWithCapacity(17);
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	
	CCSprite *light = CCSprite::createWithSpriteFrameName("lighting1.png");
	light->setScale(GlobalClass::getScaleBySprite());
	this->addChild(light, 8);

	int xPosition = screenSize.width - light->boundingBox().size.width * 2.0; 
	int yPosition = screenSize.height * 0.5f;
	xPosition = (rand() % xPosition) + light->boundingBox().size.width;
	yPosition = (rand() % yPosition) + screenSize.height * 0.5f - light->boundingBox().size.height;
    light->setPosition(ccp(xPosition,yPosition));

    char buff[30] = {0};
	for(int i=1;i<=17;i++)
	{
        sprintf(buff,"lighting%d.png",i);
        lightFrames->addObject(cache->spriteFrameByName(buff));
	}
    
    CCAnimation *lightAnimation;
	//set animation object here
    if (lightFrames->count()!=0) 
	{
        lightAnimation=CCAnimation::createWithSpriteFrames(lightFrames, 0.2);
	}
	
    light->runAction(CCSequence::create(CCAnimate::create(lightAnimation), CCCallFuncN::create(this,callfuncN_selector(HelpScreen::destroyKiteAnimation)), NULL));
	CC_SAFE_RELEASE_NULL(lightFrames);

	//---Add Circle animation with birds
	CCArray* smallFrames = CCArray::createWithCapacity(25);

	CCSprite *circle = CCSprite::createWithSpriteFrameName("circle1.png");
    circle->setPosition( ccp(light->getContentSize().width/2,light->getContentSize().height/2));  
	light->addChild(circle,1);

	for(int i=1;i<=25;i++)
	{
        sprintf(buff,"circle%d.png",i);
        smallFrames->addObject(cache->spriteFrameByName(buff));
	}
	
    CCAnimation *circleAnimation;
	//set animation object here
    if (smallFrames->count()!=0) 
	{
        circleAnimation=CCAnimation::createWithSpriteFrames(smallFrames, 0.05);
	}
	
    circle->runAction(CCRepeatForever::create(CCAnimate::create(circleAnimation)));
	CC_SAFE_RELEASE_NULL(smallFrames);
}

void HelpScreen::destroyButtonAnimation(CCNode* sender)
{
	this->reorderChild(mItmSubmit, 6);
	sender->removeFromParentAndCleanup(true);
}

void HelpScreen::destroyKiteAnimation(CCNode* sender)
{
	sender->removeFromParentAndCleanup(true);
}

void HelpScreen::flickWord(CCNode* sender){
   
    showMessage(TranslateScreen::sharedTranslate()->localeString(TEXT_HELP_FLICK));
    ball->removeChildByTag(101,false);
	
    CCLabelTTF *alphabetLabel = CCLabelTTF::create("M", FONT_SEGOECBD, GlobalClass::getScaledFont(44));
    alphabetLabel->setColor(ccc3(0,0,0));
    alphabetLabel->setPosition(ccp(ball->getContentSize().width/2, ball->getContentSize().height/2)); 
	ball->addChild(alphabetLabel);

    this->reorderChild(ball, 8);
    ball->runAction(CCSequence::create(CCMoveTo::create(1, ccp((ball->getPositionX() + ball->boundingBox().size.width), ball->getPositionY() + ball->boundingBox().size.height)), CCDelayTime::create(1), CCCallFuncN::create(this,callfuncN_selector(HelpScreen::kiteAnimation)), CCCallFuncN::create(this,callfuncN_selector(HelpScreen::destroyKiteAnimation)), NULL));
    
	//---Add Circle animation with kite
	CCArray* smallFrames = CCArray::createWithCapacity(17);
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	
	CCSprite *circle = CCSprite::createWithSpriteFrameName("circle1.png");
	circle->setScale(GlobalClass::getScaleBySprite()*1.6);
	circle->setPosition( ccp(ball->getContentSize().width/2, ball->getContentSize().height/2));  
	ball->addChild(circle,1);

	char buff[40] = {0};
	for(int i=1;i<=38;i++)
	{
        sprintf(buff,"circle%d.png",i);
        smallFrames->addObject(cache->spriteFrameByName(buff));
	}
	
    CCAnimation *circleAnimation;
	//set animation object here
    if (smallFrames->count()!=0) 
	{
        circleAnimation = CCAnimation::createWithSpriteFrames(smallFrames, 0.05);
	}
	
    circle->runAction(CCAnimate::create(circleAnimation));
    CC_SAFE_RELEASE_NULL(smallFrames);
}
