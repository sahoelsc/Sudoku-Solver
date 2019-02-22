// Sudoku_Output.c : Defines the entry point for the console application
//
// Ouputs a sudoku puzzle to the terminal window

#include "Functions.h"
#include "stdafx.h"

int sudoku_output()
{
	int row;
	int column;
	char line[] = "+-+-+-+-+-+-+-+-+-+\n";

	printf(line);

	for (row = 0; row < 9; ++row) {
		for (column = 0; column < 9; ++column) {
			if (get_value(row, column) == 0) {
				printf("|_");
			}
			else {
				printf("|%d", get_value(row, column));
			}
			
		}
		printf("|\n");
		printf(line);
	}
	return 0;
}