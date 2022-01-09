/*#include "amazons.h"*/

/*typedef struct State State;*/
/*struct State{*/
/*	int wins;*/
/*	int total;*/
/*	State *next;*/
/*};*/

/*int monte_carlo_tree_search(int color, State *tree, Queen *white_queens, Queen *black_queens, int *board){*/
/*	char end = 1;*/
/*	for (int k = 0; k < NB_QUEENS; k++){*/
/*		if (!is_queen_stuck((color == WHITE)? white_queens[k] : black_queens[k], board)){ end = 0; break; }*/
/*	}*/
/*	if (end == 1){ return -color; }*/

/*}*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "amazons.c"

typedef struct Node Node;
typedef Node* Tree;

struct Node{
	int win, tot, nbc, color;
	int brd[SIZE];
	Queen wht[NB_QUEENS], blk[NB_QUEENS];
	uint64_t sig;
	Tree prt, chd, bro;
};

void init_tree(Tree tree, Tree parent, int color, uint64_t sig, Queen *wht, Queen *blk, int *brd);
void add_bro(Tree tree, Tree bro);
void free_tree(Tree tree);
void qun_cpy(Queen *qun1, Queen *qun2);
void brd_cpy(int *brd1, int *brd2);

void selection();
Tree expansion(Tree tree);
void simulation();
void backpropagation();
void monte_carlo_tree_search();

void selection(){

}
	
Tree expansion(Tree tree){
	int new_brd[SIZE]; brd_cpy(tree->brd, new_brd);
	Queen new_wht[NB_QUEENS], new_blk[NB_QUEENS]; qun_cpy(tree->wht, new_wht); qun_cpy(tree->blk, new_blk);
	uint64_t new_sig = tree->sig;
	
	char end = TRUE;
	int queens[NB_QUEENS], queens_size = 0;
	for (int k = 0; k < NB_QUEENS; k++){
		if (!is_queen_stuck((tree->color == WHITE)? new_wht[k] : new_blk[k], new_brd)){ 
			end = FALSE; 
			queens[queens_size] = k;
			queens_size++;
		}
	}
	if (end){ return NULL; }
	
	Queen moves[MAX_MOVES], arrws[MAX_MOVES];
	int moves_size = 0, arrws_size = 0;
	int q, m, a;
	printf("ok\n");
	q = queens[rand()%queens_size];
	
	if (tree->color == WHITE){ 
		moves_size = reachable_places(new_wht[q], moves, new_brd);
		m = rand()%moves_size;
		new_brd[new_wht[q].i*WIDTH + new_wht[q].j] = EMPTY;
		new_wht[q].j = moves[m].j; new_wht[q].i = moves[m].i;
		new_brd[new_wht[q].i*WIDTH + new_wht[q].j] = WHITE; }
	else{ 
		moves_size = reachable_places(new_blk[q], moves, new_brd);
		m = rand()%moves_size;
		new_brd[new_blk[q].i*WIDTH + new_blk[q].j] = EMPTY;
		new_blk[q].j = moves[m].j; new_blk[q].i = moves[m].i;
		new_brd[new_blk[q].i*WIDTH + new_blk[q].j] = BLACK; }
	
	printf("cool\n");
	arrws_size = reachable_places(moves[m], arrws, new_brd);
	a = rand()%arrws_size;
	new_brd[arrws[a].i*WIDTH + arrws[a].j] = ARROW;
	
	printf("interessant\n");
	Tree new_chd = malloc(sizeof(Tree));
	printf("init chd : ");
	init_tree(new_chd, tree, -tree->color, new_sig, new_wht, new_blk, new_brd);
	printf("init done : ");
	printf("tree->chd = %s, tree->prt = %s, tree->bro = %s\n", (tree->chd == NULL)? "NULL":"NON-NULL", (tree->prt == NULL)? "NULL":"NON-NULL", (tree->bro == NULL)? "NULL":"NON-NULL");
	if (tree->chd == NULL){ tree->chd = new_chd; }
	else { add_bro(tree->chd, new_chd); }
	return new_chd;
}

void simulation(){

}
void backpropagation(){

}

void monte_carlo_tree_search(){

}

int main(int argc, char *argv[]){
	printf("Hello world !\n");
	Tree tree = malloc(sizeof(Tree));
	uint64_t sig;
	setup_board(&sig, white_queens, black_queens, board);
	printf("init tree : ");
	init_tree(tree, NULL, 0, WHITE, white_queens, black_queens, board);
/*	display_board(tree->brd);*/
	expansion(tree);
	free_tree(tree);
	return 0;
}

void init_tree(Tree tree, Tree parent, int color, uint64_t sig, Queen *wht, Queen *blk, int *brd){
	tree->win = 0; tree->tot = 0; tree->nbc = 0;
	tree->prt = parent;
	tree->chd = NULL; tree->bro = NULL;
	
	printf("tree->chd = %s, tree->prt = %s, tree->bro = %s\n", (tree->chd == NULL)? "NULL":"NON-NULL", (tree->prt == NULL)? "NULL":"NON-NULL", (tree->bro == NULL)? "NULL":"NON-NULL");
	if (parent != NULL){
		printf("parent->chd = %s, parent->prt = %s, parent->bro = %s\n", (parent->chd == NULL)? "NULL":"NON-NULL", (parent->prt == NULL)? "NULL":"NON-NULL", (parent->bro == NULL)? "NULL":"NON-NULL");
	} else { printf("parent = NULL\n"); }
	if (parent != NULL){
		printf("tree->prt->chd = %s, tree->prt->prt = %s, tree->prt->bro = %s\n", (tree->prt->chd == NULL)? "NULL":"NON-NULL", (tree->prt->prt == NULL)? "NULL":"NON-NULL", (tree->prt->bro == NULL)? "NULL":"NON-NULL");
	} else { printf("tree->prt = NULL\n"); }
	
	tree->color = color;
	brd_cpy(brd, tree->brd);
	qun_cpy(wht, tree->wht);
	qun_cpy(blk, tree->blk);
	tree->sig = sig;
	
	if (parent != NULL){
		printf("parent->chd = %s, parent->prt = %s, parent->bro = %s\n", (parent->chd == NULL)? "NULL":"NON-NULL", (parent->prt == NULL)? "NULL":"NON-NULL", (parent->bro == NULL)? "NULL":"NON-NULL");
	} else { printf("parent = NULL\n"); }
	if (parent != NULL){
		printf("tree->prt->chd = %s, tree->prt->prt = %s, tree->prt->bro = %s\n", (tree->prt->chd == NULL)? "NULL":"NON-NULL", (tree->prt->prt == NULL)? "NULL":"NON-NULL", (tree->prt->bro == NULL)? "NULL":"NON-NULL");
	} else { printf("tree->prt = NULL\n"); }
}

void free_tree(Tree tree){
	if (tree->bro != NULL){ free_tree(tree->bro); }
	if (tree->chd != NULL){ free_tree(tree->chd); }
	else { free(tree); }
}

void add_bro(Tree tree, Tree bro){
	printf("adding bro\n");
	if (tree->bro == NULL){ tree->bro = bro; }
	else { add_bro(tree->bro, bro); }
}

void qun_cpy(Queen *qun1, Queen *qun2){
	for (int k = 0; k < NB_QUEENS; k++){ qun2[k] = qun1[k]; }
}

void brd_cpy(int *brd1, int *brd2){
	for (int k = 0; k < SIZE; k++){ brd2[k] = brd1[k]; }
}
