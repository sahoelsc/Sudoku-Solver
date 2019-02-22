#pragma once
#include <windows.h>

//High Level Functions
int sudoku_output();
int initialize_sudoku();
int sudoku_input(void(*display_fptr)(),void(*latest_fptr)(int, int, int, int), void(*flip_fptr)());
int sudoku_output();
void sudoku_solver(void (*function_pointer)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)());
int logical_sudoku_solver(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)());

//Sub-Functions:
int row_eliminate();
int column_eliminate();
int subcell_eliminate();
int cell_check();
int cell_assign(int row, int column, int a, void (*function_pointer)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)());
int iterate_for_eights(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)());
int solve_by_row(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)());
int solve_by_column(void(*display_fptr)(), void(*latest_fptr)(int, int, int, int), void(*flip_fptr)());
int solve_by_subcell(void(*display_fptr)(),void(*latest_fptr)(int, int, int, int), void(*flip_fptr)());
struct subcell subcell_decode();
int is_puzzle_solved();

//Get and Set Functions
int get_value();
int get_elim();
int get_count();

//callbacks for Main to control view:
void display_function();

void set_value();
void set_elim();
void set_count();

void rewind_value();
void rewind_elim();
void rewind_count();

//Ledger Functions. 
void init_ledger();
void insert_ledger_entry();
void free_ledger();
void create_value_transaction();
void create_elim_transaction();
void create_count_transaction();
void rewind_puzzle();

//Ledger get calls:
int get_most_recent_main_index();
int get_most_recent_sub_index();

//Branching function calls
void branching();
struct subcell pick_branch_cell();
int pick_cell_option();
void branch_rewind();

//Initialize Structure Cell
struct cell {
	int value;
	int eliminated[10];
	int count;
};

//intitialize structure subcell location
struct subcell {
	int row;
	int column;
};

//Timers:
typedef struct {
	LARGE_INTEGER start;
	LARGE_INTEGER stop;
} stopWatch;

void startTimer(stopWatch *timer);
void stopTimer(stopWatch *timer);
double LIToSecs(LARGE_INTEGER * L);
double getElapsedTime(stopWatch *timer);

//Allegro Functions
void init_display();
void close_display();
void set_input_complete();
void draw_grid();
float* coord_by_subcell(int cell, int subcell);
int*cell_switch(int cell);
void print_int_to_subcell(int cell, int subcell, int number, int color);
void update_display();
void flip_display();
int* row_column_to_cell_subcell(int row, int column);

//callbacks
void display_function();
void most_recent_number_displayed_in_red(int cell, int subcell, int number, int color);
void flip_callback();