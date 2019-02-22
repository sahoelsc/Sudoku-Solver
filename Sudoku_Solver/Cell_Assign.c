// Cell Assign.c
// Handles assigning a value to a cell
//

#include "Functions.h"
#include "stdafx.h"

cell_assign(int row, int column, int a, 
	void(*display_fptr)(), 
	void(*latest_fptr)(int, int, int, int),
	void(*flip_fptr)()) {
	//display();
	display_fptr();
	int* cell_array;
	cell_array = row_column_to_cell_subcell(row, column);
	latest_fptr(cell_array[0], cell_array[1], a, 1);
	flip_fptr();
	// Assign to the cell:
	set_value(row, column, a);
	
	// Then Handle count and eliminates
	if (a != 0) {
		set_count(row, column, 0);

		//Row Eliminate:
		row_eliminate(a, row, column);

		//Column Eliminate:
		column_eliminate(a, row, column);

		//Subcell Eliminate:
		subcell_eliminate(a, row, column);
	}

	return 0;
}