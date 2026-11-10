#ifndef __GAME_H__
#define __GAME_H__
#include "types.h"

#define MAX_N 26

typedef enum {
    GAME_RUNNING, 
    GAME_WHITE_WINS, 
    GAME_BLACK_WINS, 
    GAME_DRAW
} GameStatus;

typedef enum {
    CELL_EMPTY = 0,
    CELL_WHITE_MAN, CELL_WHITE_KING,
    CELL_BLACK_MAN, CELL_BLACK_KING,
    CELL_OUTSIDE 
} Cell;

typedef enum { WHITE = 0, BLACK = 1 } Color;

typedef struct {
    int fr, fc;      /* odkud */
    int tr, tc;      /* kam */
    int cr, cc;      /* pozice braného kamene, nebo -1 */
} Move;

/* Čistá pozice: kopíruje se přiřazením, bez malloc */
typedef struct {
    Cell cells[MAX_N][MAX_N];
    int  size;
    int  count[2];       /* kameny + dámy podle barvy */
    int  no_progress;    /* čítač tahů bez pokroku */
    Color to_move;
} Position;

typedef struct game {
    Position position;
    PlayerDesc players[2];
    GameStatus status;
    Move *history;
    int history_len, history_cap;
    const char *in_path, *out_path;
} Game;



void game_init(Game * game, Config *cfg);

#endif