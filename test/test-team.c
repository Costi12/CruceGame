#include <team.h>
#include <errors.h>
#include <constants.h>

#include <cutter.h>
#include <stdio.h>

static struct Team team;

void test_team_createPlayer()
{
    cut_assert_equal_pointer(NULL, team_createPlayer(NULL, 0));

    struct Player *player;
    for (int i = 0; i < 100; i++) {
        player = team_createPlayer("A", i);
        cut_assert_equal_string("A", player->name);
        cut_assert_equal_int(i, player->isHuman);
        cut_assert_equal_int(0, player->score);
        team_deletePlayer(&player);
    }
}

void test_team_createTeam()
{
    struct Team *team;
    for (int i = 0; i < 100; i++) {
        team = team_createTeam();
        cut_assert_equal_int(0, team->score);
        team_deleteTeam(&team);
    }
}

void test_team_addPlayer()
{
    cut_assert_not_equal_int(NO_ERROR, team_addPlayer(NULL, NULL));

    struct Team *team1 = team_createTeam();
    struct Player *testPlayer[MAX_TEAM_PLAYERS];
    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        testPlayer[i] = team_createPlayer("A", i);
        cut_assert_equal_int(NO_ERROR, team_addPlayer(team1, testPlayer[i]));
        cut_assert_equal_int(DUPLICATE, team_addPlayer(team1, testPlayer[i]));
        int playerAdded = -1; //0 the player was added. Otherwise -1.
        for (int j = 0; j < MAX_TEAM_PLAYERS; j++)
            if (team1->players[j] == testPlayer[i])
                playerAdded = 0;
        cut_assert_equal_int(0, playerAdded);
    }

    struct Player *player1 = team_createPlayer("A", 1);
    cut_assert_equal_int(TEAM_NULL, team_addPlayer(NULL, player1));
    cut_assert_equal_int(PLAYER_NULL, team_addPlayer(team1, NULL));
    cut_assert_equal_int(TEAM_FULL, team_addPlayer(team1, player1));

    team_deletePlayer(&player1);
    team_deleteTeam(&team1);

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        team_deletePlayer(&testPlayer[i]);
    }
}

void test_team_removePlayer()
{
    cut_assert_not_equal_int(NO_ERROR, team_removePlayer(NULL, NULL));

    struct Team *team1 = team_createTeam(); 
    struct Player *testPlayer[MAX_TEAM_PLAYERS];
    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        testPlayer[i] = team_createPlayer("A", i);
        team_addPlayer(team1, testPlayer[i]);
    }

    cut_assert_equal_int(PLAYER_NULL, team_removePlayer(team1, NULL));
    cut_assert_equal_int(TEAM_NULL, team_removePlayer(NULL, testPlayer[0]));

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, team_removePlayer(team1, testPlayer[i]));

        int removed = 1;
        for (int j = 0; j < MAX_TEAM_PLAYERS; j++) {
            if (team1->players[j] == testPlayer[i])
                removed = 0;
        }
        cut_assert_equal_int(1, removed);

        cut_assert_equal_int(NOT_FOUND,
                             team_removePlayer(team1, testPlayer[i]));
    }

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        team_deletePlayer(&testPlayer[i]);
    }

    team_deleteTeam(&team1);
}

void test_team_deletePlayer()
{
    struct Player *player = team_createPlayer("A", 0);

    cut_assert_equal_int(NO_ERROR, team_deletePlayer(&player));
    cut_assert_equal_pointer(NULL, player);
    cut_assert_equal_int(POINTER_NULL, team_deletePlayer(NULL));

}

void test_team_deleteTeam()
{
    struct Team *team = team_createTeam();

    cut_assert_equal_int(NO_ERROR, team_deleteTeam(&team));
    cut_assert_equal_pointer(NULL, team);
    cut_assert_equal_int(POINTER_NULL, team_deleteTeam(NULL));

}

void test_team_addCard()
{
    struct Player *player = team_createPlayer("A", 1);
    struct Deck *deck = deck_createDeck();

    for (int i = 0; i < MAX_CARDS; i++) {
        cut_assert_equal_int(NO_ERROR, team_addCard(player, deck->cards[i]));
        cut_assert_equal_int(DUPLICATE, team_addCard(player, deck->cards[i]));
    }

    cut_assert_equal_int(FULL, team_addCard(player, deck->cards[MAX_CARDS]));

    cut_assert_equal_int(PLAYER_NULL, team_addCard(NULL, NULL));
    cut_assert_equal_int(PLAYER_NULL, team_addCard(NULL, deck->cards[0]));
    cut_assert_equal_int(CARD_NULL, team_addCard(player, NULL));

    deck_deleteDeck(&deck);
    team_deletePlayer(&player);
}

void test_team_hasCards()
{
    struct Player *player = team_createPlayer("A", 1);
    struct Deck *deck = deck_createDeck();

    cut_assert_equal_int(PLAYER_NULL, team_hasCards(NULL));

    cut_assert_equal_int(0, team_hasCards(player));

    team_addCard(player, deck->cards[0]);
    cut_assert_equal_int(1, team_hasCards(player));

    deck_deleteDeck(&deck);
    team_deletePlayer(&player);
}

void test_team_updatePlayersScore()
{
    cut_assert_equal_int(TEAM_NULL, team_updatePlayersScore(NULL));
    struct Team *team = team_createTeam();
    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        struct Player *player = team_createPlayer("A", 0);
        team_addPlayer(team, player);
    }
    team->score = 3;
    cut_assert_equal_int(NO_ERROR, team_updatePlayersScore(team));
    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        if (team->players[i] != NULL)
            cut_assert_equal_int(team->players[i]->score, team->score);
    }
}

void test_team_computePoints()
{
    struct Team *teamA = team_createTeam();
    struct Team *teamB = team_createTeam();
    struct Round *round = round_createRound();

    struct Player *player[MAX_GAME_PLAYERS];

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        player[i] = team_createPlayer("A", i);
        round_addPlayer(player[i], round);
        round->pointsNumber[i] = 2*i;
    }

    cut_assert_not_equal_int(NO_ERROR, team_computePoints(NULL, NULL));
    cut_assert_equal_int(ROUND_NULL, team_computePoints(teamA, NULL));
    cut_assert_equal_int(TEAM_NULL, team_computePoints(NULL, round));

    cut_assert_equal_int(TEAM_EMPTY, team_computePoints(teamB, round));

    team_addPlayer(teamA, player[0]);
    team_addPlayer(teamA, player[1]);
    team_addPlayer(teamB, player[2]);
    team_addPlayer(teamB, player[3]);

    cut_assert_equal_int(2, team_computePoints(teamA, round));
    cut_assert_equal_int(10, team_computePoints(teamB, round));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        team_deletePlayer(&round->players[i]);

    round_deleteRound(&round);

    team_deleteTeam(&teamA);
    team_deleteTeam(&teamB);

}

