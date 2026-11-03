#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argvalidate.h"

typedef int (*ValidateFn) (const char *arg, Config *cfg);

typedef struct toggle {
    char *name;
    int expected_args;
    ValidateFn validate;
    const char *error_msg;
    int return_value;
    const char *help;
} Toggle;

int parse_long(const char *s, long *out) {
    char *end;

    if (s == NULL || *s == '\0') {
        return 0;
    }

    errno = 0;
    *out = strtol(s, &end, 10);

    if (errno != 0 || *end != '\0') {
        return 0;
    }

    return 1;
}

int parse_player(const char *arg, PlayerDesc *out) {
    const char *comma = strchr(arg, ',');

    size_t type_len = (comma == NULL) ? strlen(arg) : (size_t) (comma - arg);
    PlayerDesc tmp;
    long depth;

    tmp.depth = DEFAULT_DEPTH;
    if (type_len == strlen("human") && strncmp(arg, "human", type_len) == 0) {
        tmp.type = PLAYER_HUMAN;
    } else if (type_len == strlen("minimax") && strncmp(arg, "minimax", type_len) == 0) {
        tmp.type = PLAYER_MINIMAX;

        if(comma != NULL) {
            if (!parse_long(comma + 1, &depth) || depth < 0 || depth > INT_MAX) {
                return 1;
            }
            tmp.depth = (int)depth;
        }
    } else {
        return 1;
    }

    *out = tmp;
    return 0;
}


int validate_white(const char *arg, Config *cfg) { 
    if (parse_player(arg, &cfg->white) != 0) {
        return 1;
    }

    cfg->white_set = 1;
    return 0;
}

int validate_black(const char *arg, Config *cfg) {
    if (parse_player(arg, &cfg->black) != 0) {
        return 1;
    }

    cfg->black_set = 1;
    return 0;
}

int validate_size(const char *arg, Config *cfg) {
    long size;

    if (!parse_long(arg, &size) || size % 2 != 0 || size < MIN_BOARD_SIZE || size > MAX_BOARD_SIZE) { 
        return 2;
    }

    cfg->size_set = (int) size;
    return 0;
}

int validate_output(const char *arg, Config *cfg) {
    FILE *f = fopen(arg, "a");
 
    if (f == NULL) {
        return 1;
    }
    fclose(f);
    cfg->output_file = arg;
    return 0;
}

int validate_input(const char *arg, Config *cfg) {
    FILE *f = fopen(arg, "r");
 
    if (f == NULL) {
        return 1;
    }
    fclose(f);
    cfg->input_file = arg;
    return 0;
}


/** Tabulka vsech podporovanych prepinacu. */
static const Toggle toggles[] = {
    {WHITE_PLAYER_DESC, 1, validate_white, "Invalid player description!\n", 1,
     "Popis hrace s bilymi kameny.\n"},
    {BLACK_PLAYER_DESC, 1, validate_black, "Invalid player description!\n", 1,
     "Popis hrace s cernymi kameny.\n"},
    {AREA_SIZE, 1, validate_size, "Invalid board size!\n", 2,
     "Pocet radku a sloupcu (vychozi: 8), sude hodnoty z <4, 26>.\n"},
    {OUTPUT_FILE, 1, validate_output, "Invalid output file destination!\n", 3,
     "Soubor, do ktereho se ulozi stav hry.\n"},
    {INPUT_FILE, 1, validate_input, "Invalid input file!\n", 4,
     "Soubor, ze ktereho se nacte hra.\n"}
};

const size_t toggles_size = sizeof(toggles) / sizeof(toggles[0]);




void config_init(Config *cfg) {
    cfg->white.type = PLAYER_HUMAN;
    cfg->white.depth = DEFAULT_DEPTH;
    cfg->black.type = PLAYER_HUMAN;
    cfg->black.depth = DEFAULT_DEPTH;
    cfg->size = DEFAULT_BOARD_SIZE;
    cfg->output_file = NULL;
    cfg->input_file = NULL;
    cfg->white_set = 0;
    cfg->black_set = 0;
    cfg->size_set = 0;
}

static const Toggle *find_toggle(const char *name) {
    int toggle_idx;

    for(toggle_idx = 0; toggle_idx < (int) toggles_size; toggle_idx++) {
        if(!strcmp(toggles[toggle_idx].name, name)) {
            return &toggles[toggle_idx];
        }
    }

    return NULL;
}


int parse_args(int argc, char *argv[], Config *cfg) {
    int i = 1;
    const Toggle *toggle;

    while(i < argc) {
        toggle = find_toggle(argv[i]);

        if(toggle == NULL) {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return ERR_UNKNOWN_OPTION;
        }

        if (i + toggle->expected_args >= argc || toggle->validate(argv[i + 1], cfg) != 0) {
            fputs(toggle->error_msg, stderr);
            return toggle->return_value;
        }
        i += 1 + toggle->expected_args;
    }

    return 0;
}
