//
//  KitemaniaAppDelegate.cpp
//  Kitemania
//
//  Created by Vinod's Mac on 11/04/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "LoadingScreen.h"
#include "GameSettings.h"
#include "Resources.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(false);

	// Set 2D projection
	pDirector->setProjection(kCCDirectorProjection2D);
	
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = LoadingScreen::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
	
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
	
	if(GameSettings::sharedSetting()->getMusic() == kTagSoundOn || GameSettings::sharedSetting()->getMusic() == 0)
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	}
}
