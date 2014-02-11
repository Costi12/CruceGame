#include "cli.h"

#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARDS_PER_LINE 8
#define MAX_NAME_SIZE 50

void welcomeMessage()
{
    printw("Welcome to a new game of Cruce\n\n");
}

int printCard(struct Card *card, int position)
{
    char suit[] = {0xE2, 0x99, 0x00, 0x00};
    switch (card->suit) {
        case DIAMONDS:
            suit[2] = 0xA6;
            break;
        case CLUBS:
            suit[2] = 0xA3;
            break;
        case HEARTS:
            suit[2] = 0xA5;
            break;
        case SPADES:
            suit[2] = 0xA0;
            break;
        default:
            return ILLEGAL_VALUE;
    }

    char value;
    switch (card->value) {
        case 0:
            value = '9';
            break;
        case 2:
        case 3:
        case 4:
            value = '0' + card->value;
            break;
        case 10:
            value = 'X';
            break;
        case 11:
            value = 'A';
            break;
        default:
            return ILLEGAL_VALUE;
    }

    int x, y;
    getyx(stdscr, y, x);
    printw(" ___ ");
    move(y+1, x);
    printw("|%c  |", value);
    move(y+2, x);
    printw("|%s  |", suit);
    move(y+3, x);
    printw("|  %s|", suit);
    move(y+4, x);
    printw("|  %c|", value);
    move(y+5, x);
    char bottom[] = {0xE2, 0x80, 0xBE, 0x00};
    printw(" %s%s%s ", bottom, bottom, bottom);
    move(y, x + 6);
    refresh();

    return NO_ERROR;
}

int printPlayerCards(struct Player *player)
{
    if (player == NULL)
        return PLAYER_NULL;

    for (int i = 0; i < MAX_CARDS; i++) {
        if (player->hand[i] != NULL)
            printCard(player->hand[i], i);

    }
    return NO_ERROR;
}

int getNoOfPlayers()
{
    printw("Insert the number of players (2-4) ");
    char ch = getch();
    while (ch < '2' || ch > '4') {
        printw("\nPlease insert a number between 2 and 4. ");
        ch = getch();
    }
    printw("\n");

    return ch - '0';
}

struct Player *newPlayer(int i)
{
    char *name = malloc(MAX_NAME_SIZE*sizeof(char));
    printw("Insert player %d name: ", i);
    scanw("%s", name);

    struct Player *player = team_createPlayer(name, 0, 1);
    return player;
}

int printScore(struct Game *game, struct Round *round)
{
    if (game == NULL)
        return GAME_NULL;
    if (round == NULL)
        return ROUND_NULL;

    char verticalBoxDouble[]           = {0xe2, 0x95, 0x91, 0x00};
    char horizontalBoxDouble[]         = {0xe2, 0x95, 0x90, 0x00};
    char downRightBoxDouble[]          = {0xe2, 0x95, 0x94, 0x00};
    char downLeftBoxDouble[]           = {0xe2, 0x95, 0x97, 0x00};
    char upRightBoxDouble[]            = {0xe2, 0x95, 0x9a, 0x00};
    char upLeftBoxDouble[]             = {0xe2, 0x95, 0x9d, 0x00};
    char downHorizontalBoxDouble[]     = {0xe2, 0x95, 0xa6, 0x00};
    char upHorizontalBoxDouble[]       = {0xe2, 0x95, 0xa9, 0x00};
    char verticalHorizontalBoxDouble[] = {0xe2, 0x95, 0xac, 0x00};
    char verticalRightBoxDouble[]      = {0xe2, 0x95, 0xa0, 0x00}; 
    char verticalLeftBoxDouble[]       = {0xe2, 0x95, 0xa3, 0x00};

    int maxLength = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (game->players[i] != NULL) {
            int length = strlen(game->players[i]->name);
            if (length > maxLength)
                maxLength = length;
        }
    maxLength++;

    if (maxLength < 4 )
        maxLength = 4; 

    int x, y;
    int line = 0;
    getyx(stdscr, y, x);

    printw("%s",downRightBoxDouble);
    for (int i = 1; i <= maxLength + 12; i++){
        if (i == maxLength + 1 || i == maxLength + 8)
            printw("%s", downHorizontalBoxDouble);
        else
            printw("%s", horizontalBoxDouble);
    }
    printw("%s", downLeftBoxDouble);
    line++;

    move(y + 1, x);
    printw("%sNume", verticalBoxDouble);
    move(y + 1, x + maxLength + 1);
    printw("%sPuncte", verticalBoxDouble);
    move(y + 1, x + maxLength + 8);
    printw("%sScor%s", verticalBoxDouble, verticalBoxDouble);
    line++;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (game->players[i] != NULL) {
            move(y + line, x);
            printw("%s", verticalRightBoxDouble);
            for (int j = 1; j <= maxLength + 12; j++) {
                if (j == maxLength + 1 || j == maxLength + 8)
                    printw("%s", verticalHorizontalBoxDouble);
                else
                    printw("%s", horizontalBoxDouble);
            }
            printw("%s", verticalLeftBoxDouble);
            line++;
            move(y + line, x);
            printw("%s%s ", verticalBoxDouble, game->players[i]->name);
            move(y + line, x + maxLength + 1);
            printw("%s %*d", verticalBoxDouble, 5, round->pointsNumber[i]);
            move(y + line, x + maxLength + 8);
            printw("%s %*d", verticalBoxDouble, 3, game->players[i]->score);
            move(y + line, x + maxLength + 13);
            printw("%s", verticalBoxDouble);
            line++;
        }            
    }

    move(y + line, x);
    printw("%s", upRightBoxDouble); 
    for (int i = 1; i <= maxLength + 12; i++) {
        if (i == maxLength + 1 || i == maxLength + 8)
            printw("%s", upHorizontalBoxDouble);
        else
            printw("%s", horizontalBoxDouble);
    }
    printw("%s", upLeftBoxDouble);

    return NO_ERROR;
}


