#ifndef __TYPES_H__
#define __TYPES_H__

/** Typ hrace. */
typedef enum {
    PLAYER_HUMAN,
    PLAYER_MINIMAX
} PlayerType;
 
/** Popis hrace: typ a hloubka stromu (vyznam jen pro minimax). */
typedef struct {
    PlayerType type;
    int depth;
} PlayerDesc;

#endif 