// Sudoku_input.c : Define entry point for console application
//

#include "Functions.h"
#include "stdio.h"

int sudoku_input(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)())
{
	//This function imports a 9x9 sudoku puzzle into a 9x9 array from a .txt file.
	//open the file to read it:

	FILE *inputfile;
	int row, column, a;
	inputfile = fopen("sudoku_input.txt", "r");

	//Ensure that the file was opened succesfully:
	if (inputfile == NULL) {
		printf("The input file could not be opened");
	}

	//Read the input in from the file
	for (row = 0; row < 9; ++row) {
		for (column = 0; column < 9; ++column) {
			fscanf_s(inputfile, "%d", &a);
			// Assign to the cell
			cell_assign(row, column, a, display_fptr,latest_fptr,flip_fptr);
		}
	}

	//close file
	fclose(inputfile);
	//and update display with newly entered items
	set_input_complete();
	return 0;
}