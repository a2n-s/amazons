////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Author : Antoine Stevan*/
/*Date   : 09/06/2020*/
/*Description : this file contains the alpha beta algorithm*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "amazons.h"
#include "evaluation.c"
     
int alpha_beta(int alpha, int beta, int color, Queen *white_queens, Queen *black_queens, int *board){
	// if node is terminal.
	char end = 1;
	for (int k = 0; k < NB_QUEENS; k++){
		if (!is_queen_stuck((color == WHITE)? white_queens[k] : black_queens[k], board)){ end = 0; break; }
	}
	if (end == 1){ return -color; }
	
	int a = alpha, b = beta;
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
					v = alpha_beta(a, beta, -color, white_queens, black_queens, board); // recursive call
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					best_value = max(best_value, v);
					a = max(a, best_value);
					if (best_value >= beta){ // beta cut, undo the move completely and go back in the tree (return)
						// unmove
						board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
						white_queens[q].j = prev_j; white_queens[q].i = prev_i;
						board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; //
						return best_value; 
					}
				}
				// unmove
				board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
				white_queens[q].j = prev_j; white_queens[q].i = prev_i;
				board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; //
			}
		} 
	} else { // minimizing  step
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
					v = alpha_beta(alpha, b, -color, white_queens, black_queens, board); // recursive call
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					best_value = min(best_value, v);
					b = min(b, best_value);
					if (best_value <= alpha){ // alpha cut, undo the move completely and go back in the tree (return)
						// unmove
						board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
						black_queens[q].j = prev_j; black_queens[q].i = prev_i;
						board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; //
						return best_value; 
					}
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

int alpha_beta_cut(int alpha, int beta, int color, Queen *white_queens, Queen *black_queens, int depth, int *board){
	// this is the exact same algorithm as before
	// except the comparison involving depth at line 98 and the decrease of depth at lines 121 and 154
	
	// if node is terminal.
	char end = 1;
	for (int k = 0; k < NB_QUEENS; k++){
		if (!is_queen_stuck((color == WHITE)? white_queens[k] : black_queens[k], board)){ end = 0; break; }
	}
	if (end == 1){ return -color; }
	
	if (depth == 0){ // stop the exploration and simply return the evaluation of the board
		return evaluation(color, white_queens, black_queens, board); // depends on the evaluation method
	}
	
	int a = alpha, b = beta;
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
					v = alpha_beta_cut(a, beta, -color, white_queens, black_queens, depth-1, board); // recursive call
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					best_value = max(best_value, v);
					a = max(a, best_value);
					if (best_value >= beta){ // beta cut, undo the move completely and go back in the tree (return)
						// unmove
						board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
						white_queens[q].j = prev_j; white_queens[q].i = prev_i;
						board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; //
						return best_value; 
					}
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
					v = alpha_beta_cut(alpha, b, -color, white_queens, black_queens, depth-1, board); // recursive call
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					best_value = min(best_value, v);
					b = min(b, best_value);
					if (best_value <= alpha){ // alpha cut, undo the move completely and go back in the tree (return)
						// unmove
						board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
						black_queens[q].j = prev_j; black_queens[q].i = prev_i;
						board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; //
						return best_value; 
					}
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

