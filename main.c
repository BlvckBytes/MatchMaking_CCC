#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "include/stdparse.h"
#include "include/ccc_game.h"
#include "include/ccc_playerscore.h"
#include "include/ccc_player.h"

void proc_lvl1(void)
{
    int game_count = stdp_long();
    int player_count = stdp_long();
    int players_per_game = 2;

    CCC_Game games[game_count];
    CCC_PlayerScore max_score = {-1, -1};

    for (int i = 0; i < game_count; i++) {
        CCC_PlayerScore scores[players_per_game];
        for (int j = 0; j < players_per_game; j++) {
            CCC_PlayerScore curr = { stdp_long(), stdp_long() };
            
            if (
                curr.player_score > max_score.player_score || 
                (curr.player_score == max_score.player_score && curr.player_id < max_score.player_id)
            )
                max_score = curr;

            scores[j] = curr;
        }
        games[i] = (CCC_Game) { players_per_game, scores };
    }

    printf("%d %d\n", max_score.player_id, max_score.player_score);
}

int compare_player_wins (const void *a, const void *b)
{
    CCC_Player *player_a = (CCC_Player *) a;
    CCC_Player *player_b = (CCC_Player *) b;
    
    int win_comp = player_b->win_count - player_a->win_count;
    int id_comp = player_a->player_id - player_b->player_id;

    if (win_comp != 0) return win_comp;
    return id_comp;
}

int compare_player_points (const void *a, const void *b)
{
    CCC_Player *player_a = (CCC_Player *) a;
    CCC_Player *player_b = (CCC_Player *) b;
    
    int point_comp = player_b->points - player_a->points;
    int id_comp = player_a->player_id - player_b->player_id;

    if (point_comp != 0) return point_comp;
    return id_comp;
}

int compare_player_rating (const void *a, const void *b)
{
    CCC_Player *player_a = (CCC_Player *) a;
    CCC_Player *player_b = (CCC_Player *) b;
    
    int rating_comp = player_b->rating - player_a->rating;
    int id_comp = player_a->player_id - player_b->player_id;

    if (rating_comp != 0) return rating_comp;
    return id_comp;
}

void proc_lvl2(void)
{
    int game_count = stdp_long();
    int player_count = stdp_long();
    int players_per_game = 2;

    CCC_Game games[game_count];
    CCC_Player players[player_count];
    for (int i = 0; i < player_count; i++)
        players[i] = (CCC_Player) { i, 0, 0 };

    for (int i = 0; i < game_count; i++)
    {
        CCC_PlayerScore scores[players_per_game];
        CCC_PlayerScore winner = {-1, -1};

        for (int j = 0; j < players_per_game; j++)
        {
            CCC_PlayerScore curr = { stdp_long(), stdp_long() };
            scores[j] = curr;
            
            if (curr.player_score > winner.player_score)
                winner = curr;
        }
        games[i] = (CCC_Game) { players_per_game, scores };
        players[winner.player_id].win_count++;
    }

    qsort(players, player_count, sizeof(CCC_Player), compare_player_wins);

    for (int i = 0; i < player_count; i++)
        printf("%d %d\n", players[i].player_id, players[i].win_count);
}

void proc_lvl3(void)
{
    int win_increment = stdp_long();
    int loss_decrement = stdp_long();
    int game_count = stdp_long();
    int player_count = stdp_long();
    int players_per_game = 2;

    CCC_Game games[game_count];
    CCC_Player players[player_count];
    for (int i = 0; i < player_count; i++)
        players[i] = (CCC_Player) { i, 0, 0, 0 };

    for (int i = 0; i < game_count; i++)
    {
        CCC_PlayerScore scores[players_per_game];
        CCC_PlayerScore winner = {-1, -1};

        for (int j = 0; j < players_per_game; j++)
        {
            CCC_PlayerScore curr = { stdp_long(), stdp_long() };
            scores[j] = curr;
            
            if (curr.player_score > winner.player_score)
                winner = curr;
        }
        games[i] = (CCC_Game) { players_per_game, scores };
        players[winner.player_id].win_count++;
        players[winner.player_id].points += win_increment;

        for (int j = 0; j < players_per_game; j++)
        {
            CCC_PlayerScore curr = scores[j];
            if (curr.player_id != winner.player_id)
            {
                players[curr.player_id].loss_count++;
                players[curr.player_id].points -= loss_decrement;
            }
        }
    }

    qsort(players, player_count, sizeof(CCC_Player), compare_player_points);

    for (int i = 0; i < player_count; i++)
        printf("%d %d\n", players[i].player_id, players[i].points);
}

double chance_of_winning_a(double rating_a, double rating_b) {
    return 1.0 / (1.0 + powl(10.0, (rating_b - rating_a) / 400.0));
}

double new_rating_a(double old_rating_a, double rating_b, _Bool won) {
    return floorl(old_rating_a + 32.0 * ((won ? 1.0 : 0.0) - chance_of_winning_a(old_rating_a, rating_b)));
}

void proc_lvl4(void)
{
    int game_count = stdp_long();
    int player_count = stdp_long();
    int players_per_game = 2;

    CCC_Game games[game_count];
    CCC_Player players[player_count];
    for (int i = 0; i < player_count; i++)
        players[i] = (CCC_Player) { i, 0, 0, 0, 1000.0 };

    for (int i = 0; i < game_count; i++)
    {
        CCC_PlayerScore scores[players_per_game];
        CCC_PlayerScore winner = {-1, -1};

        for (int j = 0; j < players_per_game; j++)
        {
            CCC_PlayerScore curr = { stdp_long(), stdp_long() };
            scores[j] = curr;
            
            if (curr.player_score > winner.player_score)
                winner = curr;
        }
        games[i] = (CCC_Game) { players_per_game, scores };
        CCC_Player *player_win = &players[winner.player_id];
        player_win->win_count++;
        CCC_Player *player_lose;

        for (int j = 0; j < players_per_game; j++)
        {
            CCC_PlayerScore curr = scores[j];
            if (curr.player_id != winner.player_id)
            {
                player_lose = &players[curr.player_id];
                players[curr.player_id].loss_count++;
            }
        }

        double win_rating = player_win->rating;
        double lose_rating = player_lose->rating;

        // fprintf(stderr, "%.2f %.2f\n", win_rating, lose_rating);
        player_win->rating = new_rating_a(win_rating, lose_rating, 1);
        player_lose->rating = new_rating_a(lose_rating, win_rating, 0);
    }

    qsort(players, player_count, sizeof(CCC_Player), compare_player_rating);

    for (int i = 0; i < player_count; i++)
        printf("%d %d\n", players[i].player_id, (int)players[i].rating);
}

void play_game(CCC_Game game, CCC_Player *players) {
    long points_a = 0, points_b = 0;
    double rating_a = 0, rating_b = 0;
    for (int i = 0; i < game.player_count; i++) {
        CCC_PlayerScore score = game.scores[i];
        CCC_Player *player = &players[score.player_id];
        if (i < game.player_count / 2)
        {
            points_a += score.player_score;
            rating_a += player->rating;
        }
        else
        {
            points_b += score.player_score;
            rating_b += player->rating;
        }
    }

    if (points_a == points_b)
    {
        fprintf(stderr, "ERROR! Points are the same!\n");
        return;
    }

    for (int j = 0; j < game.player_count / 2; j++)
    {
        CCC_Player *tar = &players[game.scores[j].player_id];
        double rating_self = tar->rating;
        double rating_other = rating_b - rating_a + rating_self;
        tar->rating = new_rating_a(rating_self, rating_other, points_a > points_b);
    }

    for (int j = game.player_count / 2; j < game.player_count; j++)
    {
        CCC_Player *tar = &players[game.scores[j].player_id];
        double rating_self = tar->rating;
        double rating_other = rating_a - rating_b + rating_self;
        tar->rating = new_rating_a(rating_self, rating_other, points_a < points_b);
    }
}

void proc_lvl5(void)
{
    int game_count = stdp_long();
    int player_count = stdp_long();
    int players_per_team = stdp_long();
    int players_per_game = players_per_team * 2;

    CCC_Player players[player_count];
    for (int i = 0; i < player_count; i++)
        players[i] = (CCC_Player) { i, 0, 0, 0, 1000.0 };

    for (int i = 0; i < game_count; i++)
    {
        CCC_PlayerScore scores[players_per_game];
        CCC_Game game = (CCC_Game) { players_per_game, scores };

        for (int j = 0; j < players_per_game; j++)
            scores[j] = (CCC_PlayerScore) { stdp_long(), stdp_long() };

        play_game(game, players);
    }

    qsort(players, player_count, sizeof(CCC_Player), compare_player_rating);

    for (int i = 0; i < player_count; i++)
        printf("%d %d\n", players[i].player_id, (int)players[i].rating);
}

void swap_players(CCC_Player *players, int a, int b)
{
    CCC_Player temp = players[a];
    players[a] = players[b];
    players[b] = temp;
}

int calc_score(CCC_Player *players, int team_size)
{
    double game_score = 0;
    for (int i = 0; i < team_size * 2; i++)
        // Add if in first half, subtract if in second
        // team_a - team_b = (1 + 2) - (3 + 4) = 1 + 2 - 3 - 4
        game_score += players[i].rating * (i >= team_size ? -1 : 1);
    return fabs(game_score);
}

void optimize_game(CCC_Player *players, int team_size)
{
    // By far not all combinations, but hey - good enough, right?
    int min_score = calc_score(players, team_size);
    for (int i = 0; i < team_size * 2; i++)
    {
        for (int j = i + 1; j < team_size * 2 - 1; j++)
        {
            // Try swapping
            swap_players(players, i, j);

            // Calc new score
            int curr_score = calc_score(players, team_size);

            // Better, keep and set new bar
            if (curr_score < min_score)
            {
                min_score = curr_score;
            }
            
            // Not better, swap back
            else
                swap_players(players, i, j);
        }
    }
}

void proc_lvl6(void)
{
    int game_count = stdp_long();
    int player_count = stdp_long();
    int players_per_team = stdp_long();
    int players_per_game = players_per_team * 2;

    CCC_Player players[player_count];
    for (int i = 0; i < player_count; i++)
        players[i] = (CCC_Player) { i, 0, 0, 0, 1000.0 };

    for (int i = 0; i < game_count; i++)
    {
        CCC_PlayerScore scores[players_per_game];
        CCC_Game game = (CCC_Game) { players_per_game, scores };

        for (int j = 0; j < players_per_game; j++)
            scores[j] = (CCC_PlayerScore) { stdp_long(), stdp_long() };

        play_game(game, players);
    }

    int max_elo_diff = stdp_long(); 
    int score_threshold = stdp_long();
    int queue_size = stdp_long();

    // Load players from queue
    CCC_Player player_queue[queue_size];
    for (int i = 0; i < queue_size; i++)
        player_queue[i] = players[stdp_long()];

    // Sort player queue by rating (so that closest elo diffs are closest together)
    qsort(player_queue, queue_size, sizeof(CCC_Player), compare_player_rating);

    int total_score = 0;
    int queue_pointer = 0;

    for (int k = 0; k < queue_size / players_per_game; k++)
    {
        CCC_Player team_players[players_per_game];
        double min_elo = -1;
        double max_elo = -1;

        // Put players with closest possible elo together
        for (int i = 0; i < players_per_team; i++)
        {
            int j = players_per_team + i;

            CCC_Player *player_a = &player_queue[queue_pointer++];
            CCC_Player *player_b = &player_queue[queue_pointer++];
            double curr_diff = fabs(player_a->rating - player_b->rating);

            team_players[j] = *player_a;
            team_players[i] = *player_b;

            if (player_a->rating > max_elo)
                max_elo = player_a->rating;

            if (player_a->rating < min_elo || min_elo == -1)
                min_elo = player_a->rating;

            if (player_b->rating > max_elo)
                max_elo = player_b->rating;

            if (player_b->rating < min_elo || min_elo == -1)
                min_elo = player_b->rating;
        }

        // Optimize order of players in game array to minimize score
        optimize_game(team_players, players_per_team);

        // Print player ids
        for (int i = 0; i < players_per_game; i++)
            printf("%d ", team_players[i].player_id);
        printf("\n");

        // Calc and add game score to total score
        int game_score = calc_score(team_players, players_per_team);
        total_score += game_score;

        // Calc max elo difference in game
        int game_max_elo_diff = fabs(max_elo - min_elo);
        if (game_max_elo_diff > max_elo_diff)
            fprintf(stderr, "ERROR! Max elo diff went over barrier! (%d, %d)\n", game_max_elo_diff, max_elo_diff);
    }

    if (total_score > score_threshold)
        fprintf(stderr, "ERROR! Total score went over score threshold barrier! (%d, %d)\n", total_score, score_threshold);
}

int main(int argc, char *argv[])
{
    if (argc < 2) return 1;
    int targ_level = atoi(argv[1]);

    switch (targ_level)
    {
    case 1:
        proc_lvl1();
        break;
    
    case 2:
        proc_lvl2();
        break;
    
    case 3:
        proc_lvl3();
        break;

    case 4:
        proc_lvl4();
        break;

    case 5:
        proc_lvl5();
        break;

    case 6:
        proc_lvl6();
        break;
    }

    return 0;
}