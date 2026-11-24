#ifndef __GAME_H__
#define __GAME_H__
#include "types.h"

#define MAX_N 26

#define MAX_KING_TARGETS 2 * MAX_N

#define MAX_MOVES 1024

#ifndef MAN_CAPTURES_BACKWARD
/** Smí obyčejný kámen brát i dozadu? Zadání to výslovně neuvádí. */
#define MAN_CAPTURES_BACKWARD 0
#endif


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

/** Druh tahu; vyšší hodnota má přednost (braní je povinné). */
typedef enum {
    MOVE_QUIET = 0,         /**< přesun bez braní */
    MOVE_MAN_CAPTURE = 1,   /**< braní obyčejným kamenem */
    MOVE_KING_CAPTURE = 2   /**< braní dámou */
} MoveKind;

/**
 * Seznam legálních tahů jednoho druhu. Obsahuje vždy jen tahy nejvyššího
 * druhu, který byl nalezen (viz movelist_add), takže pravidlo povinného
 * braní se uplatní samo.
 */
typedef struct {
    Move moves[MAX_MOVES];
    int count;
    MoveKind kind;
} MoveList;

typedef struct {
    Move moves[MAX_KING_TARGETS];   /* 2 * MAX_N */
    int count;
} MoveBuf;

/* Čistá pozice: kopíruje se přiřazením, bez malloc */
typedef struct {
    Cell cells[MAX_N][MAX_N];
    int  size;
    int  count[2];       /* kameny + dámy podle barvy */
    int  no_progress;    /* čítač tahů bez pokroku */
    Color to_move;
} Position;

typedef enum {
    GAME_RUNNING, 
    GAME_WHITE_WINS, 
    GAME_BLACK_WINS, 
    GAME_DRAW
} GameStatus;


typedef struct game {
    Position position;
    PlayerDesc players[2];
    GameStatus status;
    Move *history;
    int history_len, history_cap;
    const char *in_path, *out_path;
} Game;



void game_init(Game * game,const Config *cfg);

void game_run(Game * game);

/**
 * Zjistí, zda leží souřadnice uvnitř hrací plochy.
 * @param row řádek
 * @param col sloupec
 * @param size velikost plochy
 * @return nenulová hodnota, pokud je souřadnice platná
 */
int in_board(int row, int col, int size);

/** @return nenulová hodnota, pokud je kámen dáma */
int is_king(Cell cell);

/** @return nenulová hodnota, pokud pole obsahuje kámen či dámu dané barvy */
int cell_belongs_to_color(Cell cell, Color color);

/** @return nenulová hodnota, pokud pole obsahuje soupeřův kámen či dámu */
int is_enemy(Cell cell, Color color);

/**
 * Vytvoří tah.
 * @param cr řádek braného kamene, nebo -1 (bez braní)
 * @param cc sloupec braného kamene, nebo -1 (bez braní)
 */
Move make_move(int fr, int fc, int tr, int tc, int cr, int cc);

/**
 * Přidá tah do seznamu. Tah nižšího druhu než dosud nalezený se zahodí,
 * tah vyššího druhu seznam nejdřív vyprázdní.
 * @param list seznam tahů
 * @param move přidávaný tah
 * @param kind druh tahu
 */
void movelist_add(MoveList *list, Move move, MoveKind kind);

/**
 * Vygeneruje všechny legální tahy hráče na tahu (pos->to_move) v pořadí
 * požadovaném zadáním (důležité pro Minimax). Při existenci braní vrací jen
 * braní, přičemž braní dámou má přednost před braním kamenem.
 * @param pos pozice
 * @param list výstupní seznam tahů (je vždy nejdřív vyprázdněn)
 */
void gen_moves(const Position *pos, MoveList *list);

void print_board(Game *game);


#endif