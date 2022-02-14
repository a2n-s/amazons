# This repo is getting old and was not developed under version control, hence the single commit.

Welcome to the GAME OF AMAZONS.

Everything is made to make your life easier whilst trying to play the game. You have to do almost nothing however, here are some points to take into consideration :

	- I don't have any makefile on my computer, so the inclusion of code files is not like we have been taught. I am including .c in other .c files, I know the drawbacks of this method, but the final code is the same as including the .h only where they are needed.
	- This was easier for compilation. You just have to write "gcc -std=c99 -Wall -Werror -O3 main.c name_of_executable" in your favorite terminal to compile the program and then write "./name_of_executable" to run it. If I had a makefile, I would have included .h instead of .c and hide the compilation of the involving .c in the makefile (to create temporary .o files and then link them together as a final executable).
	- the code is made for you. Everything is taken into consideration when compiling. Of course, you have access to some parameters that modify the compilation process and thus the executable behaviour. These parameters are located in a single place. Simply go to the file parameters.h, you can see some parameters to tweek (by defining them or not):
			- BOARD3 to BOARD10 : this is the size of the board. Define the one you want and the board will be of that size (only one, if you define several, the smallest will be prioritized).
			- COMP : define it if you want to compare the results of the algorithms. DON'T compare if you use iterative deepening, in any case the choice of iterative deepening OVERWRITES your other choices (they are incompatible with the simpler algorithms). ONLY compare minimax, alpha beta and alpha beta with hash table. You have to know that minimax runs only on the 3*3 grid and alpha beta struggle to resolve the 4*4 grid (it takes a long time on my computer). Alpha beta with hash table is very fast on these sizes.
			- MINIMAX : enables the minimax algorithm. (DON'T do iterative deepening at the same time)
			- MINIMAX_DEBUG : enables the minimax algorithm with debug messages. (DON'T do iterative deepening at the same time)
			- ALPHA_BETA : enables the alpha beta algorithm. (DON'T do iterative deepening at the same time)
			- ALPHA_BETA_CUT : enables the alpha beta algorithm. Give a value to ALPHA_BETA_CUT and the exploration of the game tree will stop at this depth by using the evaluation function. (DON'T do iterative deepening at the same time)
			- ALPHA_BETA_HASH : enables the alpha beta algorithm with hash table. (DON'T do iterative deepening at the same time)
			
			- ALPHA_BETA_ITER : enables the iterative deepening alpha beta algorithm. This choice overwrites the other simpler algorithms above. DON'T define the two iterative deepening algorithms at the same time.
			- ALPHA_BETA_ITER_HASH : enables the iterative deepening alpha beta algorithm with hash table. This choice overwrites the other simpler algorithms above. DON'T define the two iterative deepening algorithms at the same time.
			
			- EMPTY_BOARD : define it to start with an empty board. If defined : not implemented so define it anyway.
			- RANDOM_HUMAN : allows a random agent to play against the AI (only if you're lazy today :-D)
			
			- SECONDS : this is the number of seconds allowed for the iterative deepening (with or without hash). You HAVE TO DEFINE it if you choose one of the two iterative deepening algorithms.
			- MICSECS : this is the number of micro seconds allowed (in addition of SECONDS) for the iterative deepening (with or without hash). You HAVE TO DEFINE it if you choose one of the two iterative deepening algorithms.
