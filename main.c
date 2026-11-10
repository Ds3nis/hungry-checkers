#include <stdio.h>
#include <stdlib.h>
#include "argvalidate.h"
#include "game.h"


int main(int argc, char *argv[]) {
    Config cfg;
    int result = 0;
    
    config_init(&cfg);
    result = parse_args(argc, argv, &cfg);

    if(result) {
        return result;
    }

    Game game;
    game_init(&game, &cfg);
    print_board(&game);
    
    return EXIT_SUCCESS;
}