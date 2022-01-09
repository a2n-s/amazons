////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Author : Antoine Stevan*/
/*Date   : 09/06/2020*/
/*Description : this file is the main header of the project and contains all the prototypes needed to play and compilation parameters to tweek*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AMAZONS_H_INCLUDED
#define AMAZONS_H_INCLUDED

#include <stdint.h>
#include <signal.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "parameters.h"

#define WHITE +1
#define BLACK -1
#define  DRAW  0

#define EMPTY  0
#define ARROW  2

#define TRUE 1
#define FALSE 0

#define EXIT_ERROR 126

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

// in the following, j is the number of the column (0 <= j < WIDTH) and i is the number of the row (0 <= i < HEIGHT) on the board. To access the element at (j, i), write board[i*WIDTH + j]
// this is not true in the initialization of the board !! i and j are swapped for it is easier in my opinion to read the coordinates of a square on the board when they are backwards. For instance, in printf, j is always printed before i for convenience

// the Queen can also represent any piece on the board
typedef struct Queen Queen;
struct Queen {
    int i;
    int j;
	//char stuck;
};

#ifdef ALPHA_BETA_ITER
#define ITER
#endif

#ifdef ALPHA_BETA_HASH
#define HASH
#endif
#ifdef ALPHA_BETA_ITER_HASH
#define HASH
#define ITER
#endif

// initialization of the initial configuration of the board according to the parameters of the compilation
// MAX_MOVES is the maximum numbers of empty squares accessible at a given point of the game with a given board size
#ifdef BOARD3
	#define  WIDTH 3
	#define MAX_MOVES 8
	#define WMAX 5
	#define NB_QUEENS 1
	Queen  BLACK_INIT[1] = {{1, 0}}, 			      		  WHITE_INIT[2] = {{1, 2}};
#else
	#ifdef BOARD4
		#define  WIDTH 4
		#define MAX_MOVES 11
		#define WMAX 11
		#define NB_QUEENS 1
		Queen  BLACK_INIT[1] = {{2, 0}}, 			      		  WHITE_INIT[2] = {{1, 3}};
	#else
		#ifdef BOARD5
			#define  WIDTH 5
			#define MAX_MOVES 16
			#define WMAX 15
			#define NB_QUEENS 2
			Queen  BLACK_INIT[2] = {{2, 0}, {0, 2}},		      	  WHITE_INIT[2] = {{2, 4}, {4, 2}};
		#else
			#ifdef BOARD6
				#define  WIDTH 6
				#define MAX_MOVES 19
				#define WMAX 23
				#define NB_QUEENS 2
				Queen  BLACK_INIT[2] = {{3, 0}, {0, 2}}, 		      	  WHITE_INIT[2] = {{2, 5}, {5, 3}};
			#else
				#ifdef BOARD7
					#define  WIDTH 7
					#define MAX_MOVES 24
					#define WMAX 31
					#define NB_QUEENS 3
					Queen  BLACK_INIT[3] = {{3, 0}, {0, 2}, {6, 2}}, 	      WHITE_INIT[3] = {{3, 6}, {0, 4}, {6, 4}};
				#else
					#ifdef BOARD8
					#define  WIDTH 8
					#define MAX_MOVES 27
					#define WMAX 39
					#define NB_QUEENS 3
					Queen  BLACK_INIT[3] = {{4, 0}, {0, 2}, {7, 2}}, 	      WHITE_INIT[3] = {{3, 7}, {0, 5}, {7, 5}};
					#else
					#ifdef BOARD9
					#define  WIDTH 9
					#define MAX_MOVES 32
					#define WMAX 55
					#define NB_QUEENS 4
					Queen  BLACK_INIT[4] = {{3, 0}, {5, 0}, {0, 3}, {8, 3}}, WHITE_INIT[4] = {{3, 8}, {5, 8}, {0, 5}, {8, 5}};
					#else
					#ifdef BOARD10
					#define  WIDTH 10
					#define MAX_MOVES 35
					#define WMAX 70
					#define NB_QUEENS 4
					Queen BLACK_INIT[4] = {{3, 0}, {6, 0}, {0, 3}, {9, 3}}, WHITE_INIT[4] = {{3, 9}, {6, 9}, {0, 6}, {9, 6}};
					#endif 
					#endif
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#define HEIGHT WIDTH
#define SIZE (WIDTH*HEIGHT)

#define WIN 32766
#define BWIN (WIN -SIZE)
#define MAXV 32767

// keep track of the state of the board with two elements : a board with all the pieces and queen arrays to access the queens faster
Queen white_queens[NB_QUEENS];
Queen black_queens[NB_QUEENS];

#ifdef EMPTY_BOARD
#define INIT_ARROWS 0
Queen ARROWS[INIT_ARROWS] = {};
#else
#define INIT_ARROWS 8
// you can add here "by default" arrows that are on the board at the beginning ot the game, don't forget to change INIT_ARROWS to the size of ARROWS
Queen ARROWS[INIT_ARROWS] = {{0, 0}, {0, 4}, {4, 0}, {4, 4}, {1, 0}, {4, 1}, {3, 4}, {0, 3}};
#endif

#ifdef ITER
#define GET_OUT 32768
struct itimerval timer={{0,0},{0,0}};
int setitimer(int _which, const struct itimerval *_restrict_value, struct itimerval *_restrict_ovalue);
int get_out = FALSE;
void handler(int signum) { get_out=TRUE;};
#endif

int board[HEIGHT*WIDTH] = {0};

int abs(int a);
int distance(int a, int b, int c, int d);

// encodes the move : if queen with index k moves to (j, i) and shoots an arrow to (m, l), the encoded movement is lmijq in base 10 thanks to the fact that all the numbers are always less than 10
int encode(int q, Queen *moves, int m, Queen *arrws, int a){
	return q + 10*moves[m].j + 100*moves[m].i + 1000*arrws[a].j + 10000*arrws[a].i;
}

// the opposite of encode
void decode(int move, int *q, int *mj, int *mi, int *aj, int *ai){
	*q = move%10;
	move = move/10; *mj = move%10;
	move = move/10; *mi = move%10;
	move = move/10; *aj = move%10;
	move = move/10; *ai = move%10;
}

// setups the game by updating the "queen" arrays and the board according to the rules of the Game of Amazons
#ifdef HASH
void setup_board(uint64_t *sig, Queen *white, Queen *black, int *board);
#else
void setup_board(Queen *white, Queen *black, int *board);
#endif

// tell whether queen q is stuck on the board or not
bool is_queen_stuck(Queen q, int *board);

// enables the human player to play. move encodes the player move in a 6 decimal digit number. player_queens and board are modified. 
////unused////
int play(int move, int player_color, Queen *player_queens, int *board);

// enables the human player to play step by step. It is easier for a human being to understand what's going on'.
// the queen q is a pointer to a Queen in order to "give" this information to move_queen and then shoot_arrow
int choose_queen(Queen *q, int player_color, int *board);
#ifdef HASH
int move_queen(Queen *q, uint64_t *sig, int player_color, Queen *player_queens, int *board);
int shoot_arrow(Queen *q, uint64_t *sig, int *board);
#else
int move_queen(Queen *q, int player_color, Queen *player_queens, int *board);
int shoot_arrow(Queen *q, int *board);
#endif

// computes all possible places you can reach from a given position. places is modified (filled with these places)
int reachable_places(Queen q, Queen *places, int *board);
// enables the AI to play with the algorithms, according to the compilation parameters
#ifdef ITER
#ifdef HASH
uint64_t play_AI(int player_color, Queen *white_queens, Queen *black_queens, uint64_t sig, int base, int *board);
#else
void play_AI(int player_color, Queen *white_queens, Queen *black_queens, int base, int *board);
#endif
#else
#ifdef HASH
uint64_t play_AI(int player_color, Queen *white_queens, Queen *black_queens, uint64_t sig, int *board);
#else
void play_AI(int player_color, Queen *white_queens, Queen *black_queens, int *board);
#endif
#endif

// this is the random agent
void play_at_random(int color, Queen *queens, int *board);

// tells if the board is full (debug) 
////unused////
bool is_board_full(int *board);

// displays the board (simple version) in the console
void display_board(int *board);

//displays the board with an overlay to show the possible moves and help the human player
void show_moves(Queen queen, int *board);

void printf_utf_8(int code);

//// some tools on arrays of int and arrays of queens
// checks if a position is inside a queen array
bool is_in(Queen *array, int j, int i, int size);
// copy of tab1 in tab2
void mat_cpy(Queen *tab1, Queen *tab2, int n);
// displays an array (debug)
void show_tab(int *tab, int size);
// displays the two arrays of queens (debug)
void show_queens(Queen *white, Queen *black);

#endif // AMAZONS_H_INCLUDED
