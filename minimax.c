////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Author : Antoine Stevan*/
/*Date   : 09/06/2020*/
/*Description : this file contains the minimax algorithm*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "amazons.h"
     
int minimax(int color, Queen *white_queens, Queen *black_queens, int *board){
	// if node is terminal.
	char end = 1;
	for (int k = 0; k < NB_QUEENS; k++){
		if (!is_queen_stuck((color == WHITE)? white_queens[k] : black_queens[k], board)){ end = 0; break; }
	}
	if (end == 1){ return -color; }
	
	Queen moves[MAX_MOVES], arrws[MAX_MOVES];
	int moves_size, arrws_size;
	int best_value, v, prev_j, prev_i;
	
	if (color == WHITE){ // maximizing step
		best_value = INT_MIN;
		for (int q = 0; q < NB_QUEENS; q++){
			moves_size = reachable_places(white_queens[q], moves, board);
			for (int m = 0; m < moves_size; m++){
				// move
				board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
				prev_j = white_queens[q].j; prev_i = white_queens[q].i;
				white_queens[q].j = moves[m].j; white_queens[q].i = moves[m].i;
				board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; //
				arrws_size = reachable_places(white_queens[q], arrws, board);
				for (int ar = 0; ar < arrws_size; ar++){
					// shoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = ARROW; //
					v = minimax(-color, white_queens, black_queens, board); // recursive call
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					best_value = max(best_value, v);
				}
				// unmove
				board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
				white_queens[q].j = prev_j; white_queens[q].i = prev_i;
				board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; //
			}
		} 
	} else { // minimizing step
		best_value = INT_MAX;
		for (int q = 0; q < NB_QUEENS; q++){
			moves_size = reachable_places(black_queens[q], moves, board);
			for (int m = 0; m < moves_size; m++){
				// move
				board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
				prev_j = black_queens[q].j; prev_i = black_queens[q].i;
				black_queens[q].j = moves[m].j; black_queens[q].i = moves[m].i;
				board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; //
				arrws_size = reachable_places(black_queens[q], arrws, board);
				for (int ar = 0; ar < arrws_size; ar++){
					// shoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = ARROW; //
					v = minimax(-color, white_queens, black_queens, board); // recursive call
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					best_value = min(best_value, v);
				}
				// unmove
				board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
				black_queens[q].j = prev_j; black_queens[q].i = prev_i;
				board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; //
			}
		} 
	}
	return best_value;
}

int minimax_debug(int color, Queen *white_queens, Queen *black_queens, int *board, int depth){
	// same algorithm with debug messages
	display_board(board);
	printf("%s turn\n", (color == WHITE)? "white":"black");
	
	// if node is terminal. Make sense only if it is time to choose the queen
	
	char end = 1;
	for (int k = 0; k < NB_QUEENS; k++){
		if (!is_queen_stuck((color == WHITE)? white_queens[k] : black_queens[k], board)){ 
			end = 0; 
			break;
		}
	}
	if (end == 1){ 
		printf("\t\t\t\t\tVictory : %s\nReturning %2d\n", (color == WHITE)? "black":"white", -color);
		return -color; }
	
	int best_value = (color == WHITE)? INT_MIN : INT_MAX;
	Queen moves[MAX_MOVES], arrws[MAX_MOVES];
	int moves_size, arrws_size;
	int v, prev_j, prev_i;
	for (int q = 0; q < NB_QUEENS; q++){
		moves_size = reachable_places((color == WHITE)? white_queens[q] : black_queens[q], moves, board);
		for (int m = 0; m < moves_size; m++){
			// move
			if (color == WHITE){
				board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
				prev_j = white_queens[q].j; prev_i = white_queens[q].i;
				white_queens[q].j = moves[m].j; white_queens[q].i = moves[m].i;
				board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE;
			} else {
				board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
				prev_j = black_queens[q].j; prev_i = black_queens[q].i;
				black_queens[q].j = moves[m].j; black_queens[q].i = moves[m].i;
				board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK;
			} //
			arrws_size = reachable_places((color == WHITE)? white_queens[q] : black_queens[q], arrws, board);
			for (int ar = 0; ar < arrws_size; ar++){
				// shoot
				board[arrws[ar].i*WIDTH + arrws[ar].j] = ARROW; //
				printf("%s -> (%d %d) -> (%d %d) and ", (color == WHITE)?"white":"black", moves[m].j, moves[m].i, arrws[ar].j, arrws[ar].i);
				v = minimax_debug(-color, white_queens, black_queens, board, depth+1);
				// unshoot
				board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
				printf("%s %2d and %2d at depth %d\n", (color == WHITE)? "minimizing":"maximizing", best_value, v, depth);
				best_value = (color == WHITE)? max(best_value, v) : min(best_value, v);
			}
			// unmove
			if (color == WHITE){
				board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
				white_queens[q].j = prev_j; white_queens[q].i = prev_i;
				board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE;
			} else {
				board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
				black_queens[q].j = prev_j; black_queens[q].i = prev_i;
				board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK;
			}
		}
	} 
	printf("Returning best_value = %2d  at depth %d (%s)\n", best_value, depth, (color == WHITE)? "minimized":"maximized");
	return best_value;
}

/*int minimax(int color, Queen *white_queens, Queen *black_queens, int *board){
	// if node is terminal. Make sense only if it is time to choose the queen
	char end = 1;
	for (int k = 0; k < NB_QUEENS; k++){
		if (!is_queen_stuck((color == WHITE)? white_queens[k] : black_queens[k], board)){ end = 0; break; }
	}
	if (end == 1){ return -color; }
	
	int best_value = (color == WHITE)? INT_MIN : INT_MAX;
	Queen moves[MAX_MOVES], arrws[MAX_MOVES];
	int moves_size, arrws_size;
	int v, prev_j, prev_i;
	for (int q = 0; q < NB_QUEENS; q++){
		moves_size = reachable_places((color == WHITE)? white_queens[q] : black_queens[q], moves, board);
		for (int m = 0; m < moves_size; m++){
			// move
			if (color == WHITE){
				board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
				prev_j = white_queens[q].j; prev_i = white_queens[q].i;
				white_queens[q].j = moves[m].j; white_queens[q].i = moves[m].i;
				board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE;
			} else {
				board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
				prev_j = black_queens[q].j; prev_i = black_queens[q].i;
				black_queens[q].j = moves[m].j; black_queens[q].i = moves[m].i;
				board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK;
			} //
			arrws_size = reachable_places((color == WHITE)? white_queens[q] : black_queens[q], arrws, board);
			for (int ar = 0; ar < arrws_size; ar++){
				// shoot
				board[arrws[ar].i*WIDTH + arrws[ar].j] = ARROW; //
				v = minimax(-color, white_queens, black_queens, board);
				// unshoot
				board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
				best_value = (color == WHITE)? max(best_value, v) : min(best_value, v);
			}
			// unmove
			if (color == WHITE){
				board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
				white_queens[q].j = prev_j; white_queens[q].i = prev_i;
				board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE;
			} else {
				board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
				black_queens[q].j = prev_j; black_queens[q].i = prev_i;
				board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK;
			}
		}
	} 
	return best_value;
}*/
