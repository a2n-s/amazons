////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Author : Antoine Stevan*/
/*Date   : 09/06/2020*/
/*Description : this file is the main source of the project and contains all the function needed to play*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "amazons.h"

// this file is the core of the program

// first, include the needed libraries
// the iterative deepening overwrites the other algorithms for the structure is very different and incompatible
#ifdef ITER
	#ifdef ALPHA_BETA_ITER
	#include "alpha_beta_iter.c"
	#endif
	#ifdef ALPHA_BETA_ITER_HASH
	#include "alpha_beta_iter_hash.c"
	#endif
#else
	#ifdef MINIMAX
	#include "minimax.c"
	#endif
	#ifdef MINIMAX_DEBUG
	#include "minimax.c"
	#endif

	#ifdef ALPHA_BETA
	#include "alpha_beta.c"
	#endif
	#ifdef ALPHA_BETA_CUT
	#include "alpha_beta.c"
	#endif

	#ifdef ALPHA_BETA_HASH
	#include "alpha_beta_hash.c"
	#endif
#endif


int abs(int a){ return (a >= 0)? a : -a; }

int distance(int a, int b, int c, int d){ return abs(a - c) + abs(b - d); }

#ifdef HASH
void setup_board(uint64_t *sig, Queen *white, Queen *black, int *board){
#else
void setup_board(Queen *white, Queen *black, int *board){
#endif
	// i and j are here swapped for convenience when a human operator needs to modify the initial configurations of the board
	for (int k = 0; k < NB_QUEENS; k++){
		board[BLACK_INIT[k].j*WIDTH + BLACK_INIT[k].i] = BLACK;
	    black[k].j = BLACK_INIT[k].i;
	    black[k].i = BLACK_INIT[k].j;

	    board[WHITE_INIT[k].j*WIDTH + WHITE_INIT[k].i] = WHITE;
	    white[k].j = WHITE_INIT[k].i;
	    white[k].i = WHITE_INIT[k].j;
	    
	    #ifdef HASH
	    *sig = *sig ^ hash_white[white[k].j*WIDTH + white[k].i] ^ hash_black[black[k].j*WIDTH + black[k].i];
	    #endif
	}

	for (int k = 0; k < INIT_ARROWS; k++){
		board[ARROWS[k].j*WIDTH + ARROWS[k].i] = ARROW;
		#ifdef HASH
		*sig = *sig ^ hash_arrws[ARROWS[k].j*WIDTH + ARROWS[k].i];
		#endif
	}
}

bool is_queen_stuck(Queen q, int *board){
    int loc_j, loc_i;
	// check the 8 places around the queen
    for (int j = -1; j <=1; j++){
        for (int i = -1; i <= 1; i++){
            if (i != 0 || j != 0){
                loc_j = q.j + j;
                loc_i = q.i + i;
                if (loc_j >= 0 && loc_j < WIDTH && loc_i >= 0 && loc_i < HEIGHT){ // inside the board ?
                    if (board[loc_i*WIDTH + loc_j] == EMPTY){return false;}       // one empty space is enough not to be stuck
                }
            }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// To move the human player ///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int play(int move, int player_color, Queen *player_queens, int *board){
	// this is the old unused function //
    int moves[6]; // [previous_j, previous_i, next_j, next_i, arrow_j, arrow_i]
    for (int i = 5; i >= 0; i--){
        moves[i] = move%10;
        move /= 10;
    }

    if (board[moves[1]*WIDTH + moves[0]] != player_color){
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

    board[moves[1]*WIDTH + moves[0]] = EMPTY;
    board[moves[3]*WIDTH + moves[2]] = player_color;
    board[moves[5]*WIDTH + moves[4]] = ARROW;

    int loc;
    for (int i = 0; i < NB_QUEENS; i++){
        if (player_queens[i].i == moves[1] && player_queens[i].j == moves[0]) {loc = i;}
    }
    player_queens[loc].i = moves[3];
    player_queens[loc].j = moves[2];
    return 0;
}

void cut(int move, int *j, int *i){
	// used to cut the integer (from 0 to 99) given by the human player into the two coordinates (j, i) on the board
	*j = move / 10;
	*i = move % 10;
}

int choose_queen(Queen *q, int player_color, int *board){
	printf("Please choose one of your %s amazons : ", (player_color == WHITE)?"white":"black");
	int queen;
	scanf("%d", &queen);
	int j, i;
	cut(queen, &j, &i);
	
	Queen test = {i, j};
	if (is_queen_stuck(test, board)){ printf("Unfortunately, this amazon is stuck...\n"); return 1; }
	
	if (board[i*WIDTH+j] != player_color) {printf("This is not your color...\n"); return 1; }
	else { q->j = j; q->i = i; show_moves(*q, board); return 0; }
}
#ifdef HASH
int move_queen(Queen *q, uint64_t *sig, int player_color, Queen *player_queens, int *board){
#else
int move_queen(Queen *q, int player_color, Queen *player_queens, int *board){
#endif
	printf("Please move your amazon to a valid position : ");
	int move;
	scanf("%d", &move);
	int j, i;
	cut(move, &j, &i);
	
	Queen moves[MAX_MOVES];
	int moves_size = reachable_places(*q, moves, board);
	if (is_in(moves, j, i, moves_size)) {
		board[q->i*WIDTH + q->j] = EMPTY;
		// don't forget to update the queens array...
		int loc = 0;
		for (int k = 0; k < NB_QUEENS; k++){
			// finds the index of the queen
		    if (player_queens[k].i == q->i && player_queens[k].j == q->j) { loc = k; break; }
		}
		player_queens[loc].i = i;
		player_queens[loc].j = j;
		#ifdef HASH
		if (player_color == WHITE){ *sig ^= hash_white[q->i*WIDTH + q->j]; }
		else 					  { *sig ^= hash_black[q->i*WIDTH + q->j]; }
		#endif
		
		q->j = j; q->i = i;	
		board[q->i*WIDTH + q->j] = player_color;
		#ifdef HASH
		if (player_color == WHITE){ *sig ^= hash_white[q->i*WIDTH + q->j]; }
		else 					  { *sig ^= hash_black[q->i*WIDTH + q->j]; }
		#endif
		show_moves(*q, board); 
		
		#ifdef HASH
		if (player_color == WHITE){ *sig ^= hash_turn; }
		#endif
		return 0; 
	}
	else { printf("This is not a valid position...\n"); return 1; }
}
#ifdef HASH
int shoot_arrow(Queen *q, uint64_t *sig, int *board){
#else
int shoot_arrow(Queen *q, int *board){
#endif
	printf("Please shoot an arrow where your amazon wants to : ");
	int arrow;
	scanf("%d", &arrow);
	int j, i;
	cut(arrow, &j, &i);
	
	Queen arrows[MAX_MOVES];
	int arrows_size = reachable_places(*q, arrows, board);
	if (is_in(arrows, j, i, arrows_size)) {
		board[i*WIDTH + j] = ARROW;
		#ifdef HASH
		*sig ^= hash_arrws[i*WIDTH + j];
		#endif
		return 0;
	}
	else { printf("This place is not possible, amazons aren't magicians...\n"); return 1; }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// To move the AI player //////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int reachable_places(Queen q, Queen *places, int *board){
    int found = 0, di, dj;
	// start from all 8 neighbours if in the board
    for (int j = -1; j <= 1; j++){
        for (int i = -1; i <= 1; i++){
            if (i != 0 ||j != 0){
                dj = q.j + j;
                di = q.i + i;
                while (dj >= 0 && dj < WIDTH && di >= 0 && di < HEIGHT && board[di*WIDTH + dj] == EMPTY){
					// if, in one direction, the place is inside the board and free, it is reachable 
					// from the queen (because the previous ones are)
                    places[found].j = dj;
                    places[found].i = di;
                    found++;
					// we continue in this direction
                    dj += j;
                    di += i;
    }	}	}	} return found; }

#ifdef ITER
#ifdef HASH
uint64_t play_AI(int player_color, Queen *white_queens, Queen *black_queens, uint64_t sig, int base, int *board){
#else
void play_AI(int player_color, Queen *white_queens, Queen *black_queens, int base, int *board){
#endif
#else
#ifdef HASH
uint64_t play_AI(int player_color, Queen *white_queens, Queen *black_queens, uint64_t sig, int *board){
#else
void play_AI(int player_color, Queen *white_queens, Queen *black_queens, int *board){
#endif
#endif
	int q, best_move, result, mj, mi, aj, ai;
	clock_t time = clock();
	
	#ifndef ITER
	int m, a;
	Queen moves[MAX_MOVES], arrws[MAX_MOVES];
	int moves_size, arrws_size, prev_j, prev_i;
	int wins[NB_QUEENS*MAX_MOVES*MAX_MOVES] = {0};
	int loss[NB_QUEENS*MAX_MOVES*MAX_MOVES] = {0};
	int wins_count = 0, loss_count = 0;
	for (q = 0; q < NB_QUEENS; q++){
		moves_size = reachable_places((player_color == WHITE)? black_queens[q] : white_queens[q], moves, board);
		if (player_color == BLACK){ printf("QMA = (%d %d) -> \n", white_queens[q].j, white_queens[q].i); }
		else 					  { printf("QMA = (%d %d) -> \n", black_queens[q].j, black_queens[q].i); }
		for (m = 0; m < moves_size; m++){
			// move queen
			printf("\t\t(%d %d) -> \n", moves[m].j, moves[m].i);
			if (player_color == BLACK){
				board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
				prev_j = white_queens[q].j; prev_i = white_queens[q].i;
				white_queens[q].j = moves[m].j; white_queens[q].i = moves[m].i;
				board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; 
			} else {
				board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
				prev_j = black_queens[q].j; prev_i = black_queens[q].i;
				black_queens[q].j = moves[m].j; black_queens[q].i = moves[m].i;
				board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; 
			}//
			arrws_size = reachable_places((player_color == WHITE)? black_queens[q] : white_queens[q], arrws, board);
			for (a = 0; a < arrws_size; a++){
				printf("\t\t\t (%d %d) -> ", arrws[a].j, arrws[a].i);
				// shoot arrow
				board[arrws[a].i*WIDTH + arrws[a].j] = ARROW; //
				
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				#ifdef HASH
				uint64_t sig_t;
				if (player_color == WHITE){
					sig_t = sig^hash_black[    prev_i*WIDTH +     prev_j]
						       ^hash_black[moves[m].i*WIDTH + moves[m].j]
							   ^hash_arrws[arrws[a].i*WIDTH + arrws[a].j];
				} else {
					sig_t = sig^hash_white[    prev_i*WIDTH +     prev_j]
						       ^hash_white[moves[m].i*WIDTH + moves[m].j]
							   ^hash_arrws[arrws[a].i*WIDTH + arrws[a].j]
							   ^hash_turn;
				}
				#endif
				
				#ifndef COMP
				#ifdef MINIMAX
				result = minimax(player_color, white_queens, black_queens, board);
				#else
				#ifdef MINIMAX_DEBUG
				result = minimax_debug(player_color, white_queens, black_queens, board, 0);
				#else
				#ifdef ALPHA_BETA
				result = alpha_beta(INT_MIN, INT_MAX, player_color, white_queens, black_queens, board);
				#else
				#ifdef ALPHA_BETA_CUT
				result = alpha_beta_cut(INT_MIN, INT_MAX, player_color, white_queens, black_queens, ALPHA_BETA_CUT, board);
				#else
				#ifdef ALPHA_BETA_HASH
				result = alpha_beta_hash(INT_MIN, INT_MAX, player_color, white_queens, black_queens, 0, sig_t, board);
				#endif
				#endif
				#endif
				#endif
				#endif
				printf("%4s (%2d |%2d)\n", (result*player_color < 0)? "win":"lose", result, -player_color);
				
				#else
/*				result = minimax(player_color, white_queens, black_queens, board);*/
/*				int result2 = alpha_beta(INT_MIN, INT_MAX, player_color, white_queens, black_queens, board);*/
/*				#ifdef ALPHA_BETA_HASH*/
/*				int result3 = alpha_beta_hash(INT_MIN, INT_MAX, player_color, white_queens, black_queens, 0, sig_t, board);*/
/*				printf("%4s (%2d | %2d | %2d | %2d)\n", (result == player_color)? "lose":"win", result, result2, result3, -player_color);*/
/*				#else*/
/*				printf("%4s (%2d | %2d | %2d)\n", (result == player_color)? "lose":"win", result, result2, -player_color);*/
/*				#endif*/
				result = alpha_beta_hash(INT_MIN, INT_MAX, player_color, white_queens, black_queens, 0, sig_t, board);
				int result2 = alpha_beta(INT_MIN, INT_MAX, player_color, white_queens, black_queens, board);;
				printf("%4s (%2d | %2d | %2d)\n", (result == player_color)? "lose":"win", result, result2, -player_color);
				
				#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				if (result*player_color < 0){ 
					wins[wins_count] = encode(q, moves, m, arrws, a); 
					wins_count += 1; }
				else {
					loss[loss_count] = encode(q, moves, m, arrws, a); 
					loss_count += 1; }
				
				// unshoot arrow
				board[arrws[a].i*WIDTH + arrws[a].j] = EMPTY; //
			}
			// unmove queen
			if (player_color == BLACK){
				board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
				white_queens[q].j = prev_j; white_queens[q].i = prev_i;
				board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; 
			} else {
				board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
				black_queens[q].j = prev_j; black_queens[q].i = prev_i;
				board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; 
			}//
		}
	}
	time = clock() - time;
	
	// move selection
	best_move = (wins_count > 0)? wins[rand()%wins_count] : loss[rand()%loss_count];

	#else
	int maxdepth = base;
	int old_best_move = -1;
	clock_t otime = clock();
	double ftime;
	timer.it_value.tv_sec = SECONDS;
    timer.it_value.tv_usec= MICSECS;
    setitimer(ITIMER_REAL,&timer,NULL);
    do {
      maxdepth++;
      best_move = -1;
      get_out = FALSE;
      #ifdef ALPHA_BETA_ITER
      result = alpha_beta_iter(INT_MIN, INT_MAX, -player_color, white_queens, black_queens, base, maxdepth, base, &best_move, board); // the call is on (base, maxdepth, base), we are at base "base", we start from base in the tree and go to maxdepth
      #endif
      #ifdef ALPHA_BETA_ITER_HASH
      result = alpha_beta_iter_hash(INT_MIN, INT_MAX, -player_color, white_queens, black_queens, base, maxdepth, base, sig, &best_move, board);
      #endif
      time = clock() - otime;
      ftime = (double)time / (double)CLOCKS_PER_SEC;
      printf("best_move = %5d, result = %3d, base = %2d, maxdepth = %2d, time = %f\n", best_move, result, base, maxdepth, ftime);
      if (best_move == -1){ best_move = old_best_move; } else { old_best_move = best_move; }
      if (best_move == -1) {printf("error -1\n");exit(EXIT_ERROR);}
    } while ((!get_out) && (maxdepth < (SIZE-1)));
    #endif
    
	decode(best_move, &q, &mj, &mi, &aj, &ai);
	if (player_color == BLACK){
		printf("AI chose queen at (%d %d)", white_queens[q].j, white_queens[q].i);
		board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
		white_queens[q].j = mj; white_queens[q].i = mi;
		board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; 
		#ifdef HASH
		sig ^= hash_white[white_queens[q].i*WIDTH + white_queens[q].j] 
			 ^ hash_white[mi*WIDTH + mj] ^ hash_arrws[ai*WIDTH + aj] ^ hash_turn;
		#endif
	}
	else{
		printf("AI chose queen at (%d %d)", black_queens[q].j, black_queens[q].i);
		board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
		black_queens[q].j = mj; black_queens[q].i = mi;
		board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; 
		#ifdef HASH
		sig ^= hash_black[black_queens[q].i*WIDTH + black_queens[q].j] 
			 ^ hash_black[mi*WIDTH + mj] ^ hash_arrws[ai*WIDTH + aj];
		#endif
	}
	printf(", moved her to (%d %d) ", mj, mi);
	printf("and shot her arrow towards (%d %d) in %2fs.\n\n", aj, ai, ((double)time/(double)CLOCKS_PER_SEC));
	board[ai*WIDTH + aj] = ARROW;
	
	#ifdef HASH
	return sig;
	#endif
}

void play_at_random(int color, Queen *queens, int *board){
	Queen places[MAX_MOVES]; // have to be big enough to avoid overflow
    int k, q;
    
	do{
		q = rand()%NB_QUEENS;
	}while(is_queen_stuck(queens[q], board));
    
    Queen start = {queens[q].i, queens[q].j};
    printf("Random agent choose queen at (%d %d)", start.j, start.i);
    k = reachable_places(start, places, board);
    Queen end = places[rand()%k];
    printf(", moved her to (%d %d) ", end.j, end.i);

    board[start.i*WIDTH + start.j] = EMPTY;
    board[end.i*WIDTH + end.j] = color;

    queens[q].j = end.j;
    queens[q].i = end.i;

    k = reachable_places(end, places, board);
    end = places[rand()%k];
    printf("and shot her arrow towards (%d %d).\n\n", end.j, end.i);
    board[end.i*WIDTH + end.j] = ARROW;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// To show the board + utils //////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_board_full(int *board){
    int nb_empty = 0;
    int i = 0;
    while (nb_empty == 0 && i < SIZE){
        if (board[i] == EMPTY){nb_empty++;}
    }
    return (nb_empty == 0);
}

void display_board(int *board){
    printf("   ");
    for (int j = 0; j < WIDTH; j++){
        printf("%d ", j);
    }
    printf("\n");

    for(int i = 0; i < HEIGHT; i++){
        printf("%d  ", i);
        for(int j = 0; j < WIDTH; j++){
            switch (board[i*WIDTH+j]){
                case EMPTY:
		            printf(". ");
                    break;
                case WHITE:
                	printf_utf_8(0x265b);
                	printf(" ");
                    break;
                case BLACK:
                	printf_utf_8(0x2655);
                	printf(" ");
                    break;
                case ARROW:
                	printf_utf_8(0x25c9);
                	printf(" ");
                    break;
                default:
                    printf("  ");
                    break;
            }
        }
        printf("\n");
    }
}

void show_moves(Queen queen, int *board){
	printf("   ");
    for (int j = 0; j < WIDTH; j++){
        printf("%d ", j);
    }
    printf("\n");

	// compute the reachable places from the choosen queen
	Queen places[MAX_MOVES];
	int places_size = reachable_places(queen, places, board);

    for(int i = 0; i < HEIGHT; i++){
        printf("%d  ", i);
        for(int j = 0; j < WIDTH; j++){
			if (is_in(places, j, i, places_size)){ printf_utf_8(0x2297/*0x2388*/); printf(" "); }
			else {
		        switch (board[i*WIDTH+j]){
		            case EMPTY:
		                printf(". ");
		                break;
		            case WHITE:
		            	printf_utf_8(0x265b);
                		printf(" ");
		                break;
		            case BLACK:
		            	printf_utf_8(0x2655);
                		printf(" ");
		                break;
		            case ARROW:
		                printf_utf_8(0x25c9);
                		printf(" ");
		                break;
		            default:
		                printf("  ");
		                break;
		        }
			}
        }
        printf("\n");
    }
}

void printf_utf_8(int code){
	if      (code < 0x0080){  printf("%c", code); }
	else if (code < 0x0800){  printf("%c%c", 0xc0 + (code>>6), 0x80 + (code&0x3f)); }
	else if (code < 0x10000){ printf("%c%c%c", 0xe0 + (code>>12), 
											   0x80 + ((code>>6)&0x3f),
											   0x80 + (code&0x3f)); }
	else { printf("%c%c%c%c", 0xf0 + (code>>18),
							  0x80 + ((code>>12)&0x3f),
							  0x80 + ((code>>6)&0x3f),
							  0x80 + (code&0x3f)); }
}

bool is_in(Queen *array, int j, int i, int size){
	// says if array contains a queen at position (j, i)
	for (int k = 0; k < size; k++){
		if (array[k].j == j && array[k].i == i){ return true; }
	}
	return false;
}

void mat_cpy(Queen *tab1, Queen *tab2, int n){
	// copies the elements of tab1 in tab2
	for(int k = 0; k < n; k++){
		tab2[k] = tab1[k];
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
    for (int i = 0; i < NB_QUEENS; i++){
        printf("(%d %d) ", white[i].j, white[i].i);
    }
    printf("\nblack : ");
    for (int i = 0; i < NB_QUEENS; i++){
        printf("(%d %d) ", black[i].j, black[i].i);
    }
    printf("\n");
}
