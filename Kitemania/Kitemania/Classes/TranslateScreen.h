//
//  TranslateScreen.h
//  Kitemania
//
//  Created by Vishal Golia on 26/11/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#ifndef Kitemania_TranslateScreen_h
#define Kitemania_TranslateScreen_h

#include "cocos2d.h"

USING_NS_CC;

typedef enum
{
    TEXT_LOADING = 0,

    //--12
    TEXT_MENU_PLAY,
    TEXT_MENU_LANGUAGE,
    TEXT_MENU_LANG_VAL,
    TEXT_MENU_MODE,
    TEXT_MENU_MODE_CLASSIC,
    TEXT_MENU_MODE_ENDLESS,
    TEXT_MENU_MODE_TIMER,
    TEXT_MENU_SOUND,
    TEXT_MENU_SOUND_ON,
    TEXT_MENU_SOUND_OFF,
    TEXT_MENU_HELP,
    TEXT_MENU_EXIT,
    
    //----10
    TEXT_PLAY_SCORE,
    TEXT_PLAY_TIMER,
    TEXT_PLAY_CORRECT,
    TEXT_PLAY_INCOORECT,
    TEXT_PLAY_BONUS_POINT,
    TEXT_SAVE_GAME,
    TEXT_END_GAME,
    TEXT_YES,
    TEXT_NO,
    TEXT_CANCEL,
    
    //----6
    TEXT_IU_GAME_OVER,
    TEXT_IU_NAME,
    TEXT_IU_ENTER_NAME,
    TEXT_IU_POINTS_SCORED,
    TEXT_IU_OK,
    TEXT_IU_GUEST,
    
    //----4
    TEXT_GO_TOP_TEN_SCORES,
    TEXT_GO_RANK,
    TEXT_GO_PLAYER,
    TEXT_GO_SCORE,
    
    //--4
    TEXT_HELP_TAP_LETTER,
    TEXT_HELP_TAP_SUBMIT,
    TEXT_HELP_BEWARE,
    TEXT_HELP_FLICK,
    
}TextToShow; //----37


class TranslateScreen
{
	public:
		TranslateScreen();
		~TranslateScreen();
	
		/** returns a shared instance of the Translate */
		static TranslateScreen* sharedTranslate(void);

		const char* localeString(TextToShow textToShow);
};

#endif	//	Kitemania_TranslateScreen_h
