// Branching.c
//
#include "Functions.h"
#include "stdafx.h"

//This set of functions will handle branching and managment of 
// non-100% certainty decisions

struct branch {
	int main_index;
	int row;
	int column;
	int choice;
};

// Starting with a branch size of 50. I can change that if needed
// but 50 seems like way more than I will ever need.
static struct branch branchlist[100];
static int current_branch = 0;

void branching(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)()) {
	struct subcell location;
	struct branch branch_information;
	int choice;
	
	printf("----------------------------------------\n");
	printf("BRANCHING\n");
	

	//Step one. Determine which cell is going to be used for branching
	location = pick_branch_cell();

	//Step two. pick which of the two options you will be using.
	//create list of branch options. (for this location)
	//
	choice = pick_cell_option(location.row, location.column);

	//Step three. record 
	// 0. Main ledger index from before the branch.
	//(i.e. the one you will rewind to if this branch fails.)
	// 1. location (row, column)
	// 2. What number you chose
	// 3. Store this in the nested branch list.
	branch_information.main_index = get_most_recent_main_index();
	branch_information.row = location.row;
	branch_information.column = location.column;
	branch_information.choice = choice; // depends on step 2;
	branchlist[current_branch] = branch_information;
	current_branch++; //increment current branch



	//Set the number and send back to the logical set to evaluate until failure
	//
	cell_assign(location.row, location.column, choice, display_fptr, latest_fptr,flip_fptr);

	printf("----------------------------------------\n");
}


struct subcell pick_branch_cell() {
	//this is a function that chooses the next cell to be branched.
	//if a more sophisticated method is chosen, I can just replace this function call
	struct subcell location;
	int row, column;
	int best_available_cell;

	for (best_available_cell = 7; best_available_cell > 0; best_available_cell--) {
		for (row = 0; row < 9; row++) {
			for (column = 0; column < 9; column++) {
				if (get_count(row, column) == best_available_cell) {
					location.row = row;
					location.column = column;
					return location;
				}
			}
		}

	}
}

int pick_cell_option(int row, int column) {
	int i;
	//this is a function to pick which of the options for a cell you will use
	//the initial choice will just be picking the first elim available.
	for (i = 1; i < 10; ++i) {
		if (get_elim(row, column, i) == 0) {
			return i;
		}
	}
}

void branch_rewind() {
	//this function rewinds a branch that has failed.
	// it needs to: walk back up the ledger, removing items and undoing
	// them until it hits it's main index goal (in the branch list)
	// then it takes that branch item and sets it to eliminated.
	int goal_index;
	printf("----------------------------------------\n");
	printf("BRANCH REWIND. BRANCH FAIL\n");
	printf("----------------------------------------\n");
	//1: go get the most recent branch: 
	// branchlist[current_branch-1];
	goal_index = branchlist[current_branch - 1].main_index;
	// Then rewind the puzzle till that main index
	rewind_puzzle(goal_index);
	set_elim(branchlist[current_branch - 1].row, branchlist[current_branch - 1].column, branchlist[current_branch - 1].choice, 1);
	set_count(branchlist[current_branch - 1].row, branchlist[current_branch - 1].column, 1);
	//Something is wrong here
	current_branch--;
	printf("\n");
	sudoku_output();
	printf("\n");


}