// Cell_check.c
// Checks to see if the value to be assigned to a cell 
// causes an exception.

#include "Functions.h"
#include "stdafx.h"

int cell_check(int row,int column, int a) {
	
	//local variables
	int i, j, mod_column, mod_row, debug_column, debug_row;
	int subcell_row[2], subcell_column[2];
	// row check: go through the row.
	for (i = 0; i < 9; ++i) {
		if (i != row) {
			// if a cell has count == 8
			if (get_count(i, column) == 8) {
				// be sure that cell has already eliminated a
				switch (get_elim(i, column, a))
				{
				case 0: 
					//if no, break and return -1 to indicate failure
					return -1;
					break;
				case 1:
					break;
				}
			}
		}

		
		
	}

	//column check:
	for (j = 0; j < 9; ++j) {
		if (j != column) {
			//if a cell has count 8:
			if (get_count(row, j) == 8) {
				//be sure that cell has already eliminated a
				switch (get_elim(row, j, a))
				{
				case 0:
					return -1;
					break;
				case 1:
					break;
				default:
					return -1;
				}
			}
		}
	}

	//Subcell Check:
	mod_column = column % 3;
	mod_row = row % 3;
	//Use these modulus to set up the subcell eliminated and counts.
	switch (mod_row)
	{
	case 0:
		subcell_row[0] = 1;
		subcell_row[1] = 2;
		break;
	case 1:
		subcell_row[0] = -1;
		subcell_row[1] = 1;
		break;
	case 2:
		subcell_row[0] = -2;
		subcell_row[1] = -1;
		break;
	default:
		break;
	}
	//switch statement for column
	switch (mod_column)
	{
	case 0:
		subcell_column[0] = 1;
		subcell_column[1] = 2;
		break;
	case 1:
		subcell_column[0] = -1;
		subcell_column[1] = 1;
		break;
	case 2:
		subcell_column[0] = -2;
		subcell_column[1] = -1;
		break;
	default:
		break;
	}
	//Subcell Check
	for (i = 0; i < 2; ++i) {
		for (j = 0; j < 2; ++j) {
			debug_row = row + subcell_row[i];
			debug_column = column + subcell_column[j];
			if (get_count(debug_row, debug_column) == 8) {
				switch (get_elim(debug_row, debug_column, a))
				{
				case 0:
					//if no, break and return -1 to indicate failure
					return -1;
					break;
				case 1:
					break;
				default:
					return -1;
				}
			}
		}
	}
	return 0;
}