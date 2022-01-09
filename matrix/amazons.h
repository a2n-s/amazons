#ifndef AMAZONS_H_INCLUDED
#define AMAZONS_H_INCLUDED

#include <stdbool.h>

#define WHITE +1
#define BLACK -1
#define  DRAW  0

#define EMPTY  0
#define ARROW  2

#define  WIDTH 10
#define HEIGHT 10

typedef struct Queen Queen;
struct Queen {
    int i;
    int j;
};

Queen white_queens[4];
Queen black_queens[4];

int abs(int a);
int distance(int a, int b, int c, int d);

void setup_board(int *board[], Queen *white, Queen *black);

bool is_queen_stuck(Queen q, int *board[]);

int play(int move, int player_color, int *board[], Queen *player_queens);

int reachable_places(Queen q, Queen *places, int *board[]);
void play_IA(int IA_color, int *board[], Queen *IA_queens);

bool is_board_full(int *board[]);

void display_board(int *board[]);

void show_tab(int *tab, int size);
void show_queens(Queen *white, Queen *black);

#endif // AMAZONS_H_INCLUDED
