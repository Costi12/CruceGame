#ifndef ERRORS_H
#define ERRORS_H

enum ERROR_CODE {
    NO_ERROR = 0,

    POINTER_NULL = -1,
    MALLOC_ERROR = -2,

    CARD_NULL = -3,
    DECK_NULL = -4,
    PLAYER_NULL = -5,
    TEAM_NULL = -6,

    TEAM_FULL = -7,
    TEAM_EMPTY = -8,

    DUPLICATE = -9,
    NOT_FOUND = -10,

    ROUND_NULL = -11
};

#endif
