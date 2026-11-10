#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"


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

void set_symbol(int cell, char *piece) {
    switch (cell){
        case CELL_EMPTY:
            *piece = '-';
            break;
        case CELL_BLACK_KING:
            *piece = 'X';
            break;
        case CELL_WHITE_KING:
            *piece = 'O';
            break;
        case CELL_BLACK_MAN:
            *piece = 'x';
            break;
        case CELL_WHITE_MAN:
            *piece = 'o';
            break;
        case CELL_OUTSIDE:
            *piece = '#';
            break;
        default:
            *piece = '?';
            break;
    }
}

void print_board(Game *game){
    int row, col, i;
    int board_size = game->position.size;
    char piece;
    for (row = 0; row < board_size; row++) {
        if (row == 0) {
            printf("+");
            for (i = 0; i < (board_size * 2 - 1); i++) {
                printf("-");
            }
            printf("+\n");
        }
        for (col = 0; col < board_size; col++) {
            col == 0 ? printf("|") : 1;
            set_symbol(game->position.cells[row][col], &piece);
            col == (board_size - 1) ? printf("%c", piece) : printf("%c ", piece);
            if (col == (board_size - 1)) {
                printf("| %2d\n", board_size - row);
            }  
        }
        if (row == (board_size - 1)) {
            printf("+");
            for (i = 0; i < (board_size * 2 - 1); i++) {
                printf("-");
            }
            printf("+ \n ");
            for (int i = 0; i < board_size; i++) {
                printf("%c ", 65 + i);
            }
        }
    }
}