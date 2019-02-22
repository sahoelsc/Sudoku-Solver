// Solver.c :
// Takes a sudoku puzzle from main, and returns it solved.
//

#include "Functions.h"
#include "stdafx.h"

void sudoku_solver(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)()) {
	int solved = 0;
	int logical_exit_condition;

	(display_fptr)();

	while (solved == 0) {
		//the first step is to do all the logical steps necessary
		logical_exit_condition = logical_sudoku_solver(display_fptr, latest_fptr, flip_fptr);
		solved = is_puzzle_solved();
		if (logical_exit_condition == -1 && solved == 0) {
			logical_exit_condition = 0;
			//sudoku_output();
			//printf("%d \n",get_most_recent_main_index());
			branch_rewind();
			continue;
		}
		solved = is_puzzle_solved();

		//begin branching.
		if (solved == 0) {
			branching(display_fptr, latest_fptr,flip_fptr);
		}
		

	}
	
}

int logical_sudoku_solver(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)()) {
	int found_eight, done;
	int row_cells, column_cells, subcell_cells;
	done = 0;
	row_cells = 0;
	column_cells = 0;
	subcell_cells = 0;
	
	//go through the puzzle to find the next cell where count == 8.
	//
	do {
		//solve by simple elimination
		do {
			found_eight = iterate_for_eights(display_fptr, latest_fptr,flip_fptr);
			//break out of the function if you find a failure
			if (found_eight == -1) {
				return -1;
			}
		} while (found_eight == 1);
		
		//solve by rows, columns, and subcells.
		row_cells = solve_by_row(display_fptr, latest_fptr, flip_fptr);
		//if this fails, break out of the function with a fail flag
		if (row_cells == -1) {
			return -1;
		}
		column_cells = solve_by_column(display_fptr, latest_fptr, flip_fptr);
		//if this fails, break out of the function with a fail flag
		if (column_cells == -1) {
			return -1;
		}
		subcell_cells = solve_by_subcell(display_fptr, latest_fptr, flip_fptr);
		//if this fails, break out of the function with a fail flag
		if (subcell_cells == -1) {
			return -1;
		}

		//Logical Elimination by already eliminated numbers.

		//row/column grind by elim.
		
		//exit conditions:
		if (row_cells == 0 && column_cells == 0 && subcell_cells == 0) {
			done = 1;
		}
		
	} while (done == 0);
	
	

	return 0;
}

