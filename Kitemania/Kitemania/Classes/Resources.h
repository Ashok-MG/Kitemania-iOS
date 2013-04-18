//
//  Resources.h
//  KiteMania
//

#ifndef KiteMania_Resources_h
#define KiteMania_Resources_h

enum
{
	kTagClassic = 10,
	kTagEndless,
	kTagTimer,
};

enum
{
	kTagEnglish = 20,
	kTagSpanish,
	kTagPortuguese,
	kTagFrench,
	kTagGerman,
};

enum
{
	kTagSoundOn = 30,
	kTagSoundOff,
};

//FONTS
#define FONT_ARIAL16 					"arial16.fnt"
#define FONT_ARIAL 						"Arial.ttf"
#define FONT_SEGOECBD 					"SEGOECBD.TTF"
#define FONT_SEGOECRG 					"SEGOECRG.TTF"
#define FONT_TW40						"Tw40.fnt"

//SCREEN ASSETS
#define DEFAULT_IMAGE                   "Default.png"
#define BACKGROUND_IMAGE                "background.png"
#define KITEMANIA_LOGO                  "kitemania.png"
#define BACK_UP							"back_up.png"
#define BACK_DOWN						"back_down.png"
#define SUBMIT_UP						"submit_up.png"
#define SUBMIT_DOWN						"submit_over.png"
#define SUBMIT_WRONG					"submit_red.png"
#define SUBMIT_RIGHT					"submit_green.png"
#define BTN_ON_UP						"button_up.png"
#define BTN_ON_DOWN						"button_down.png"

//MENU ASSETS
#define MENU_PLAY_UP					"play_up.png"
#define MENU_PLAY_DOWN					"play_down.png"
#define MENU_LANGUAGE_UP				"language_up.png"
#define MENU_LANGUAGE_DOWN				"language_down.png"
#define MENU_MODE_UP					"mode_up.png"
#define MENU_MODE_DOWN					"mode_down.png"
#define MENU_SOUND_UP					"sound_up.png"
#define MENU_SOUND_DOWN					"sound_down.png"
#define MENU_HELP_UP					"help_up.png"
#define MENU_HELP_DOWN					"help_down.png"
#define MENU_EXIT_UP					"exit_up.png"
#define MENU_EXIT_DOWN					"exit_down.png"
#define MENU_MENU_UP					"menu_up.png"
#define MENU_MENU_DOWN					"menu_down.png"

//LEADERBOARD ASSETS
#define LEADER_PLAY_UP					"lplay_up.png"
#define LEADER_PLAY_DOWN				"lplay_down.png"
#define LEADER_MENU_UP					"lmenu_up.png"
#define LEADER_MENU_DOWN				"lmenu_down.png"

//GAME ASSETS
#define PLAY_BG_IMAGE                   "play_bg.png"
#define BALL_IMAGE						"ball.png"
#define KITE_WALL						"kite_wall.png"
#define RED_KITE						"kRed.png"
#define GAME_KITE						"kLive.png"
#define HOT_BALLON						"Hot_Air_Balloon.png"
#define SCROE_PANEL						"scorepanel.png"
#define SCORE_HEAD_BAR					"blue_bar.png"
#define SCORE_DATA_BAR					"leaderboard_bar.png"

//SOUNDS
#define SFX_BIRD_HIT					"HIT.m4a"
#define SFX_SUBMIT_CORRECT				"CORRECTWORD.m4a"
#define SFX_WORK_FLICK					"FLICK.m4a"
#define SFX_WORD_PICK					"LETTERPICK.m4a"
#define SFX_AIRHOT_HIT					"Balloon.m4a"
#define SFX_LIGHT_HIT					"zap.m4a"
#define SFX_GAME_BG						"BG.m4a"

//KM RESOURCE PLIST
#define IMAGE_PLIST_MENU                "Menu_Ass.plist"
#define IMAGE_PLIST_GAME                "Game_Ass.plist"
#define IMAGE_PLIST_LIGHT               "lighting.plist"
#define IMAGE_PLIST_OVER				"GameOver_Ass.plist"
#define IMAGE_PLIST_CIRCEL				"circle.plist"
#define IMAGE_PLIST_EFFECT				"effect.plist"

//KM RESOURCE PNG
#define IMAGE_PNG_MENU 					"Menu_Ass.png"
#define IMAGE_PNG_GAME 					"Game_Ass.png"
#define IMAGE_PNG_LIGHT					"lighting.png"
#define IMAGE_PNG_OVER					"GameOver_Ass.png"
#define IMAGE_PNG_CIRCEL				"circle.png"
#define IMAGE_PNG_EFFECT				"effect.png"

#endif
