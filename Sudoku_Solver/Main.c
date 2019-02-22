// Sudoku_Solver.c : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Functions.h"
#include "allegro5/allegro.h"

int main()
{
	/*
	double execution_time;
	stopWatch elapsed;
	//Timer Start;
	startTimer(&elapsed); */

	//function pointer test bed
	void (*pointer_to_main_display_function)() = &display_function;
	void (*pointer_to_most_recent_number_function)(int, int, int, int) = &most_recent_number_displayed_in_red;
	void (*pointer_to_display_flip)() = &flip_callback;
	//void (*pointer_to_display_most_recent_item)()

	// initialize The puzzle array:
	initialize_sudoku();

	//initialize the display
	init_display();

	//call an input function to import a 9x9 grid into the program
	sudoku_input(pointer_to_main_display_function, pointer_to_most_recent_number_function, pointer_to_display_flip);

	//call a logical function to solve the 100% parts of the problem
	sudoku_solver( pointer_to_main_display_function, pointer_to_most_recent_number_function, pointer_to_display_flip);

	//call an output function to print the solved puzzle
	sudoku_output();
	update_display();
	flip_display();

	/*stopTimer(&elapsed);

	execution_time = getElapsedTime(&elapsed);
	printf("%g \n", execution_time);*/

	close_display();

	return 0;
}

void display_function() {
	//call the update display function located in the View section of the program
	update_display();
}

void most_recent_number_displayed_in_red(int cell, int subcell, int number, int color) {
	//call print int to subcell
	print_int_to_subcell(cell, subcell, number, color);
}

void flip_callback() {
	//call the flipping funciton
	flip_display();
}