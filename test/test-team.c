#include <team.h>
#include <cutter.h>

static struct Team team;

void test_team_createPlayer()
{
    struct Player *player;
    for (int i = 0; i < 100; i++) {
        player = team_createPlayer("A", i, i);
        cut_assert_equal_string("A", player->name);
        cut_assert_equal_int(i, player->sockfd);
        cut_assert_equal_int(i, player->isHuman);
        cut_assert_equal_int(0, player->score);
        cut_assert_operator_int(-1, <, player->id);
    }
}

void test_team_createTeam()
{
    struct Team *team;
    for (int i = 0; i < 100; i++) {
        team = team_createTeam("A");
        cut_assert_equal_string("A", team->name);
        cut_assert_operator_int(-1, <, team->id);
    }
}

void test_team_addPlayer()
{  
    cut_assert_equal_int(-1, team_addPlayer(NULL, NULL));
    
    struct Team *team1 = team_createTeam("E");
    cut_assert_equal_int(-1, team_addPlayer(team1, NULL));
    
    struct Player *player1 = team_createPlayer("A", 0, 0);
    cut_assert_equal_int(-1, team_addPlayer(NULL, player1));
    
    struct Player *testPlayer[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++)
        testPlayer[i] = team_createPlayer("A", i, i);
    
    for (int i = 0; i < MAX_PLAYERS; i++) {
        cut_assert_equal_int(0, team_addPlayer(team1, testPlayer[i]));
        cut_assert_equal_int(-1, team_addPlayer(team1, testPlayer[i]));
        int playerAdded = -1; //0 the player was added. Otherwise -1.
        for (int j = 0; j < MAX_PLAYERS; j++)
            if (team1->players[j] == testPlayer[i])
                playerAdded = 0;
        cut_assert_equal_int(0, playerAdded);
    }
        
    cut_assert_equal_int(-1, team_addPlayer(team1, testPlayer[0]));
    
    for (int i = 0; i < MAX_PLAYERS; i++) {
        free(testPlayer[i]);
        testPlayer[i] = NULL;
    }
    
    free(team1);
    team1 = NULL;
    free(player1);
    player1 = NULL;
}

void test_team_removePlayer()
{
    cut_assert_equal_int(-1, team_removePlayer(NULL, NULL));
    
    struct Team *team1 = team_createTeam("A");
    cut_assert_equal_int(-1, team_removePlayer(team1, NULL));
    
    struct Player *player1 = team_createPlayer("P", 1, 1);
    cut_assert_equal_int(-1, team_removePlayer(NULL, player1));
    cut_assert_equal_int(-1, team_removePlayer(team1, player1));
    free(player1);
    player1 = NULL;
    
    struct Player *testPlayer[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++) {
        testPlayer[i] = team_createPlayer("A", i, i);
        cut_assert_equal_int(0, team_addPlayer(team1, testPlayer[i]));
        cut_assert_equal_int(0, team_removePlayer(team1, testPlayer[i]));
        free(testPlayer[i]);
        testPlayer[i] = NULL;
    }
    
    free(team1);
    team1 = NULL;
}