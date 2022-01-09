////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Author : Antoine Stevan*/
/*Date   : 09/06/2020*/
/*Description : this file is the main and contains the game loops and the executable code*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include "amazons.c"

int main(int argc, char *argv[]){
	// we need the interruptions only when using iterative deepening
	#ifdef ITER
	signal(SIGALRM, handler);
	#endif
	
	//initializing the random generator
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

	// the signature is only used when the hash table is needed
	#ifdef HASH
	uint64_t sig = 0;
	printf("Initialization of hash table...");
	hash_turn = init_hash();
	printf(" done\n");
    setup_board(&sig, white_queens, black_queens, board);
    printf("hash_turn -> %16lx\n", hash_turn);
    for (int k = 0; k < SIZE; k++){
    	printf("%16lx\t-\t%16lx\t-\t%16lx\n", hash_white[k], hash_black[k], hash_arrws[k]);
    }
    printf("start_game_signature -> %16lx\n\n", sig);
    #else
    setup_board(white_queens, black_queens, board);
    #endif

    ///////////////
    // game loop //
    ///////////////
    #ifndef RANDOM_HUMAN // this is a feature that allows the AI to play against a random agent, the followings variables are need only with a real human
    int queen, move, arrow;
    Queen q;
    #endif
    int turn = WHITE; // the white begin
    bool game = true;
    bool white_stuck, black_stuck;
    #ifdef ITER
    int base = (player_color == WHITE)? 1 : 0;
    #endif
    while (game){ // game loop
        display_board(board);
        if (turn == player_color){
            /////////////////
            // player turn //
            /////////////////
            
            #ifdef RANDOM_HUMAN
            play_at_random(player_color, (player_color == WHITE)? white_queens:black_queens, board);
            #else
            queen = 1;
            while (queen == 1){
            	queen = choose_queen(&q, player_color, board);
            }
            move = 1;
            while (move == 1){
            	#ifdef HASH
            	move = move_queen(&q, &sig, player_color, (player_color == WHITE)? white_queens : black_queens, board);
            	#else
            	move = move_queen(&q, player_color, (player_color == WHITE)? white_queens : black_queens, board);
            	#endif
            }
            arrow = 1;
            while (arrow == 1){
           		#ifdef HASH
            	arrow = shoot_arrow(&q, &sig, board);
            	#else
            	arrow = shoot_arrow(&q, board);
            	#endif
            }
            #endif
	
        } else {
            /////////////
            // IA turn //
            /////////////
            printf("\nAI is thinking...\n");
            #ifdef ITER
            #ifdef HASH
            sig = play_AI(player_color, white_queens, black_queens, sig, base, board);
            #else
            play_AI(player_color, white_queens, black_queens, base, board);
            #endif
            base += 2;
            #else
            #ifdef HASH
            sig = play_AI(player_color, white_queens, black_queens, sig, board);
            #else
            play_AI(player_color, white_queens, black_queens, board);
            #endif
            #endif
        }
        turn *= -1;

        /////////////////
        // end of game //
        /////////////////
        white_stuck = true;
        black_stuck = true;
        for (int i = 0; i < NB_QUEENS; i++){
            white_stuck &= is_queen_stuck(white_queens[i], board);
            black_stuck &= is_queen_stuck(black_queens[i], board);

        }
        // the game ends when the next player is stuck
        game = !(turn == BLACK && black_stuck) && !(turn == WHITE && white_stuck);
    }

    /////////////
    // results //
    /////////////
    display_board(board);
    if      (white_stuck && turn == WHITE){printf( "Blacks (%s) win !", (player_color == BLACK)?"player":"AI");}
    else if (black_stuck && turn == BLACK){printf( "Whites (%s) win !", (player_color == WHITE)?"player":"AI");}
	else   								  {printf("......");}
    return 0;
}


