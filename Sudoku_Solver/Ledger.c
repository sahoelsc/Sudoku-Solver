// Ledger.c
// This file manages the ledger for the puzzle solver.
// Some core components:
// Continuously growing dynamic list
// Main index only itereates when a set_value is performed
// sub-indices reset when a set_value is performed. 
// right now, only able to perform addition on this ledger. 
// reversion to be managed by a branching subroutine?

#include "Functions.h"
#include "stdafx.h"
#include "stdlib.h"

typedef struct {
	int main_index;
	int sub_index;
	int row;
	int column;
	int type; // 1 = value, 2 = elim, 3 = count;
	int elim_number;
	int from;
	int to;
} Ledger_t;

typedef struct{
	Ledger_t *Entry;
	size_t used;
	size_t size;

} Chain_t;

static Chain_t Ledger_item;
static Chain_t *transaction_list = &Ledger_item;
int initial_size = 20;

Ledger_t initial_entry;




void init_ledger(){

	transaction_list->Entry = (Ledger_t *)malloc(initial_size * sizeof(Ledger_t));
	transaction_list->used = 0;
	transaction_list->size = initial_size;

	initial_entry.main_index = 0;
	initial_entry.sub_index = 0;
	initial_entry.row = 0;
	initial_entry.column = 0;
	initial_entry.type = 0;
	initial_entry.elim_number = 0;
	initial_entry.from = 0;
	initial_entry.to = 0;

	insert_ledger_entry(initial_entry);
}


void insert_ledger_entry(Ledger_t next_transaction) {
	// a-> used tracks the number of entries used out of max
	// if a-> used goes past a-> size, then we allocate more memory.
	if (transaction_list->used == transaction_list->size) {
		transaction_list->size *= 2;
		transaction_list->Entry = (Ledger_t *)realloc(transaction_list->Entry, transaction_list->size * sizeof(Ledger_t));
	}

	/*
	printf("----------------------------------------")
	printf("The ledger entry being entered is: \n");
	printf("main_index = %d, sub_index =   %d \n", next_transaction.main_index, next_transaction.sub_index);
	printf("Type =       %d, Elim_number = %d \n", next_transaction.type, next_transaction.elim_number);
	printf("row =        %d, column =      %d\n", next_transaction.row, next_transaction.column);
	printf("from =       %d, to =          %d \n", next_transaction.from, next_transaction.to);
	*/

	//printf("%d \n", transaction_list->used);
	transaction_list->Entry[transaction_list->used++] = next_transaction;
}

//free the memory allocated to the ledger
void free_ledger() {
	free(transaction_list->Entry);
	transaction_list->Entry = NULL;
	transaction_list->used = transaction_list->size = 0;
}

void create_value_transaction(int row, int column, int new_value) {
	Ledger_t temp_entry;
	// create the entry to be entered, and then commit to the ledger.

	temp_entry.main_index =get_most_recent_main_index() + 1; //Next set of changes
	temp_entry.sub_index = 0; // value commits reset this
	temp_entry.row = row; 
	temp_entry.column = column;
	temp_entry.type = 1; // 1 = value, 2 = elim, 3 = count
	temp_entry.elim_number = 0; // not a valid entry for type = 1. Null seems to break it
	temp_entry.from = get_value(row, column); // what is the current value of the cell
	temp_entry.to = new_value; // this is the new value of the cell.
	/*
	printf("The ledger entry being entered is: \n");
	printf("main_index = %d, sub_index = %d \n", temp_entry.main_index, temp_entry.sub_index);
	printf("row = %d, column = %d\n", temp_entry.row, temp_entry.column);
	printf("from = %d, to = %d \n", temp_entry.from, temp_entry.to);
	*/

	printf("#%d -> putting %d in row:%d, column:%d \n", temp_entry.main_index, new_value, row, column);

	// send temp entry to the ledger:
	insert_ledger_entry(temp_entry); //

}

void create_elim_transaction(int row, int column, int elim_number,int boolean) {
	Ledger_t temp_entry;
	//create the entry to be entered

	temp_entry.main_index = get_most_recent_main_index(); //Next set of changes
	temp_entry.sub_index = get_most_recent_sub_index() +1; // value commits reset this
	temp_entry.row = row;
	temp_entry.column = column;
	temp_entry.type = 2; // 1 = value, 2 = elim, 3 = count
	temp_entry.elim_number = elim_number; 
	temp_entry.from = get_elim(row, column, elim_number); // what is the current value of the cell
	temp_entry.to = boolean; // this is the new value of the cell.

	insert_ledger_entry(temp_entry);
}

void create_count_transaction(int row, int column, int up_or_zero) {
	Ledger_t temp_entry;
	//create the entry to be entered

	temp_entry.main_index = get_most_recent_main_index(); //Next set of changes
	temp_entry.sub_index = get_most_recent_sub_index() + 1; // value commits reset this
	temp_entry.row = row;
	temp_entry.column = column;
	temp_entry.type = 3; // 1 = value, 2 = elim, 3 = count
	temp_entry.elim_number = 0;
	temp_entry.from = get_count(row,column); // what is the current value of the cell
	if (up_or_zero == 1) {
		temp_entry.to = get_count(row, column) + 1; // this is the new value of the cell.
	}
	if (up_or_zero == 0) {
		temp_entry.to = 0;
	}

	//insert ledger entry
	insert_ledger_entry(temp_entry);
	
	
}

//refining the ledger entry calls to be specific to what you're getting.
int get_most_recent_main_index() {
	//I need to be able to get the most recent ledger entry.
	//And also maybe to get any ledger entry? Though that may be a different thing
	
	return transaction_list->Entry[transaction_list->used - 1].main_index;
}

int get_most_recent_sub_index() {
	return transaction_list->Entry[transaction_list->used - 1].sub_index;
}

Ledger_t get_most_recent_ledger_entry() {
	return transaction_list->Entry[transaction_list->used - 1];
}


//to rewind the puzzle. This function interacts with the ledger
void rewind_puzzle(int goal_index) {
	//while rewinding to the main index,
	// get the row, column, and from, 
	// change that back without setting the ledger, and remove the entry. 
	int row, column;
	int from;
	int type, elim_number;
	int at_goal_index = 0;
	Ledger_t current_rewind;
	

	while (at_goal_index == 0) {
		if (get_most_recent_main_index() == goal_index) {
			at_goal_index = 1;
		}

		current_rewind = get_most_recent_ledger_entry();
		//begin setting up to remove entries.
		row = current_rewind.row;
		column = current_rewind.column;
		type = current_rewind.type;
		elim_number = current_rewind.elim_number;
		from = current_rewind.from;

		//establish what needs to be sent to the "undo entry" function
		//determine what type of change it is:
		switch (type)
		{
		case 1:
			//value
			rewind_value(row, column, from);
			break;
		case 2:
			//elim
			rewind_elim(row, column, elim_number, from);
			break;
		case 3:
			//count 
			rewind_count(row, column, from);
			break;
		default:
			break;
		}
		//iterate used.
		//need to change used in the ledger.
		transaction_list->Entry[transaction_list->used--];
	}

}