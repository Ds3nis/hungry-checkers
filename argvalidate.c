#include <stdio.h>
#include <stdlib.h>

#include "argvalidate.h"


typedef struct toggle {
    char *name;
    int expexted_args;
    const char **arg_error_msgs;
    int return_value;
    int (*validate)(const char *arg, Config *cfg);
    const char *help;
} Toggle;


// Toggle toggles[] = {
//     {WHITE_PLAYER_DESC, 1, "Invalid player description!\n", 1, "Popis hráče s bílými kameny.\n"},
//     {BLACK_PLAYER_DESC, 1, "Invalid player description!\n", 1, "Popis hráče s černými kameny.\n"},
//     {AREA_SIZE, 1, "Invalid board size!\n", 2, "Argument N označuje počet řádků a sloupců hrací plochy (výchozí:8). Povoleny jsou pouze sudé hodnoty v rozsahu <4, 26>.\n"},
//     {OUTPUT_FILE, 1, "Invalid output file destination!\n", 3, "Do souboru gamefile bude v předepsaném formátu uložen aktuální stav hry. \n"},
//     {INPUT_FIlE, 1, "Invalid input file!\n", 4, "Popis hráče s bílými kameny."}
// };
