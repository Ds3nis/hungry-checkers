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

 
/**
 * Vysledek zpracovani argumentu, ze ktereho se pozdeji inicializuje hra.
 * Retezce output_file a input_file ukazuji primo do argv (nealokuji se).
 * Priznaky *_set rikaji, zda byl parametr zadan explicitne na prikazove
 * radce - dulezite, protoze hodnoty ze souboru (-i) smi prepsat jen ty,
 * ktere zadany nebyly.
 */
typedef struct {
    PlayerDesc white;
    PlayerDesc black;
    int size;
    const char *output_file;
    const char *input_file;
    int white_set;
    int black_set;
    int size_set;
} Config;

#endif 