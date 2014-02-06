#include <game.h>
#include <errors.h>
#include <constants.h>
#include <team.h>

#include <cutter.h>
#include <stdio.h>

void test_game_createGame()
{
    struct Game *game = game_createGame(11);

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        cut_assert_equal_pointer(NULL, game->players[i]);

    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        cut_assert_equal_pointer(NULL, game->teams[i]);

    cut_assert_equal_pointer(NULL, game_createGame(0));
    cut_assert_equal_pointer(NULL, game->deck);
    cut_assert_equal_pointer(NULL, game->round);
    cut_assert_equal_int(0, game->numberPlayers);
    cut_assert_equal_int(11, game->pointsNumber);

    game_deleteGame(&game);
}

void test_game_deleteGame()
{
    struct Game *game = game_createGame(11);

    cut_assert_equal_int(NO_ERROR, game_deleteGame(&game));
    cut_assert_equal_pointer(NULL, game);
    cut_assert_equal_int(POINTER_NULL, game_deleteGame(NULL));
    cut_assert_equal_int(GAME_NULL, game_deleteGame(&game));
}

void test_game_addPlayer()
{
    struct Game *game = game_createGame(11);
    struct Player *player[MAX_GAME_PLAYERS];

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        player[i] = team_createPlayer("A", i, i);
        cut_assert_equal_int(NO_ERROR, game_addPlayer(player[i], game));
        cut_assert_equal_int(i+1, game->numberPlayers);
        cut_assert_equal_int(DUPLICATE, game_addPlayer(player[i], game));
        int playerAdded = -1;
        for (int j = 0; j < MAX_GAME_PLAYERS; j++)
            if (game->players[j] == player[i])
                playerAdded = 0;
        cut_assert_equal_int(0, playerAdded);
    }

    struct Player *player1 = team_createPlayer("A", 0, 0);
    cut_assert_equal_int(GAME_NULL, game_addPlayer(player1, NULL));
    cut_assert_equal_int(PLAYER_NULL, game_addPlayer(NULL, game));
    cut_assert_operator_int(0, >, game_addPlayer(NULL, NULL));
    cut_assert_equal_int(FULL, game_addPlayer(player1, game));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        team_deletePlayer(&player[i]);

    team_deletePlayer(&player1);
    game_deleteGame(&game);
}

void test_game_removePlayer()
{
    struct Game *game = game_createGame(11);
    struct Player *player[MAX_GAME_PLAYERS];

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        player[i] = team_createPlayer("A", i, i);
        game_addPlayer(player[i], game);
    }

    cut_assert_equal_int(GAME_NULL, game_removePlayer(player[0], NULL));
    cut_assert_equal_int(PLAYER_NULL, game_removePlayer(NULL, game));
    cut_assert_operator_int(0, >, game_removePlayer(NULL, NULL));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, game_removePlayer(player[i], game));
        cut_assert_equal_int(NOT_FOUND, game_removePlayer(player[i], game));
        int playerRemoved = -1;
        for (int j = 0; j < MAX_GAME_PLAYERS; j++)
            if (game->players[j] == player[i])
                playerRemoved = 0;
        cut_assert_equal_int(-1, playerRemoved);
        team_deletePlayer(&player[i]);
    }

    game_deleteGame(&game);
}

void test_addTeam()
{
    struct Game *game = game_createGame(11);
    struct Team *team[MAX_GAME_TEAMS];

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        team[i] = team_createTeam("A");
        cut_assert_equal_int(NO_ERROR, game_addTeam(team[i], game));
        cut_assert_equal_int(DUPLICATE, game_addTeam(team[i], game));
        int teamAdded = -1;
        for (int j = 0; j < MAX_GAME_TEAMS; j++)
            if (game->teams[j] == team[i])
                teamAdded = 0;
        cut_assert_equal_int(0, teamAdded);
    }

    struct Team *team1 = team_createTeam("A");
    cut_assert_equal_int(GAME_NULL, game_addTeam(team1, NULL));
    cut_assert_equal_int(TEAM_NULL, game_addTeam(NULL, game));
    cut_assert_operator_int(0, >, game_addTeam(NULL, NULL));
    cut_assert_equal_int(FULL, game_addTeam(team1, game));

    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        team_deleteTeam(&team[i]);

    team_deleteTeam(&team1);
    game_deleteGame(&game);
}

void test_game_removeTeam()
{
    struct Game *game = game_createGame(11);
    struct Team *team[MAX_GAME_TEAMS];

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        team[i] = team_createTeam("A");
        game_addTeam(team[i], game);
    }

    cut_assert_equal_int(GAME_NULL, game_removeTeam(team[0], NULL));
    cut_assert_equal_int(TEAM_NULL, game_removeTeam(NULL, game));
    cut_assert_operator_int(0, >, game_removeTeam(NULL, NULL));

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        cut_assert_equal_int(NO_ERROR, game_removeTeam(team[i], game));
        cut_assert_equal_int(NOT_FOUND, game_removeTeam(team[i], game));
        int teamRemoved = -1;
        for (int j = 0; j < MAX_GAME_TEAMS; j++)
            if (game->teams[j] == team[i])
                teamRemoved = 0;
        cut_assert_equal_int(-1, teamRemoved);
        team_deleteTeam(&team[i]);
    }

    game_deleteGame(&game);
}

void test_game_winningTeam()
{
    cut_assert_equal_pointer(NULL, game_winningTeam(NULL));

    struct Game *game = game_createGame(11);
    struct Player *players[MAX_GAME_PLAYERS];
    struct Team *teams[MAX_GAME_TEAMS];

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        teams[i] = team_createTeam("A");
        game_addTeam(teams[i], game);
        players[i] = team_createPlayer("A", i, i);
        team_addPlayer(teams[i / 2], players[i]);
    }    

    cut_assert_equal_pointer(NULL, game_winningTeam(game));

    players[3]->score = 11; 
    cut_assert_equal_pointer(teams[1], game_winningTeam(game));

    players[0]->score = 11;
    cut_assert_equal_pointer(NULL, game_winningTeam(game));
    cut_assert_equal_int(21, game->pointsNumber);

    players[3]->score = 0;
    players[0]->score = 0;
    game->pointsNumber = 11;
    team_removePlayer(teams[0], players[1]);
    team_removePlayer(teams[1], players[3]);
    team_addPlayer(teams[2], players[1]);
    team_addPlayer(teams[3], players[3]);

    cut_assert_equal_pointer(NULL, game_winningTeam(game));

    players[0]->score = 11;
    cut_assert_equal_pointer(teams[0], game_winningTeam(game));

    players[1]->score = 11;
    cut_assert_equal_pointer(NULL, game_winningTeam(game));
    cut_assert_equal_int(21, game->pointsNumber);

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        team_deleteTeam(&teams[i]);
        team_deletePlayer(&players[i]);
    }

    game_deleteGame(&game);
}


