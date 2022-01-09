#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "amazons.c"

int board[HEIGHT][WIDTH] = {{0}};

int main(int argc, char *argv[]){
    srand(time(NULL));

    printf("***********************\n");
    printf("  The Game Of Amazons  \n");
    printf("***********************\n");

    //////////////////
    // color choice //
    //////////////////
    char color = 0;
    int player_color;
    while (color == 0){
        printf("\nChoose your color (b/w) : ");
        scanf("%c", &color);
        if (color == 'w' || color == 'W'){
            printf("You are white !\n\n");
            player_color = WHITE;
        } else if (color == 'b' || color == 'B'){
            printf("You are black !\n\n");
            player_color = BLACK;
        } else {color = 0;}
    }
		
	printf("setup\n");
    setup_board(board, white_queens, black_queens);

    ///////////////
    // game loop //
    ///////////////
    int move;
    int turn = WHITE;
    bool game = true;
    bool white_stuck, black_stuck;
    while (game){
        /*char junk;
        scanf("%c", &junk);*/
        if (turn == player_color){
            /////////////////
            // player turn //
            /////////////////
            display_board(board);
            move = 1;
            while (move == 1){
                printf("player turn ");
                scanf("%d", &move);
                move = play(move, player_color, board, (player_color == WHITE)? white_queens : black_queens);
                printf("\n");
            }
        } else {
            /////////////
            // IA turn //
            /////////////
            printf("IA is thinking...\n\n");
            play_IA(player_color * -1, board, (player_color == WHITE)? black_queens : white_queens);
        }
        turn *= -1;

        /////////////////
        // end of game //
        /////////////////
        white_stuck = true;
        black_stuck = true;
        for (int i = 0; i < 4; i++){
            white_stuck &= is_queen_stuck(white_queens[i], board);
            black_stuck &= is_queen_stuck(black_queens[i], board);

        }
        game = !(turn == BLACK && black_stuck) && !(turn == WHITE && white_stuck);
    }

    /////////////
    // results //
    /////////////
    if (white_stuck){printf("Les noirs gagnent !");}
    else if (black_stuck){printf("Les blancs gagnent !");}
    return 0;
}


