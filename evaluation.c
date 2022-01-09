////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Author : Antoine Stevan*/
/*Date   : 09/06/2020*/
/*Description : this file contains the evaluation function*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "amazons.h"

typedef struct Explorer Explorer;
struct Explorer{
	int queen_dist, king_dist;
	char seen;
};

void reset_distances(Explorer *distances);
void combine(Explorer *sub, Explorer *main);
int explore_queen_moves(Queen q, int start, Queen *places, Explorer *distances, int distance, int *board);
int explore_king_moves(Queen q, int start, Queen *places, Explorer *distances, int distance, int *board);
void evaluate_amazons(Queen *q, Explorer *distances, int *board);
void show_map_queens(Explorer *distances);
void show_map_kings(Explorer *distances);
int evaluation(int turn, Queen *white_queens, Queen *black_queens, int *board);

int delta(int n, int m);
double pow_inv(int a, int b);
double w_v(Explorer *white, Explorer *black);
double f1(double w);
double f4(double w);

// reset all the cells in the matrix
void reset_distances(Explorer *distances){
	for (int k = 0; k < SIZE; k++){
		distances[k].queen_dist = INT_MAX;
		distances[k].king_dist = INT_MAX;
		distances[k].seen = FALSE;
	}
}

// combines the distances of main and sub, stores it in main
void combine(Explorer *sub, Explorer *main){
	for (int k = 0; k < SIZE; k++){
		main[k].queen_dist = min(main[k].queen_dist, sub[k].queen_dist);
		main[k].king_dist = min(main[k].king_dist, sub[k].king_dist);
	}
}

// recursive exploration of the board and marking distances
int explore_queen_moves(Queen q, int start, Queen *places, Explorer *distances, int distance, int *board){
    int found = 0, di, dj, index;
    for (int j = -1; j <= 1; j++){
        for (int i = -1; i <= 1; i++){
            if (i != 0 ||j != 0){
                dj = q.j + j;
                di = q.i + i;
                index = di*WIDTH + dj;
                while (dj >= 0 && dj < WIDTH && di >= 0 && di < HEIGHT && board[index] == EMPTY){
                	if (distances[index].seen == FALSE){
                		distances[index].seen = TRUE;
                		distances[index].queen_dist = distance;
                		places[start+found].j = dj;
                		places[start+found].i = di;
                		found++;
                	}
                    dj += j;
                    di += i;
                    index = di*WIDTH + dj;
    }	}	}	} 
/*    for (k = 0; k < found; k++){*/
/*    	explore_queen_moves(places[k], distances, distance+1, board);*/
/*    }*/
/*	show_map_queens(distances);*/
	return found;
}
int explore_king_moves(Queen q, int start, Queen *places, Explorer *distances, int distance, int *board){
    int found = 0, di, dj, index;
    for (int j = -1; j <= 1; j++){
        for (int i = -1; i <= 1; i++){
            if (i != 0 ||j != 0){
                dj = q.j + j;
                di = q.i + i;
                index = di*WIDTH + dj;
                if (dj >= 0 && dj < WIDTH && di >= 0 && di < HEIGHT && board[index] == EMPTY && distances[index].seen == FALSE){
            		distances[index].seen = TRUE;
            		distances[index].king_dist = distance;
            		places[start+found].j = dj;
            		places[start+found].i = di;
            		found++;
    }	}	}	} 
	return found;
}
  
// evalution of the distances for an array of queens (white or black queens for instance)
void evaluate_amazons(Queen *q, Explorer *distances, int *board){
	Explorer dists[SIZE];
	Queen to_explore_queen[SIZE], to_explore_king[SIZE]; // these are te queues
	int start, end, dist, k, found;
	// we start from the queens of the given player
	for (k = 0; k < NB_QUEENS; k++){
		to_explore_queen[k].j = q[k].j;
		to_explore_queen[k].i = q[k].i;
		to_explore_king[k].j  = q[k].j;
		to_explore_king[k].i  = q[k].i;
	}
	
	// the exploration of the graph of the game is breadth-first-search-like
	// it is straight-forward to show that the size of the queue is always less or equal than the SIZE of the board
	start = 0; end = NB_QUEENS-1; // we start with the queens
	dist = 1;
	reset_distances(dists);
	while(start <= end){ // if start > end, the queue is empty, i.e. there is no empty square to explore anymore
		found = 0;
		for (k = start; k <= end; k++){
			found += explore_queen_moves(to_explore_queen[k], end+found+1, to_explore_queen, dists, dist, board);
			// we count the number of places found and add these places after the last element in the queue, i.e. at end+found+1
		}
		dist++;
		// update of the queue
		start = end+1;
		end = start+found-1;
	}
	combine(dists, distances);
	
	// exact same principles with the king moves
	start = 0; end = NB_QUEENS-1;
	dist = 1;
	reset_distances(dists);
	while(start <= end){
		found = 0;
		for (k = start; k <= end; k++){
			found += explore_king_moves(to_explore_king[k], end+found+1, to_explore_king, dists, dist, board);
		}
		dist++;
		start = end+1;
		end = start+found-1;
	}
	combine(dists, distances);
}

int delta(int n, int m){
	if  (n < m){  return 1; }
	if  (n > m){ return -1; }
	return 0;
}

double w_v(Explorer *white, Explorer *black){
	double v = 0.;
	for (int k = 0; k < SIZE; k++){
		if ((board[k] == EMPTY) && (white[k].queen_dist < INT_MAX) && (black[k].queen_dist < INT_MAX)){
			v += pow_inv(2, abs(white[k].queen_dist - black[k].queen_dist));
		}
	}
	return v;
}

double pow_inv(int a, int b){
	double v = 1.;
	for(int k = 0; k < b; k++){ v /= (double)a; }
	return v;
}

double f1(double w){
	return w/(double)WMAX;
}
double f4(double w){
	return 1-f1(w);
}

int evaluation(int turn, Queen *white_queens, Queen *black_queens, int *board){
	// if the game has to end, one of the two player wins
	char end = TRUE;
	if (turn == WHITE){
		   for (int k = 0; k < NB_QUEENS; k++){ if (!is_queen_stuck(white_queens[k], board)){ end = FALSE; break; } } }
	else { for (int k = 0; k < NB_QUEENS; k++){ if (!is_queen_stuck(black_queens[k], board)){ end = FALSE; break; } } }
	if (end){ return (turn == WHITE)? WIN:-WIN; }
	
	// otherwise, we count the minimal distances from each empty square to the closest white/black queen using queen/king moves
	Explorer white_dists[SIZE], black_dists[SIZE];
	reset_distances(white_dists);
	reset_distances(black_dists);
	evaluate_amazons(white_queens, white_dists, board);
	evaluate_amazons(black_queens, black_dists, board);
	
	int t1 = 0, t2 = 0, t, k;
	for (k = 0; k < SIZE; k++){
		if (board[k] == EMPTY){
			t1 += delta(white_dists[k].queen_dist, black_dists[k].queen_dist);
			t2 += delta(white_dists[k].king_dist,  black_dists[k].king_dist);
		}
	}
	double w = w_v(white_dists, black_dists);
	t = f1(w)*t1 + f4(w)*t2;
	
	return (t != 0)? t : -turn;
}

// used for debugging
void show_map_queens(Explorer *distances){
	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			if (distances[i*WIDTH+j].queen_dist == INT_MAX){ printf(" "); printf_utf_8(0x221e); printf(" "); }
			else    									   { printf("%2d ", distances[i*WIDTH+j].queen_dist); }
		}
		printf("\n");
	}
}
// used for debugging
void show_map_kings(Explorer *distances){
	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			if (distances[i*WIDTH+j].king_dist == INT_MAX){ printf(" "); printf_utf_8(0x221e); printf(" "); }
			else    									  { printf("%2d ", distances[i*WIDTH+j].king_dist); }
		}
		printf("\n");
	}
}
/////////////////////
