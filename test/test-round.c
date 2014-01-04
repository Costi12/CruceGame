#include <round.h>
#include <cutter.h>
#include <stdlib.h>
#include <stdio.h>

static struct Hand *hand;
static struct Player *player1;
static struct Player *player2;
static struct Player *player3;
static struct Player *player4;

void cut_setup()
{
    hand = malloc(sizeof(struct Hand));
    hand->players[0] = NULL;

    player1 = malloc(sizeof(struct Player));
    player2 = malloc(sizeof(struct Player));
    player3 = malloc(sizeof(struct Player));
    player4 = malloc(sizeof(struct Player));
}

void cut_teardown()
{
    free(hand);
    free(player1);
    free(player2);
    free(player3);
    free(player4);
}

void test_addPlayer()
{
    cut_assert_not_equal_int(0, addPlayer(NULL, hand));
    cut_assert_not_equal_int(0, addPlayer(player1, NULL));
    cut_assert_not_equal_int(0, addPlayer(NULL, NULL));

    cut_assert_equal_int(0, addPlayer(player1, hand));
    cut_assert_equal_pointer(hand->players[1], NULL);

    cut_assert_equal_int(0, addPlayer(player2, hand));
    cut_assert_equal_pointer(hand->players[2], NULL);

    cut_assert_equal_int(0, addPlayer(player3, hand));
    cut_assert_equal_pointer(hand->players[3], NULL);

    cut_assert_equal_int(0, addPlayer(player4, hand));
    cut_assert_equal_pointer(hand->players[4], NULL);

    cut_assert_equal_pointer(hand->players[0], player1);
    cut_assert_equal_pointer(hand->players[1], player2);
    cut_assert_equal_pointer(hand->players[2], player3);
    cut_assert_equal_pointer(hand->players[3], player4);
}

void printHand()
{
    fprintf(stderr, "%p %p\n", player1, hand->players[0]);
    fprintf(stderr, "%p %p\n", player2, hand->players[1]);
    fprintf(stderr, "%p %p\n", player3, hand->players[2]);
    fprintf(stderr, "%p %p\n", player4, hand->players[3]);
}

void printBids()
{
    fprintf(stderr, "%d\n", hand->bids[0]);
    fprintf(stderr, "%d\n", hand->bids[1]);
    fprintf(stderr, "%d\n", hand->bids[2]);
    fprintf(stderr, "%d\n", hand->bids[3]);
}

