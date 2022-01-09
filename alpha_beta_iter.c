////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Author : Antoine Stevan*/
/*Date   : 09/06/2020*/
/*Description : this file contains the alpha beta algorithm with iterative deepening*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "amazons.h"
#include "evaluation.c"
     
int alpha_beta_iter(int alpha, int beta, int color, Queen *white_queens, Queen *black_queens, int depth, int maxdepth, int base, int *best_move, int *board){
	// if node is terminal.
	char end = 1;
	for (int k = 0; k < NB_QUEENS; k++){
		if (!is_queen_stuck((color == WHITE)? white_queens[k] : black_queens[k], board)){ end = 0; break; }
	}
	if (end == 1){ return -color; }
	
	// we are where the current iteration needs to stop
	if (depth == maxdepth){ return evaluation(color, white_queens, black_queens, board); }
	
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
					v = alpha_beta_iter(a, beta, -color, white_queens, black_queens, depth+1, maxdepth, base, best_move, board);
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					if (get_out) { //this is the interruption, undo move completely and get out
						// unmove
						board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
						white_queens[q].j = prev_j; white_queens[q].i = prev_i;
						board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; //
						return GET_OUT; }
					if (v > best_value && base == depth){ *best_move = encode(q, moves, m, arrws, ar); } // best move found
					best_value = max(best_value, v);
					a = max(a, best_value);
					if (best_value >= beta){ // beta cut
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
					v = alpha_beta_iter(alpha, b, -color, white_queens, black_queens, depth+1, maxdepth, base, best_move, board);
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					if (get_out) { //this is the interruption, undo move completely and get out
						// unmove
						board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
						black_queens[q].j = prev_j; black_queens[q].i = prev_i;
						board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; //
						return GET_OUT; }
					if (v < best_value && base == depth){ *best_move = encode(q, moves, m, arrws, ar); } // best move found
					best_value = min(best_value, v);
					b = min(b, best_value);
					if (best_value <= alpha){ // alpha cut
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
