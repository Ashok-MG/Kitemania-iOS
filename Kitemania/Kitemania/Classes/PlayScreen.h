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

typedef enum {
    kGameLevelBasic = 50,
    kGameLevelBonus,
    kGameLevelAdvance,
} kTagGameLevel;

typedef enum {
    kStateGamePlay = 60,
    kStateQuitMenu,
    kStateShowScoreBoard,
} kTagGameState;


class  PlayScreen : public CCLayerColor
{
    private:
        CCRenderTexture* renderTexture;
    
		CCSize screenSize;
		kTagGameLevel m_GameLevel;
		kTagGameState m_GameState;
	
        CCArray *_leftKites;
		CCArray *_colWord;
		CCArray *_bubbles;
		CCArray *_pBubbles;
		CCArray *_obstacles;
		
		CCSprite *kite;
		CCSprite *optionalBubble;
	
		CCString *dictWordList;
		CCString *dictWord;
		CCString *bonusWord;
	
		float m_time;
		int m_score;
		int mSbmtAnimatWat;
		int dictWordSeq;
		char wordChar[10];
	
		int  charCompMoved;
		bool isEnableFlicker;
	
		void loadAlphabetDictionary(CCNode* sender);
	
		void addObstacles(CCObject* pSender);
		void addGameScores();
		void addGameLife();
		void addGameTimer();
		void changeOptionalBallChar(float t);

		void addMoveBalls(float t);
		void addPowerBalls(float t);
		void addMoveClouds(float t);
		void addMoveBirds(float t);
		void addThunderbolt(float t);
		void addMoveAirHot(float t);

		void destroyBalls(CCNode* pSender);
		void destroySprites(CCNode* pSender);
		void destroyObstacles(CCNode* pSender);
		
		void tick(float dt);
		void showKiteCollisionResetAnimation(CCNode *sender);
		void enableKiteFlying(CCNode* sender);
		void ballCompletelyMoved(CCNode* sender);
		void moveBubble(CCPoint source, CCPoint destination, CCNode* sender);

		void addBonusPointsWord(CCObject *pSender);
		void addDictWord(int minCharLen, int maxCharLen);
		int getRandomLetter(bool isSequential);

		void updateScores();
		void lostGameLife(CCNode* sender);

		void clickOnSubmitBtn(CCObject* sender);
		void canAbleToSubmit(CCObject *pSender);

		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    
        bool checkPixelCollision(CCSprite* spr1, CCSprite* spr2, bool pixelPerfect);
        CCRect getIntersectionRect(CCRect rect1, CCRect rect2);

        void resetFlicker(CCObject* pSender);

		void pauseGame();
		void resumeGame();

        void showMessage(std::string msg, bool status);
		void showCollectWordAnimation(CCNode* sender);
		void showAnimatedScore(CCNode* sender);
		void showScoreBoard(CCObject* sender);
		
        void clickOnYesButton(CCObject* sender);
        void clickOnNoButton(CCObject* sender);
        void clickOnCancelButton(CCObject* sender);
        
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
