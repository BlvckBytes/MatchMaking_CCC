#ifndef CCC_PLAYER_H_
#define CCC_PLAYER_H_

typedef struct CCC_Player
{
    int player_id;
    int win_count;
    int loss_count;
    int points;
    double rating;
} CCC_Player;

#endif