//
//  TranslateScreen.cpp
//  Kitemania
//
//  Created by Vishal Golia on 26/11/11.
//  Copyright 2011 developer@maharajagames.com. All rights reserved.
//

#include "TranslateScreen.h"
#include "GameSettings.h"

//IMPORTANT KEEP THE SIZE OF ALL ARRAYS SAME

const char* LANG_ENGLISH_ARRAY[]=   {   "Loading...", "Play", "Language", "English",
                                        "Mode", "Classic", "Endless", "Timer",
                                        "Sound", "On", "Off", "Help", "Exit",                                      
                                        "Score: ", "Timer: ", "Correct", "Incorrect", "Bonus Point",
                                        "Do you want to save the game?", "Do you want to end the game?",
                                        "Yes", "No", "Cancel",
                                        "Game Over", "Name:", "Enter Name",
                                        "Points Scored:", "Ok", "Guest",
                                        "Top Ten Scores", "Rank", "Player", "Score",
                                        "Tap a letter to make kite to collect it.",
                                        "Tap the submit button to validate \n                your word.",
                                        "Beware of obstacles.",
                                        "Flick the letter if it is unwanted."
};

const char* LANG_SPANISH_ARRAY[]=   {   "Cargando ...", "Jugar", "Lengua", "Español",
                                        "Modo", "Clásico", "Sin Fin", "Avisador",
                                        "Sonido", "En", "De", "Ayuda", "Salida",
                                        "Punto: ", "Avisador: ", "Correcto", "Incorrecto", "Punto bonus",
                                        "Quieres salvar el juego?", "Quieres terminar el juego?",
                                        "sí", "No", "Cancelar",
                                        "Juego encima", "Nombre:", "Entrar nombre",
                                        "Puntos obtenidos:", "Ok", "Invitado",
                                        "Diez puntos", "Rango", "Jugador", "Punto",
                                        "Pulse una letra para hacer kite para \n                 recogerlo.",
                                        "Pulse el botón de enviar para validar \n               de su palabra.",
                                        "Tenga cuidado con los obstáculos.",
                                        "Película de la letra si no es deseado."
                                    };

const char* LANG_PORTE_ARRAY[]=     {   "Carregamento...", "Jogar", "Linguagem", "Português",
                                        "Modo", "Clássico", "Interminável", "Cronômetro",
                                        "Som", "Em", "Fora", "Ajuda","Sair",     
                                        "Contagem: " ,"Cronômetro: " ,"Correto" ,"Incorreto","Ponto de bÛnus",      
                                        "Você quer salvar o jogo?", "Você quer terminar o jogo?",
                                        "Sim", "Não", "Cancelar", 
                                        "jogo sobre", "Nome:", "Digite o Nome",
                                        "Pontos Marcados:", "Ok", "Convidado",
                                        "Topo Dez Ponto", "Fila", "Jogador", "Ponto",
                                        "Toque em uma carta para fazer \n          kite para recolhê-lo.",
                                        "Toque no botão de envio para \n      validar a sua palavra.",
                                        "Ter cuidado com os obstáculos.",
                                        "Flick da letra, se é indesejada."
                                    };

const char* LANG_FRENCH_ARRAY[]=    {   "Chargement...", "Jouez", "Langue", "Français",
                                        "Mode", "Classique", "Sans fin", "Minuterie",
                                        "Sonore", "Sur", "Off", "Aide", "Sortir",
                                        "Score: ", "Minuterie: ", "Correcte", "Incorrecte", "Point de Bonus",
                                        "Voulezvous sauver le jeu?", "Voulezvous la fin du jeu?",
                                        "Oui", "Aucun", "Annuler",
                                        "jeu plus", "Nom:", "Entrez le nom",
                                        "Points marqués:", "Ok", "InvitÈs",
                                        "Haut Dix Scores", "Rang", "Joueur", "Score",
                                        "Touchez une lettre pour faire des \n  cerfsvolants pour les recueillir.",
                                        "Appuyez sur le bouton Soumettre \n        pour valider votre mot.",
                                        "Méfiez-vous des obstacles.",
                                        "Flick la lettre si n'est pas désirée"
};

const char* LANG_GERMAN_ARRAY[]=    {   "Laden...", "Spielen", "Sprache", "Deutsch",
                                        "Mode", "Klassiker", "Endlos", "Timer",
                                        "Klang", "Auf", "Ab", "Hilfe", "Ausfahrt",
                                        "Partitur: ", "Timer: ", "Richtig", "Falsch", "Bonus Punkt",
                                        "Möchten Sie das Spiel speichern?", "Möchten Sie das Spiel?",
                                        "Ja", "Nicht", "Kündigen",
                                        "Spiel ¸ber", "Name:", "Eingeben Name",
                                        "TippPunkte:", "Ok", "Gast",
                                        "Top Zehn Scores", "Rang", "Partitur", "Spieler",
                                        "Tippen Sie auf einen Buchstaben, \n     um Drachen zu sammeln.",
                                        "Tippen Sie auf die Schaltfläche Senden, \n        um Ihr Wort zu bestätigen.",
                                        "Hüten Sie sich vor Hindernissen.",
                                        "Flick den Brief, wenn er unerwünscht ist."                   
                                    };


TranslateScreen::TranslateScreen() {}
TranslateScreen::~TranslateScreen() {}

// singleton stuff
static TranslateScreen *s_SharedTranslate = NULL;

TranslateScreen* TranslateScreen::sharedTranslate(void)
{
    if (!s_SharedTranslate)
	{
        s_SharedTranslate = new TranslateScreen();
	}
	
    return s_SharedTranslate;
}

const char* TranslateScreen::localeString(TextToShow textToShow)
{
	switch (GameSettings::sharedSetting()->getLanguageType())
    {
        case 0: return LANG_ENGLISH_ARRAY[textToShow];
		break;
		case 1: return LANG_SPANISH_ARRAY[textToShow];
		break;
		case 2:	return LANG_PORTE_ARRAY[textToShow];
		break;
        case 3: return LANG_FRENCH_ARRAY[textToShow];
		break;
        case 4: return LANG_GERMAN_ARRAY[textToShow];
		break;
        default:return LANG_ENGLISH_ARRAY[textToShow];
		break;
    }
}
