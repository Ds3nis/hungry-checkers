#ifndef __ARGVALIDATE_H__
#define __ARGVALIDATE_H__
#include "types.h"

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