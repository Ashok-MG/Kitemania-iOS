//
//  PlayScreen.h
//  KiteMania
//
//  Created by Vishal Golia on 27/08/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef KiteMania_PlayScreen_h
#define KiteMania_PlayScreen_h

#include "cocos2d.h"

USING_NS_CC;
    
class  PlayScreen : public CCLayerColor
{
    private:
		
		
        CCArray *_leftKites;
		CCArray *_colWord;
		CCArray *_bubbles;
		CCArray *_obstacles;

		CCSprite *optionalBubble;
        CCLabelTTF *optionalLabel;
        
        bool isRunningTick;
		bool isFlicking;
		int movedBallCounter;
		float m_time;
		
		CCSize screenSize;
        int score;
        CCPoint touchLocation;
        CCSprite *kite;
        std::string bonusPointWord;
		int cScore;
		unsigned int ci;

		std::string wordList;
		char wordChar[20];
//		int alphaCount, alphaListArray[500];
		CCString *alphabets;
	
		void getAllAlpha(CCNode* sender);
		void getAllWordFromDictionary(CCNode* sender);

		void addGameScores();
		void addGameLife();
		void addGameTimer();
		void changeOptionalBallChar(float t);

		void addMoveBalls(float t);
		void addMoveClouds(float t);
		void addMoveBirds(float t);
		void addThunderbolt(float t);
		void addMoveAirHot(float t);

		void destroyBalls(CCNode* pSender);
		void destroySprites(CCNode* pSender);
		void destroyObstacles(CCNode* pSender);
		
		void tick(float dt);
		void kiteFall(CCNode* sender);
		void resetKitePosition(CCNode *sender);
		void setIsKiteBlinking(CCNode* sender);
        void setIsKiteBlinkingEnd(CCNode* sender);
        void showMessage(std::string msg, bool status);
		void showCollectWordAnimation(CCNode* sender);
		void showAnimatedScore(CCNode* sender);
		
		void showScoreBoard(CCObject* sender);
		
        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
		
		void moveBubble(CCPoint source, CCPoint destination, CCNode* sender);
		void moveBubbleCompleted(CCNode* sender);
		
		virtual	void clickOnSubmitBtn(CCObject* sender);
        
        int getRandomLettor();
        void pauseGame();
        void resumeGame();
        
        void clickOnYesButton(CCObject* sender);
        void clickOnNoButton(CCObject* sender);
        void clickOnCancelButton(CCObject* sender);
        
        void addBonusPointsWord(CCNode * sender);
		
        int backButtonState;
        const char *os;
		
	public:
		PlayScreen();
		~PlayScreen();

	    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
		virtual bool init();
		
		// there's no 'id' in cpp, so we recommand to return the exactly class pointer
		static CCScene* scene();
		
		// implement the "static node()" method manually
		CREATE_FUNC(PlayScreen);
		
		virtual void onEnter();
		virtual void onExit();
		
		void initScene(CCNode* sender);
		virtual void keyBackClicked();
};
    
#endif	//	__KiteMania_PlayScreen_h__
