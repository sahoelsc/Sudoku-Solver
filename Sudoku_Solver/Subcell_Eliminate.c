// Subcell_eliminate.c: 
//
// This file handles eliminating possibilities in a Subcell after
// assigning a value. (but not the row and column that that entry is in.
// Entries to this function must be verified, no checking happens here.

#include "Functions.h"
#include "stdafx.h"

int subcell_eliminate(int a, int row, int column) {

	int i, j, k, mod_column, mod_row, debug_column, debug_row;
	int subcell_row[2], subcell_column[2];

	// Find modulus:
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
	//switch statement for row
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
	//
	for (i = 0; i < 2; ++i) {
		for (j = 0; j < 2; ++j) {
			//check to see if the value of the cell has been found or not.
			debug_row = row + subcell_row[i];
			debug_column = column + subcell_column[j];
			if (get_value(debug_row, debug_column) == 0) {
				//check to see that this value has not been eliminated already.
				if (get_elim(debug_row, debug_column, a) == 0) {
					set_elim(debug_row, debug_column, a, 1);
					set_count(debug_row, debug_column, 1);
					if (get_count(debug_row, debug_column) > 8) {
						printf("I found an issue in setting subcell eliminated.\n");
						printf("currently eliminating eliminated from:\n");
						printf("row: %d, column: %d\n", row, column);
						printf("location = [%d][%d]: Count = %d\n", debug_row, debug_column, get_count(debug_row, debug_column));
						printf("Possible: [");
						for (k = 0; k < 10; ++k) {
							printf(" %d", get_elim(debug_row, debug_column, k));
						}
						printf("\n");
					}
				}
			}
		}
	}
	return 0;
}