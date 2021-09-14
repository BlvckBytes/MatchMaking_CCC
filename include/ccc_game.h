#include "ccc_playerscore.h"

#ifndef CCC_Game_H_
#define CCC_Game_H_

typedef struct CCC_Game
{
    int player_count;
    CCC_PlayerScore *scores;
} CCC_Game;


#endif