// Row_Eliminate.c : 
//
// This file handles eliminating possibilities in a row after
// assigning a value
// Entries to this function must be verified, no checking happens here.

#include "Functions.h"
#include "stdafx.h"

int row_eliminate(int a, int row, int column) {
	//local variables:
	int i, k;

	for (i = 0; i < 9; ++i) {
		//exclude the cell you just set
		if (i != column) {
			//check to see if the value has already been determined
			if (get_value(row,i) == 0) {
				//check to see that this value has not been eliminated already.
				if (get_elim(row, i, a) == 0) {
					set_elim(row, i, a, 1);
					set_count(row, i, 1);
					if (get_count(row, i) > 8) {
						printf("I found an issue in setting column possibilies\n");
						printf("currently eliminating eliminated from:\n");
						printf("row: %d, column: %d\n", row, column);
						printf("location = [%d][%d]: Count = %d\n", row, i, get_count(row, i));
						printf("Possible: [");
						for (k = 0; k < 10; ++k) {
							printf(" %d", get_elim(row, i, k));
						}
						printf("\n");
					}
				}
			}
		}
	}
	return 0;
}