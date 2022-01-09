////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Author : Antoine Stevan*/
/*Date   : 09/06/2020*/
/*Description : this file is the alpha beta algorithm header*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ALPHA_BETA_HASH_H_INCLUDED
#define ALPHA_BETA_HASH_H_INCLUDED

#include "amazons.h"

#define NB_BITS 25
#define HASH_SIZE (1 << NB_BITS)
#define HASH_MASK (HASH_SIZE - 1)

typedef struct hash_line hash_line;
struct hash_line{
	uint64_t sig;
	uint8_t v_d;
	#ifdef ITER
	int16_t v_inf, v_sup;
	unsigned int v_bmove, v_base;
  	#else
  	int8_t v_inf, v_sup;
  	#endif
};

uint64_t rand64_v()
{
  return
    (((uint64_t)rand())<<62) ^
    (((uint64_t)rand())<<31) ^
    ((uint64_t)rand());
}

#ifdef HASH
uint64_t hash_white[SIZE];
uint64_t hash_black[SIZE];
uint64_t hash_arrws[SIZE];

uint64_t hash_turn;

hash_line hash_table[HASH_SIZE];
#endif

uint64_t init_hash(){
	for (int i = 0; i < SIZE; i++){
		hash_white[i] = rand64_v();
		hash_black[i] = rand64_v();
		hash_arrws[i] = rand64_v();
	}
	for (int k = 0; k < HASH_SIZE; k++){ 
		hash_table[k].v_d = 0; 
		#ifdef ITER
		hash_table[k].v_base = 0; hash_table[k].v_bmove = 0; 
		#endif	
	}
	return rand64_v();
}

// tells 
#ifndef ITER
// tells if the signature has already been seen through the algorithm, if true, modifies v_inf and v_sup
char retrieve_v_hash(uint64_t sig, int *v_inf, int *v_sup);
// stores v in v_inf and v_sup, exact evaluation of the node. Storage only if distance condition is satisfied.
void store_v_hash_both(uint64_t sig, int v, int depth);
// stores or reset the has table at the signature given
void store_v_hash(uint64_t sig, int alpha, int beta, int g, int depth);
int alpha_beta_hash(int alpha, int beta, int color, Queen *white_queens, Queen *black_queens, int depth, uint64_t sig, int *board);
#else
// same functions with more conditions to satisfy to enable storage
char retrieve_v_iter_hash(uint64_t sig, int dist, int *v_inf, int *v_sup, int *v_bmove);
void store_v_iter_hash_both(uint64_t sig, int v, int dist, int base, int move);
void store_v_iter_hash(uint64_t sig, int alpha, int beta, int g, int dist, int base, int move);
int alpha_beta_iter_hash(int alpha, int beta, int color, Queen *white_queens, Queen *black_queens, int depth, int maxdepth, int base, uint64_t sig, int *best_move, int *board);
#endif

#endif // ALPHA_BETA_HASH_H_INCLUDED
