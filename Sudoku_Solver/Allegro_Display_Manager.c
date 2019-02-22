//Allegro_Display_Manager.c
//This file holds the main process management of the
//Sudoku display using allegro.

#include "stdio.h"
#include "Functions.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_primitives.h"


static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_COLOR black;
static ALLEGRO_COLOR white;
static ALLEGRO_COLOR grey;
static ALLEGRO_COLOR red;
static ALLEGRO_FONT *font_large;
static ALLEGRO_FONT *font_small;
static float height = 480*2;
static float width = 640*2;
static int large_height_offset;
static int small_height_offset;
static bool input_complete = 0;

void init_display()
{
	
	al_init(); //Initialize allegro
	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
	al_init_primitives_addon(); //initialize the primatives addon

	//Create a display object

	display = al_create_display(width, height);
	black = al_map_rgb(0, 0, 0);
	white = al_map_rgb(255, 255, 255);
	red = al_map_rgb(140, 0, 0);
	grey = al_map_rgb(130, 130, 130);

	if (!display) {
		fprintf(stderr, "Failed to create display.\n");
	}

	font_large = al_load_ttf_font("C:/Users/Sean Hoelscher/Desktop/sudokuResources/arial.ttf", 72, 0);
	font_small = al_load_ttf_font("C:/Users/Sean Hoelscher/Desktop/sudokuResources/arial.ttf", 20, 0);

	large_height_offset = al_get_font_line_height(font_large) / 2;
	small_height_offset = al_get_font_line_height(font_small) / 2;
	
	//TODO:
	//Figure out portability to avoid having to use an absolute path here

	if (!font_large) {
		fprintf(stderr, "Could not load 'arial.ttf'.\n");
	}

	al_clear_to_color(white);
	draw_grid();
	/*int *cell_array;
	for (int row = 0; row < 9; row++) {
		for (int column = 0; column < 9; column++) {
			cell_array = row_column_to_cell_subcell(row, column);
			print_int_to_subcell(cell_array[0], cell_array[1], row, 1);
		}
	}*/

	al_flip_display();

}

void close_display()
{
	//al_rest(5);
	al_destroy_display(display);
}

void draw_grid()
{
	//This function draws the grid onto the buffer bitmap
	//Will want to size the grid correctly so that each box is
	//proportional to the size of the numbers in ariel (eventually?)
	//

	//Allegro uses a coordinate system that makse the upper left corner 0,0
	//and then counds up towards the rest of the corners.

	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

	//Set up vertices based on width and height
	float upper_left_x = (width / 2 - (5 * height / 12));
	float upper_left_y = (height / 12);

	float lower_right_x = (width / 2 + (5 * height / 12));
	float lower_right_y = (11 * height / 12);

	float horz_line_start_x = upper_left_x;
	float horz_line_end_x = lower_right_x;
	float horz_line_one_y = (upper_left_y + (5 * height / 6) / 3);
	float horz_line_two_y = (upper_left_y + (10 * height / 6) / 3);

	float vert_line_start_y = upper_left_y;
	float vert_line_end_y = lower_right_y;
	float vert_line_one_x = (upper_left_x + (5 * height / 6) / 3);
	float ver_line_two_x = (upper_left_x + (10 * height / 6) / 3);

	//Draw the box (I just found, al_draw_rectangle)
	al_draw_rectangle(upper_left_x, upper_left_y, 
		lower_right_x, lower_right_y, black, 4);

	//Draw the Main lines through the grid
	al_draw_line(horz_line_start_x, horz_line_one_y,
		horz_line_end_x, horz_line_one_y, black, 4);
	al_draw_line(horz_line_start_x, horz_line_two_y,
		horz_line_end_x, horz_line_two_y, black, 4);
	al_draw_line(vert_line_one_x, vert_line_start_y,
		vert_line_one_x, vert_line_end_y, black, 4);
	al_draw_line(ver_line_two_x, vert_line_start_y,
		ver_line_two_x, vert_line_end_y, black, 4);

	//Next we need to draw the non-main grid lines.
	//We can continue to use the main grid start and end
	//What if we drew rectangles instead of lines?

	float subinterval = (10 * height / 12) / 9;
	//horizontal lines
	for (int i = 1; i < 9; ++i) {
		if (i % 3 != 0) {
			al_draw_line(horz_line_start_x, (upper_left_y + i * subinterval),
				horz_line_end_x, (upper_left_y + i * subinterval), black, 2);
		}
	}
	
	//vertical lines
	for (int i = 1; i < 9; ++i) {
		if (i % 3 != 0) {
			al_draw_line((upper_left_x + i * subinterval), vert_line_start_y,
				(upper_left_x + i * subinterval), vert_line_end_y, black, 2);
		}
	}

}

float* coord_by_subcell(int cell, int subcell) {
	//Set up delta variables
	float delta_one = (5 * height / 6) / 3;
	float delta_two = (delta_one / 3);
	float coord[2] = { width / 2, height / 2 };
	int* mod_array;

	//get the first mod array
	mod_array = cell_switch(cell);
	coord[0] = coord[0] + mod_array[0] * delta_one;
	coord[1] = coord[1] + mod_array[1] * delta_one;

	//subcell mod array:
	mod_array = cell_switch(subcell);
	coord[0] = coord[0] + mod_array[0] * delta_two;
	coord[1] = coord[1] + mod_array[1] * delta_two;

	//printf("cell = %d, x = %d, y = %d \n", cell, mod_array[0], mod_array[1]);

	return &coord;

}

int* cell_switch(int cell) {
	//function to return an array that give the -/+ for x,y by cell
	int mod_x, mod_y;
	int mod_array[2];
	switch (cell) {
	case 0:
		mod_x = -1;
		mod_y = -1;
		break;
	case 1:
		mod_x = 0;
		mod_y = -1;
		break;
	case 2:
		mod_x = 1;
		mod_y = -1;
		break;
	case 3:
		mod_x = -1;
		mod_y = 0;
		break;
	case 4:
		mod_x = 0;
		mod_y = 0;
		break;
	case 5:
		mod_x = 1;
		mod_y = 0;
		break;
	case 6:
		mod_x = -1;
		mod_y = 1;
		break;
	case 7:
		mod_x = 0;
		mod_y = 1;
		break;
	case 8:
		mod_x = 1;
		mod_y = 1;
		break;
	default:
		break;
	}
	mod_array[0] = mod_x;
	mod_array[1] = mod_y;
	
	return &mod_array;
}

void print_int_to_subcell(int cell, int subcell, int number,int color) {
	//This function accepts a location in the grid, and it's contents
	//and prints that int in black at that location. 
	float* coord = coord_by_subcell(cell, subcell);
	char* to_print;
	switch (number) {
	case 1:
		to_print = "1";
		break;
	case 2:
		to_print = "2";
		break;
	case 3:
		to_print = "3";
		break;
	case 4:
		to_print = "4";
		break;
	case 5:
		to_print = "5";
		break;
	case 6:
		to_print = "6";
		break;
	case 7:
		to_print = "7";
		break;
	case 8:
		to_print = "8";
		break;
	case 9:
		to_print = "9";
		break;
	default:
		break;
	}
	if (number != 0) {
		switch (color) {
		case 0: //black
			al_draw_text(font_large, black, coord[0], coord[1] - large_height_offset,
				ALLEGRO_ALIGN_CENTER, to_print);
			break;
		case 1: //red
			al_draw_text(font_large, red, coord[0], coord[1] - large_height_offset,
				ALLEGRO_ALIGN_CENTER, to_print);
			break;
		case 2: // grey
			al_draw_text(font_large, grey, coord[0], coord[1] - large_height_offset,
				ALLEGRO_ALIGN_CENTER, to_print);
				break;
		default:
			al_draw_text(font_large, black, coord[0], coord[1] - large_height_offset,
				ALLEGRO_ALIGN_CENTER, to_print);
			break;
		}
	}
	
}

void update_display() {
	//This function updates what is currently displayed. 
	//Clear the display
	al_clear_to_color(white);
	if (input_complete == true) {
		//then draw the grid
		draw_grid();
		//display the values currently known

		for (int cell = 0; cell < 9; ++cell) {
			for (int subcell = 0; subcell < 9; ++subcell) {
				//translate into row and column
				struct subcell location = subcell_decode(cell, subcell);
				if (get_value(location.row, location.column) != 0) {
					print_int_to_subcell(cell, subcell, get_value(location.row, location.column), 0);
				}
			}
		}
	}
}

void flip_display() {
	//so other places in the code can flip the display
	al_flip_display();
	if(input_complete == true) al_rest(.5);
}

int* row_column_to_cell_subcell(int row, int column) {
	//accepts a row and column location, and returns a cell/subcell location	
	static int cell_array[2];
	cell_array[0] = (row / 3)*3 + (column / 3);
	cell_array[1] = (row % 3) * 3 + (column % 3);
	
	return &cell_array;
}

void set_input_complete() {
	input_complete = true;
}