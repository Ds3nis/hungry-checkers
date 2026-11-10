#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void game_init(Game * game,const Config *cfg) {
    // if (cfg->input_file){ 
    //     load_game(game, cfg->input_file);
    //     if (cfg->size_set) {
    //         game->
    //     }

    //     if (cfg->white_set){
    //         game->players[0] = cfg->white;
    //     }

    //     if (cfg->black_set){
    //         game->players[1] = cfg->black;
    //     }    
    // }

    Position position;
    position_init(&position, cfg->size);
    game->position = position;
    game->players[WHITE] = cfg->white;
    game->players[BLACK] = cfg->black;
    game->status = GAME_RUNNING;
    game->in_path = cfg->input_file;
    game->out_path = cfg->output_file;
    game->history = NULL;
    game->history_cap = 0;
    game->history_len = 0;
}

void position_init(Position *pos, int size){
    int row, col;
    int chekers_count = 0;
    for (row = 0; row < MAX_N; row++){
        for (col = 0; col < MAX_N; col++){
            if (row >= size || col >= size) {
                pos->cells[row][col] = CELL_OUTSIDE; 
                continue;
            }
           
            if ((row + col) % 2 != 0) { 
                if (row < (size / 2 - 1)) {
                    pos->cells[row][col] = CELL_BLACK_MAN;
                    chekers_count++;
                } else if (row > (size / 2)) {
                   pos->cells[row][col] = CELL_WHITE_MAN;
                   chekers_count++;
                } else {
                    pos->cells[row][col] = CELL_EMPTY;
                }
            } else{
                pos->cells[row][col] = CELL_EMPTY;
            }
        }
    }
    
    pos->size = size;
    pos->count[WHITE] = chekers_count / 2;
    pos->count[BLACK] = chekers_count / 2;
    pos->no_progress = 0;
    pos->to_move = WHITE; 
}