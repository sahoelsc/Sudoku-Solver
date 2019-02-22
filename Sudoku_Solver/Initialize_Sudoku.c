// Initialize_Sudoku.c : Defines the Entry point for the Console application
//

#include "Functions.h"
#include "stdafx.h"

static struct cell puzzle[9][9];


int initialize_sudoku() {
	
	int row, column, i;

	for (row = 0; row < 9; ++row) {
		for (column = 0; column < 9; ++column) {
			puzzle[row][column].value = 0;
			puzzle[row][column].count = 0;
			for (i = 0; i < 10; ++i) {
				puzzle[row][column].eliminated[i] = 0;
			}


		}
	}

	//Part of initializing the Puzzle is init-ing the ledger. 
	init_ledger();

	return 0;

}

//Get Functions
int get_value(int row, int column) {
	return puzzle[row][column].value;
}

int get_elim(int row, int column, int number) {
	return puzzle[row][column].eliminated[number];
}

int get_count(int row, int column) {
	return puzzle[row][column].count;
}

//Set Functions
void set_value(int row, int column, int set) {
	// in order to have the calls work so that the actual previous value is correct
	// this must be in front of the  actual assignment.
	create_value_transaction(row, column, set);
	puzzle[row][column].value = set;
	//Increase th main index on the ledger entry
	//reset the sub index to 0.
	//
	
}

void set_elim(int row, int column, int number, int boolean) {
	//create ledger entry
	create_elim_transaction(row, column, number, boolean);

	//execute ledger change
	puzzle[row][column].eliminated[number] = boolean;
}

void set_count(int row, int column, int up_or_zero) {
	if (up_or_zero == 1){
		create_count_transaction(row, column, up_or_zero);
		puzzle[row][column].count++;
	}
	if (up_or_zero == 0) {
		create_count_transaction(row, column, up_or_zero);
		puzzle[row][column].count = 0;
	}
}
//rewind functions do not create any ledger entries
void rewind_value(int row, int column, int from) {
	puzzle[row][column].value = from;
}

void rewind_elim(int row, int column, int elim_number, int from) {
	puzzle[row][column].eliminated[elim_number] = from;
}

void rewind_count(int row, int column, int from) {
	puzzle[row][column].count = from;
}

//solved??
int is_puzzle_solved() {
	int row, column;
	for (row = 0; row < 9; row++) {
		for (column = 0; column < 9; column++) {
			if (puzzle[row][column].value == 0) {
				return 0;
			}
		}
	}
	//if you've made it here, the puzzle is solved.
	return 1;
}