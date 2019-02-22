// Column Eliminate.c
//
// This file handles eliminating possibilities in a column after
// assigning a value
// Entries to this function must be verified, no checking happens here.

#include "Functions.h"
#include "stdafx.h"

int column_eliminate(int a, int row, int column) {
	int j, k;
	
	//The Basic structure to set column eliminated. 
	//We set all possibilies and add to the count for all cells in the column;
	for (j = 0; j < 9; ++j) {
		//exclude the cell you just set
		if (j != row) {
			//check to see if the value has already been determined
			if (get_value(j, column) == 0) {
				//check to see that this value has not been eliminated already.
				if (get_elim(j, column, a) == 0) {
					set_elim(j, column, a, 1);
					set_count(j, column, 1);
					if (get_count(j, column) > 8) {
						printf("I found an issue in setting row eliminated.\n");
						printf("currently eliminating from:\n");
						printf("row: %d, column: %d\n", row, column);
						printf("location = [%d][%d]: Count = %d\n", j, column, get_count(j, column));
						printf("Possible: [");
						for (k = 0; k < 10; ++k) {
							printf(" %d", get_elim(j, column, k));
						}
						printf("\n");
					}
				}
			}
		}
	}
	return 0;
}