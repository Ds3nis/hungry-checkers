#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include <assert.h>


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

    position_init(&game->position, cfg->size);
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

void game_run(Game *game) {

    while (game->status == GAME_RUNNING)
    {
        
    }

}

int in_board(int row, int col, int size) {
    return row >= 0 && row < size && col >= 0 && col < size;
}

int is_king(Cell cell) {
    return cell == CELL_WHITE_KING || cell == CELL_BLACK_KING;
}

int cell_belongs_to_color(Cell cell, Color color) {
    if (color == WHITE) {
        return cell == CELL_WHITE_MAN || cell == CELL_WHITE_KING;
    }
    return cell == CELL_BLACK_MAN || cell == CELL_BLACK_KING;
}

int is_enemy(Cell cell, Color color) {
    Color opponent = (color == WHITE) ? BLACK : WHITE;

    return cell_belongs_to_color(cell, opponent);
}

Move make_move(int fr, int fc, int tr, int tc, int cr, int cc) {
    Move move;

    move.fr = fr;
    move.fc = fc;
    move.tr = tr;
    move.tc = tc;
    move.cr = cr;
    move.cc = cc;
    return move;
}

void movelist_add(MoveList *list, Move move, MoveKind kind) {
    if (kind < list->kind) {
        return;
    }
    if (kind > list->kind) {
        list->kind = kind;
        list->count = 0;
    }
    assert(list->count < MAX_MOVES);
    list->moves[list->count] = move;
    list->count++;
}

/** Obrátí pořadí tahů v seznamu. */
static void movelist_reverse(MoveList *list) {
    int i;
    int j;
    Move tmp;

    for (i = 0, j = list->count - 1; i < j; i++, j--) {
        tmp = list->moves[i];
        list->moves[i] = list->moves[j];
        list->moves[j] = tmp;
    }
}

/**
 * Přidá tahy obyčejného kamene na poli (row, col): přesun o jedno pole
 * diagonálně vpřed a braní skokem přes soupeřův kámen.
 * Směry se procházejí od "nahoře vlevo", takže cíle vznikají vzestupně
 * podle (řádek, sloupec), jak vyžaduje základní pořadí pro bílé.
 */
static void gen_man_moves(const Position *pos, int row, int col, MoveList *list) {
    int forward = (pos->to_move == WHITE) ? -1 : 1;
    int dr, dc, nr, nc, lr, lc;

    for (dr = -1; dr <= 1; dr += 2) {
        for (dc = -1; dc <= 1; dc += 2) {
            nr = row + dr;
            nc = col + dc;

            /* Přesun bez braní je možný jen vpřed. */
            if (dr == forward && in_board(nr, nc, pos->size)
                && pos->cells[nr][nc] == CELL_EMPTY) {
                movelist_add(list, make_move(row, col, nr, nc, -1, -1), MOVE_QUIET);
            }

            /* Braní: soupeřův kámen vedle, za ním volné pole. */
            if (dr == forward || MAN_CAPTURES_BACKWARD) {
                lr = row + 2 * dr;
                lc = col + 2 * dc;
                if (in_board(lr, lc, pos->size)
                    && is_enemy(pos->cells[nr][nc], pos->to_move)
                    && pos->cells[lr][lc] == CELL_EMPTY) {
                    movelist_add(list, make_move(row, col, lr, lc, nr, nc),
                                 MOVE_MAN_CAPTURE);
                }
            }
        }
    }
}


static int cmp_king_moves(const void *m1, const void *m2) {
    const Move *a = (const Move *)m1;
    const Move *b = (const Move *)m2;

    if (a->tr != b->tr) {
        return a->tr - b->tr;
    }

    return a->tc - b->tc;
}

void sort_king_moves(Move *quiet_mv, int qm, Move *capture_mv, int cm) {
    qsort(quiet_mv, qm, sizeof(Move), cmp_king_moves);
    qsort(capture_mv, cm, sizeof(Move), cmp_king_moves);
}

static void scan_direction(const Position *pos, int row, int col, int dr, int dc, MoveBuf *quiet, MoveBuf *captures) {
    int k, k1, nr, nc, lr, lc;

    for (k = 1; k < pos->size; k++) {
                nr = row + k * dr;
                nc = col + k * dc;

                if (!in_board(nr, nc, pos->size) || cell_belongs_to_color(pos->cells[nr][nc], pos->to_move)) {
                    break;
                }

                if (pos->cells[nr][nc] == CELL_EMPTY) {
                    quiet->moves[quiet->count++] = make_move(row, col, nr, nc, -1, -1);
                }
        
                if (is_enemy(pos->cells[nr][nc], pos->to_move)) {
                    for (k1 = k + 1; k1 < pos->size; k1++) {
                        lr = row + k1 * dr;
                        lc = col + k1 * dc;
                        if (!in_board(lr, lc, pos->size) || pos->cells[lr][lc] != CELL_EMPTY) {
                            break;
                        }
                        captures->moves[captures->count++] = make_move(row, col, lr, lc, nr, nc);
                    }
                    break;
                }        
    } 
}


/**
 * Přidá tahy dámy na poli (row, col).
 * TODO: doplnit (přesuny i braní; braní přidávejte s MOVE_KING_CAPTURE;
 * cíle jedné dámy musí vznikat vzestupně podle (řádek, sloupec)).
 */
static void gen_king_moves(const Position *pos, int row, int col, MoveList *list) {
    int dc, dr, i;
    MoveBuf quiet_moves;
    MoveBuf capture_moves;
    
    for (dr = -1; dr <= 1; dr+=2) {
        for (dc = -1; dc <= 1; dc+=2) {       
            scan_direction(pos, row, col, dr, dc, &quiet_moves, &capture_moves);    
        }
    }

    sort_king_moves(quiet_moves.moves, quiet_moves.count, capture_moves.moves, capture_moves.count);
    for (i = 0; i < quiet_moves.count; i++) {
        movelist_add(list, quiet_moves.moves[i], MOVE_QUIET);
    }

    for (i = 0; i < capture_moves.count; i++) {
        movelist_add(list, capture_moves.moves[i], MOVE_KING_CAPTURE);
    }

}

void gen_moves(const Position *pos, MoveList *list) {
    int row;
    int col;
    Cell cell;

    list->count = 0;
    list->kind = MOVE_QUIET;

    /* Vždy generujeme v pořadí pro bílé: shora dolů, zleva doprava. */
    for (row = 0; row < pos->size; row++) {
        for (col = 0; col < pos->size; col++) {
            cell = pos->cells[row][col];
            if (cell_belongs_to_color(cell, pos->to_move)) {
                if (is_king(cell)) {
                    gen_king_moves(pos, row, col, list);
                } else {
                    gen_man_moves(pos, row, col, list);
                }
            }
        }
    }

    /* Pořadí pro černé je přesně opačné než pro bílé. */
    if (pos->to_move == BLACK) {
        movelist_reverse(list);
    }
}

int has_captured_checker(const Move *move) {
    return move->cc > 0 && move->cr > 0;
}

int should_promote_to_king(int size, Cell from, int to_row) {
    if (is_king(from)) return 0;

    if (cell_belongs_to_color(from, WHITE)) {
        return to_row == 0;
    }

    return to_row == size - 1;
}


void apply_move(Position *pos,const Move *move) {
    Cell from = pos->cells[move->fr][move->fc];
    pos->cells[move->fr][move->fc] = CELL_EMPTY;
    pos->to_move = !pos->to_move;

    if (has_captured_checker(move)) {
        pos->cells[move->cr][move->cc] = CELL_EMPTY;
        pos->count[pos->to_move]--;
        pos->no_progress = 0;
    } else {
        pos->no_progress++;
    }
    pos->cells[move->tr][move->tc] = from;

    if (should_promote_to_king(pos->size, from, move->tr)){
        pos->cells[move->tr][move->tc] = cell_belongs_to_color(from, WHITE) ? CELL_WHITE_KING : CELL_BLACK_KING;
        pos->no_progress = 0;
    }
}

void print_board(Game *game) {
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

