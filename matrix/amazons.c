#include <stdio.h>
#include <stdlib.h>
#include "amazons.h"

int abs(int a){
    return (a >= 0)? a : -a;
}

int distance(int a, int b, int c, int d){
    return abs(a - c) + abs(b - d);
}

void setup_board(int *board[], Queen *white, Queen *black){
    int i, j;
    for (j = 0; j < WIDTH; j+=3){
        if (j == 3 || j == 6) {i = 0;}
        else                  {i = 3;}
	printf("%d %d -> %d\n", j, i, board[i][j]);
        board[i][j] = BLACK;
	printf("bien\n");
        black[j/3].j = j;
        black[j/3].i = i;

        board[HEIGHT - 1 - i][j] = WHITE;
        white[j/3].j = j;
        white[j/3].i = HEIGHT - 1 - i;
    }

    /*board[(2)*WIDTH + (0)] = ARROW;
    board[(2)*WIDTH + (1)] = ARROW;
    board[(3)*WIDTH + (1)] = ARROW;
    board[(4)*WIDTH + (1)] = ARROW;
    board[(4)*WIDTH + (0)] = ARROW;

    board[(2)*WIDTH + (9)] = ARROW;
    board[(2)*WIDTH + (8)] = ARROW;
    board[(3)*WIDTH + (8)] = ARROW;
    board[(4)*WIDTH + (8)] = ARROW;
    board[(4)*WIDTH + (9)] = ARROW;

    board[(0)*WIDTH + (2)] = ARROW;
    board[(1)*WIDTH + (2)] = ARROW;
    board[(1)*WIDTH + (3)] = ARROW;
    board[(1)*WIDTH + (4)] = ARROW;
    board[(0)*WIDTH + (4)] = ARROW;

    board[(0)*WIDTH + (5)] = ARROW;
    board[(1)*WIDTH + (6)] = ARROW;
    board[(1)*WIDTH + (7)] = ARROW;
    board[(0)*WIDTH + (7)] = ARROW;*/
}

bool is_queen_stuck(Queen q, int *board[]){
    int loc_j, loc_i;
    bool stuck = true;
    for (int j = -1; j <=1; j++){
        for (int i = -1; i <= 1; i++){
            if (i != 0 || j != 0){
                loc_j = q.j + j;
                loc_i = q.i + i;
                if (loc_j >= 0 && loc_j < WIDTH && loc_i >= 0 && loc_i < HEIGHT){
                    if (board[loc_i][loc_j] == EMPTY){stuck = false;}
                }
            }
        }
    }
    return stuck;
}

int play(int move, int player_color, int *board[], Queen *player_queens){
    int moves[6]; // [prev_j, prev_i, new_j, new_i, arrow_j, arrow_i]
    for (int i = 5; i >= 0; i--){
        moves[i] = move%10;
        move /= 10;
    }

    if (board[moves[1]][moves[0]] != player_color){
        printf("Illegal move type I (wrong piece)");
        return 1;
    }

    int di_move = moves[3] - moves[1];
    int dj_move = moves[2] - moves[0];
    if ((di_move != 0 && dj_move != 0 && abs(di_move/dj_move) != 1) || (di_move == 0 && dj_move == 0)){
        printf("Illegal move type II (wrong move)");
        return 1;
    }

    di_move = moves[5] - moves[3];
    dj_move = moves[4] - moves[2];
    if (di_move != 0 && dj_move != 0 && abs(di_move/dj_move) != 1){
        printf("Illegal move type III (wrong arrow)");
        return 1;
    }

    board[moves[1]][moves[0]] = EMPTY;
    board[moves[3]][moves[2]] = player_color;
    board[moves[5]][moves[4]] = ARROW;

    int loc;
    for (int i = 0; i < 4; i++){
        if (player_queens[i].i == moves[1] && player_queens[i].j == moves[0]) {loc = i;}
    }
    player_queens[loc].i = moves[3];
    player_queens[loc].j = moves[2];
    return 0;
}

int reachable_places(Queen q, Queen *places, int *board[]){

    int found = 0;
    for (int j = -1; j <= 1; j++){
        for (int i = -1; i <= 1; i++){
            if (i != 0 ||j != 0){
                int dj, di;
                dj = q.j + j;
                di = q.i + i;
                while (dj >= 0 && dj < WIDTH && di >= 0 && di < HEIGHT && board[di][dj] == EMPTY){
                    places[found].j = dj;
                    places[found].i = di;
                    found++;

                    dj += j;
                    di += i;
                }
            }
        }
    }
    return found;
}

void play_IA(int IA_color, int *board[], Queen *IA_queens){
    Queen places[35];
    int k, q;

    q = rand()%4;
    Queen start = {IA_queens[q].i, IA_queens[q].j};
    k = reachable_places(start, places, board);
    Queen end = places[rand()%k];

    board[start.i][start.j] = EMPTY;
    board[end.i][end.j] = IA_color;

    IA_queens[q].j = end.j;
    IA_queens[q].i = end.i;

    k = reachable_places(end, places, board);
    end = places[rand()%k];
    board[end.i][end.j] = ARROW;
}

bool is_board_full(int *board[]){
    int nb_empty = 0;
    int i = 0;
    while (nb_empty == 0 && i < WIDTH*HEIGHT){
        if (board[i] == EMPTY){nb_empty++;}
    }
    return (nb_empty == 0);
}

void display_board(int *board[]){
    printf("   ");
    for (int j = 0; j < WIDTH; j++){
        printf("%d ", j);
    }
    printf("\n");

    for(int i = 0; i < HEIGHT; i++){
        printf("%d  ", i);
        for(int j = 0; j < WIDTH; j++){
            switch (board[i][j]){
                case EMPTY:
                    printf(". ");
                    break;
                case WHITE:
                    printf("W ");
                    break;
                case BLACK:
                    printf("B ");
                    break;
                case ARROW:
                    printf("# ");
                    break;
                default:
                    printf("  ");
                    break;
            }
            //printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
}


void show_tab(int *tab, int size){
    printf("[%d", tab[0]);
    for (int i = 1; i < size; i++){
        printf(", %d", tab[i]);
    }
    printf("]");
}

void show_queens(Queen *white, Queen *black){
    printf("white : ");
    for (int i = 0; i < 4; i++){
        printf("(%d %d) ", white[i].j, white[i].i);
    }
    printf("\nblack : ");
    for (int i = 0; i < 4; i++){
        printf("(%d %d) ", black[i].j, black[i].i);
    }
    printf("\n");
}
