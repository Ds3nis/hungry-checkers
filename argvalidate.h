#ifndef __ARGVALIDATE_H__
#define __ARGVALIDATE_H__

/** Nazvy prepinacu na prikazove radce. */
#define WHITE_PLAYER_DESC "-pw"
#define BLACK_PLAYER_DESC "-pb"
#define AREA_SIZE "-size"
#define OUTPUT_FILE "-o"
#define INPUT_FILE "-i"
 
/** Vychozi a mezni hodnoty parametru. */
#define DEFAULT_BOARD_SIZE 8
#define MIN_BOARD_SIZE 4
#define MAX_BOARD_SIZE 26
#define DEFAULT_DEPTH 3
 
/** Navratovy kod pro neznamy prepinac (zadani ho nedefinuje). */
#define ERR_UNKNOWN_OPTION 5
 
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
 
/**
 * Nastavi vychozi hodnoty.
 * @param cfg konfigurace k inicializaci
 */
void config_init(Config *cfg);
 
/**
 * Zpracuje a zvaliduje vsechny argumenty prikazove radky.
 * Pri chybe vypise chybovou hlasku a vrati navratovy kod programu.
 * @param argc pocet argumentu
 * @param argv pole argumentu
 * @param cfg vystupni konfigurace
 * @return 0 pri uspechu, jinak navratovy kod, kterym ma program skoncit
 */
int parse_args(int argc, char *argv[], Config *cfg);

#endif