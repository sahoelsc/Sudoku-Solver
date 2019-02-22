// Solver_Functions.c
// 
// Holds all the functions that are used by solver.c to solve a sudoku puzzle.


#include "Functions.h"
#include "stdafx.h"


int iterate_for_eights(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)()) {
	// Local Variables:
	int row, column, check_val;
	int cell_value, k;

	for (row = 0; row < 9; ++row) {
		for (column = 0; column < 9; ++column) {
			if (get_count(row, column) == 8) {
				for (k = 1; k < 10; ++k) {
					if (get_elim(row, column, k) == 0) {
						cell_value = k;
					}
				}
				// Now Cell Value is known, check the value: 
				check_val = cell_check(row, column, cell_value);
				switch (check_val) {
				case 0:
					cell_assign(row, column, cell_value, display_fptr,latest_fptr,flip_fptr);
					return 1;
					break;
				case -1:
					//printf("you done broke it");
					return -1;
					break;
				default:
					printf("how did you do this? Check val returned != 0,-1");
					return -1;
				}
			}
		}
	}
	return 0;
}

int solve_by_row(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)()) {
	int row, column, i;
	int cell_value, found, unique;
	int solved[10];
	int cells_solved;
	int column_found, row_found;
	int check_val;

	cells_solved = 0;



	//begin iterating through the rows:
	for (row = 0; row < 9; ++row) {
		//setup:
		for (i = 0; i < 10; ++i) {
			solved[i] = 0;
		}
		found = 0;
		unique = 1;
		//create list of solved items
		//
		for (column = 0; column < 9; ++column) {
			if (get_value(row, column) != 0) {
				solved[get_value(row, column)] = 1;
			}
		}
		//Go to the first unsolved number in the list:
		//
		
		for (i = 1; i < 10; ++i) {
			//Only do the next part if the number is unknown
			if (solved[i] == 0) {

				// go through each entry in the row
				for (column = 0; column < 9; ++column) {
					//is the cell solved?
					if (get_value(row, column) == 0) {
						//Then if not solved, include in elimination options.
						//If you've already found it, check for uniqueness.
						if (found == 1) {
							if (get_elim(row, column, i) == 0) {
								unique = 0;
							}
						}
						// if you havent found it, check for existance.
						if (found == 0) {
							if (get_elim(row, column, i) == 0) {
								found = 1;
								column_found = column;
								row_found = row;
								cell_value = i;
							}
						}
					}

				}
				if (found == 0) {
					//Something has broken here. This is an error flag.
					printf("ROW this is an error flag. This shouldn't happen?\n");
					return -1;
				}
				if (unique == 1) {
					//if a cell is the only option for a number, assign that number
					check_val = cell_check(row_found, column_found, cell_value);
					switch (check_val) {
					case 0:
						cell_assign(row_found, column_found, cell_value,display_fptr,latest_fptr,flip_fptr);
						cells_solved++;
						break;
					case -1:
						//printf("you done broke it");
						return -1;
						break;
					default:
						printf("how did you do this? Check val returned != 0,-1");
						return -1;
					}
					
				}
				//Reset the values of found and unique for the next section:
				found = 0;
				unique = 1;
			}
		}

	}
	return cells_solved;
}

int solve_by_column(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)()) {
	int row, column, i;
	int cell_value, found, unique;
	int solved[10];
	int cells_solved;
	int row_found, column_found;
	int check_val;

	cells_solved = 0;



	//begin iterating through the columns:
	for (column = 0; column < 9; ++column) {
		//setup:
		for (i = 0; i < 10; ++i) {
			solved[i] = 0;
		}
		found = 0;
		unique = 1;
		//create list of solved items
		//
		for (row = 0; row < 9; ++row) {
			if (get_value(row, column) != 0) {
				solved[get_value(row, column)] = 1;
			}
		}
		//Go to the first unsolved number in the list:
		//

		for (i = 1; i < 10; ++i) {
			//if the number is known, just skip it.
			if (solved[i] == 0) {
				// go through each entry in the column
				for (row = 0; row < 9; ++row) {
					// make sure the cell is unsolved:
					if (get_value(row, column) == 0) {
						//if you've already found it, check for uniqueness:
						if (found == 1) {
							if (get_elim(row, column, i) == 0) {
								unique = 0;
							}
						}
						// otherwise, check for existance.
						if (found == 0) {
							if (get_elim(row, column, i) == 0) {
								found = 1;
								row_found = row;
								column_found = column;
								cell_value = i;
							}
						}
						
					}
				}

				
				if (found == 0) {
					//Something has broken here. This is an error flag.
					printf("row = %d, column = %d, value = %d\n", row, column, cell_value);
					printf("number? = %d, solved = %d\n",i, solved[i]);
					printf("COLUMN this is an error flag. This shouldn't happen?\n\n");
					return -1;
				}
				if (unique == 1) {
					//if a cell is the only option for a number, assign that number
					check_val = cell_check(row_found, column_found, cell_value);
					switch (check_val) {
					case 0:
						cell_assign(row_found, column_found, cell_value,display_fptr, latest_fptr,flip_fptr);
						cells_solved++;
						break;
					case -1:
						//printf("you done broke it");
						return -1;
						break;
					default:
						printf("how did you do this? Check val returned != 0,-1");
						return -1;
					}
				}
				//Reset the values of found and unique for the next section:
				found = 0;
				unique = 1;
			}
		}

	}
	return cells_solved;
}

int solve_by_subcell(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)()) {
	int subcell_index, cell_number, i;
	int row, column;
	int cell_value, found, unique, column_found, row_found;
	int solved[10];
	int cells_solved;
	int check_val;

	cells_solved = 0;

	struct subcell subcell_location;

	//Begin iterating over subcells. Gonna need to reference them. 
	// best way to do that is by upper left hand cell. then I can use 
	// a subcell interior reference to denote cells. 
	// So, instead of a row and column, i'm using subcell and subcell subset? Naming?
	// Subcell and cell number? ---- Now I have this. The subcell decode function is our friend.


	//begin iterating through the Subcells:
	for (subcell_index = 0; subcell_index < 9; ++subcell_index) {
		//setup:
		for (i = 0; i < 10; ++i) {
			solved[i] = 0;
		}
		found = 0;
		unique = 1;
		//create list of solved items
		//
		for (cell_number = 0; cell_number < 9; ++cell_number) {

			//decode code fragment. 
			subcell_location = subcell_decode(subcell_index, cell_number);
			row = subcell_location.row;
			column = subcell_location.column;
			//

			if (get_value(row, column) != 0) {
				solved[get_value(row, column)] = 1;
			}
		}
		//Go to the first unsolved number in the list:
		//

		for (i = 1; i < 10; ++i) {
			//if the number is known, just skip it.
			//Only do the next part if the number is unknown.
			if (solved[i] == 0) {


				// go through each entry in the column
				for (cell_number = 0; cell_number < 9; ++cell_number) {
					//decode code fragment:
					subcell_location = subcell_decode(subcell_index, cell_number);
					row = subcell_location.row;
					column = subcell_location.column;

					// Make sure the cell is unsolved:
					if (get_value(row, column) == 0) {
						//if you've already found it, check for uniqueness:
						if (found == 1) {
							if (get_elim(row, column, i) == 0) {
								unique = 0;
							}
						}
						// otherwise, check for existance
						if (found == 0) {
							if (get_elim(row, column, i) == 0) {
								found = 1;
								column_found = column;
								row_found = row;
								cell_value = i;
							}
						}
						
					}

				}
				if (found == 0) {
					//Something has broken here. This is an error flag.
					printf("row = %d, column = %d, value = %d\n", row, column, cell_value);
					printf("number? = %d, solved = %d\n", i, solved[i]);
					printf("SUBCELL this is an error flag. This shouldn't happen?\n");
					return -1;
				}
				if (unique == 1) {
					//if a cell is the only option for a number, assign that number
					check_val = cell_check(row_found, column_found, cell_value);
					switch (check_val) {
					case 0:
						cell_assign(row_found, column_found, cell_value,display_fptr,latest_fptr,flip_fptr);
						cells_solved++;
						break;
					case -1:
						//printf("you done broke it");
						return -1;
						break;
					default:
						printf("how did you do this? Check val returned != 0,-1");
						return -1;
					}
				}
				//Reset the values of found and unique for the next section:
				found = 0;
				unique = 1;
			}		
		}

	}
	return cells_solved;
}

struct subcell subcell_decode(int subcell_index, int cell_number) {
	//accepts a subcell index and a cell number, and translates into a row and column value.

	struct subcell location;

	int row_base, row_mod;
	int column_base, column_mod;
	
	switch (subcell_index){
	case 0:
		row_base = 0;
		column_base = 0;
		break;
	case 1:
		row_base = 0;
		column_base = 3;
		break;
	case 2:
		row_base = 0;
		column_base = 6;
		break;
	case 3:
		row_base = 3;
		column_base = 0;
		break;
	case 4:
		row_base = 3;
		column_base = 3;
		break;
	case 5:
		row_base = 3;
		column_base = 6;
		break;
	case 6:
		row_base = 6;
		column_base = 0;
		break;
	case 7:
		row_base = 6;
		column_base = 3;
		break;
	case 8:
		row_base = 6;
		column_base = 6;
		break;
	default:
		break;
	}

	switch (cell_number) {
	case 0:
		row_mod = 0;
		column_mod = 0;
		break;
	case 1:
		row_mod = 0;
		column_mod = 1;
		break;
	case 2:
		row_mod = 0;
		column_mod = 2;
		break;
	case 3:
		row_mod = 1;
		column_mod = 0;
		break;
	case 4:
		row_mod = 1;
		column_mod = 1;
		break;
	case 5:
		row_mod = 1;
		column_mod = 2;
		break;
	case 6:
		row_mod = 2;
		column_mod = 0;
		break;
	case 7:
		row_mod = 2;
		column_mod = 1;
		break;
	case 8:
		row_mod = 2;
		column_mod = 2;
		break;
	default:
		break;
	}
	
	//assign values to location.
	location.row = row_base + row_mod;
	location.column = column_base + column_mod;

	//return location
	return location;

}