////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Author : Antoine Stevan*/
/*Date   : 09/06/2020*/
/*Description : this file contains the alpha beta algorithm with hash table and iterative deepening*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "alpha_beta_hash.h"
#include "evaluation.c"

char retrieve_v_iter_hash(uint64_t sig, int dist, int *v_inf, int *v_sup, int *v_bmove){
	int ind = sig & HASH_MASK;
	if (hash_table[ind].sig == sig){
		if (hash_table[ind].v_d == dist || 
		   ((hash_table[ind].v_inf == hash_table[ind].v_sup)&&abs(hash_table[ind].v_inf)>=BWIN)){
			*v_inf   = hash_table[ind].v_inf;
			*v_sup   = hash_table[ind].v_sup;
			*v_bmove = hash_table[ind].v_bmove;
			return TRUE;
		}
		*v_bmove = hash_table[ind].v_bmove;
		return FALSE;
	}
	return FALSE;
}

void store_v_iter_hash_both(uint64_t sig, int v, int dist, int base, int move){
	int ind = sig & HASH_MASK;
	if (hash_table[ind].v_base != base || hash_table[ind].v_d <= dist){
		hash_table[ind].v_inf = (int8_t) v;
		hash_table[ind].v_sup = (int8_t) v;
		hash_table[ind].v_d = (uint8_t) dist;
		hash_table[ind].v_bmove= (uint8_t) move;
		hash_table[ind].v_base = (uint8_t) base;
		hash_table[ind].sig = sig;
	}
}

void store_v_iter_hash(uint64_t sig, int alpha, int beta, int g, int dist, int base, int move){
	int ind = sig & HASH_MASK;
	if (hash_table[ind].v_d <= dist || hash_table[ind].v_base != base){
		if (hash_table[ind].sig != sig || hash_table[ind].v_d != dist){
			hash_table[ind].v_d = (uint8_t) dist;
			hash_table[ind].v_inf = -MAXV;
			hash_table[ind].v_sup = +MAXV;
			hash_table[ind].sig = sig;
		}
		hash_table[ind].v_base = (uint8_t)base;
		if (move != -1){ hash_table[ind].v_bmove = (uint8_t)move; }
		if (g <= alpha){ hash_table[ind].v_sup = (int16_t)g; }
		else if (g >=  beta){ hash_table[ind].v_inf = (int16_t)g; }
		else if (g >  alpha && g < beta) { hash_table[ind].v_inf = (int16_t)g; hash_table[ind].v_sup = (int16_t)g; }
	}
}

int alpha_beta_iter_hash(int alpha, int beta, int color, Queen *white_queens, Queen *black_queens, int depth, int maxdepth, int base, uint64_t sig, int *best_move, int *board){
	int v_inf, v_sup, bmove = -1, lmove = -1;
	// if node in the table
	if (retrieve_v_iter_hash(sig, maxdepth-depth, &v_inf, &v_sup, &bmove)){
		if (depth == base) { *best_move = bmove; }
		if (v_inf == v_sup) { return v_inf; }
		if (v_inf >= beta)  { return v_inf; }
		if (v_sup <= alpha) { return v_sup; }
		alpha = max(alpha, v_inf);
		beta  = min( beta, v_sup);
	} 
		
	// if node is terminal.
	char end = 1;
	for (int k = 0; k < NB_QUEENS; k++){
		if (!is_queen_stuck((color == WHITE)? white_queens[k] : black_queens[k], board)){ end = 0; break; }
	}
	if (end == 1){ store_v_iter_hash_both(sig, -color, maxdepth-depth, base, -1); return -color; }

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
					v = alpha_beta_iter_hash(a, beta, -color, white_queens, black_queens, depth+1, maxdepth, base, 
									    sig^hash_white[     prev_i*WIDTH +      prev_j]
									       ^hash_white[ moves[m].i*WIDTH +  moves[m].j]
									       ^hash_arrws[arrws[ar].i*WIDTH + arrws[ar].j]
									       ^hash_turn, 
									    best_move, board);
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					if (get_out) { // interruption, needs to get out
						// unmove
						board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
						white_queens[q].j = prev_j; white_queens[q].i = prev_i;
						board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; //
						return GET_OUT; }
					if (v > best_value){ 
						lmove = encode(q, moves, m, arrws, ar);
						if (base == depth){ *best_move = encode(q, moves, m, arrws, ar); }
					}
					best_value = max(best_value, v);
					a = max(a, best_value);
					if (best_value >= beta){ // beta cut
						// unmove
						board[white_queens[q].i*WIDTH + white_queens[q].j] = EMPTY;
						white_queens[q].j = prev_j; white_queens[q].i = prev_i;
						board[white_queens[q].i*WIDTH + white_queens[q].j] = WHITE; //
						store_v_iter_hash(sig, alpha, beta, best_value, maxdepth - depth, base, lmove);
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
					v = alpha_beta_iter_hash(alpha, b, -color, white_queens, black_queens, depth+1, maxdepth, base,
									    sig^hash_black[     prev_i*WIDTH +      prev_j]
									       ^hash_black[ moves[m].i*WIDTH +  moves[m].j]
									       ^hash_arrws[arrws[ar].i*WIDTH + arrws[ar].j],
									    best_move, board);
					// unshoot
					board[arrws[ar].i*WIDTH + arrws[ar].j] = EMPTY; //
					if (get_out) { // interruption, needs to get out
						// unmove
						board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
						black_queens[q].j = prev_j; black_queens[q].i = prev_i;
						board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; //
						return GET_OUT; }
					if (v < best_value && base == depth){ *best_move = encode(q, moves, m, arrws, ar); }
					best_value = min(best_value, v);
					b = min(b, best_value);
					if (best_value <= alpha){
						// unmove
						board[black_queens[q].i*WIDTH + black_queens[q].j] = EMPTY;
						black_queens[q].j = prev_j; black_queens[q].i = prev_i;
						board[black_queens[q].i*WIDTH + black_queens[q].j] = BLACK; //
						store_v_iter_hash(sig, alpha, beta, best_value, maxdepth - depth, base, lmove);
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
	store_v_iter_hash(sig, alpha, beta, best_value, maxdepth - depth, base, lmove);
	return best_value;
}
