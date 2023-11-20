/*
	*	Sudoku File
	*	Author: J.M.Hood
	*	Version: November 2023
	*
	*	This file drives the solution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cprogram.h"

int SUDOKU_GEN_NUM = 9;
char* FILENAME = "test2.txt";
int ITTERATION_LIMIT = 10;

// Declaration of the Sudoku's nodes 
struct Node
{
	int number;		// Value to be found or is already given
	int column[9];	// Lists possible solutions for number varible based on the column 
	int row[9];		// Lists possible solutions for number varible based on the row 
	int block[9];	// Lists possible solutions for number varible based on the block 
	int check_c;	// Value of how many numbers remain in column array
	int check_r;	// Value of how many numbers remain in row array
	int check_b;	// Value of how many numbers remain in block array
};

// Declaration of the Sudoku 
struct Sudoku_Solver
{
	struct Node node[9][9];
	int check_a;	// how many missing values the sudoku has
} sudoku_solver;


void main() {
	welcome();

	// Read File, Create Sudoku structure, and populate it with text file
	FILE* file1 = fopen(FILENAME, "r");
	struct Sudoku_Solver my_ss;
	int row = 0;
	while (!feof(file1)) {
		if (ferror(file1)) {
			printf("Reading file error.\n");
			return 1;
		}
		for (int i = 0; i < SUDOKU_GEN_NUM; i++) {
			if (fscanf(file1, "%d", &my_ss.node[row][i].number) == EOF)
				break;
		}
		row++;
		if (row == SUDOKU_GEN_NUM) break;
	}
	fclose(file1);

	// Display Sudoku
	displayer(my_ss);	
	
	// fill each column, row, block checker for solving.
	my_ss = prep_check_arrays(my_ss);
	
	// SOLVE TIME
	check_all(my_ss);
	printf("Attempting solution with %d itterations\n", ITTERATION_LIMIT);
	printf("\n------START SOLVING!-------\n");
	run_solver(my_ss);

	return 0;
}

/*
	* function: welcome
	*
	* in: -
	* out: -
	*
	* purpose: Introdues the rules of Sudoku and addresses the objective of the program.
*/
void welcome()
{
	printf("Sudoku Solver\n");
	printf("=============\n");
	printf("Welcome.  Sudoku is a puzzle played on a 9x9 grid.\n");
	printf("The grid will contain the number 1-9 with the following rules:\n");
	printf("\t 1: Numbers 1-9 will appear in each row only once\n");
	printf("\t 2: Numbers 1-9 will appear in each column only once\n");
	printf("\t 3: Numbers 1-9 will appear in each 3x3 quadrant only once\n");
	printf("This program is made to solve the submitted Sudoku.\n");
	printf("\n");
}

/*
	* function: run_solver
	*
	* in: The sudoku
	* out: -
	*
	* purpose:
	*     Run functions functions that assist in solving the sudoku.
	*	  Display updated solution and new count of the remaing missing numbers.
	*	Repeat this a number of times, as allowed.
	*   If the count of missing numbers has not changed after a loop, end loop. 
	*   Display end solution.
	*
	* informally: Run supporting solver functions until loop limit is reached or solution did not update.
*/
void run_solver(struct Sudoku_Solver ss) {
	int trial = 1;
	int previous_count = 0;

	// Preform Solving Functions unless:
		// 1. the Trial meets the itteration limit decided at the start, OR
		// 2. the number of missing values hasn't change. This can mean:
		//   2.1 the sudoku has 0 missing values remaining, thus has finished solving, OR
		//	 2.2 the sudoku has missing values remaining, but the functions cannot solve for this solution.
	while (trial < ITTERATION_LIMIT) {
		printf("\nItteration: %d\n", trial);
		
		ss = check_row(ss);
		ss = check_column(ss);
		ss = check_block(ss);
		ss = check_sol_r(ss);
		ss = check_sol_c(ss);
		ss = check_sol_b(ss);
		printf("New ");
		ss = check_all(ss);
		displayer(ss);

		if (ss.check_a == 0 || previous_count == ss.check_a)
			break;
		previous_count = ss.check_a;
		trial++;
	}

	// Display Result
	printf("\n\n ... after %d itterations...\n", trial);
	printf(" ... %d numbers missing...\n", ss.check_a);
	printf("\n------FINAL SOLUTION-------\n");
	displayer(ss);
}

/*
	* function: prep_check_arrays
	*
	* in: The sudoku
	* out: The updated sudoku
	*
	* purpose:
	*   Check every number of the sukodu
	*     if a number 0 is there, 
	*		update those checkers for a 1-9 possible solution.
	*	  if a non-0 number is there,
	*		update those checkers for no other solutions remaining.
	* 
	* informally: Prepare the Sudoku Solver assisting tools.
*/
struct Sudoku_Solver prep_check_arrays(struct Sudoku_Solver ss) {
	for (int a = 0; a < SUDOKU_GEN_NUM; a++) {
		for (int b = 0; b < SUDOKU_GEN_NUM; b++) {
			for (int c = 0; c < SUDOKU_GEN_NUM; c++) {
				if (get_num_xy(ss, a, b) == 0) {
					ss.node[a][b].row[c] = c + 1;
					ss.node[a][b].column[c] = c + 1;
					ss.node[a][b].block[c] = c + 1;
					ss.node[a][b].check_r = SUDOKU_GEN_NUM;
					ss.node[a][b].check_c = SUDOKU_GEN_NUM;
					ss.node[a][b].check_b = SUDOKU_GEN_NUM;
				}
			}
			if (get_num_xy(ss, a, b) != 0) {
				null_rcb_checker(ss, a, b);
			}
		}
	}
	ss.check_a = 81;

	return ss;
}

/*
	* function: check_row
	*
	* in: The sudoku
	* out: The updated sudoku
	*
	* purpose:
	*   Count every 0 number of the sukodu.
	*     Update the sudoku counter for how many missing numbers are remaining
	*
	* informally: Update counter for missing values in sudoku.
*/
struct Sudoku_Solver check_all(struct Sudoku_Solver ss) {
	int found_number = 0;
	int counter = 0;
	for (int a = 0; a < SUDOKU_GEN_NUM; a++) {
		for (int b = 0; b < SUDOKU_GEN_NUM; b++) {
			found_number = get_num_xy(ss, a, b);
			if (found_number == 0) {
				counter++;
			}
		}
	}
	ss.check_a = counter;
	printf("Numbers Missing Count: %d\n", ss.check_a);
	return ss;
}

/*
	* function: check_row
	*
	* in: The sudoku
	* out: The updated sudoku
	*
	* purpose:
	*   Check every number of the sukodu
	*     if a non-0 number exists there, update that coordinate's row.
	*		go through entire 9x1 row, remove that non-0 number from the arrays
	*       of possible solutions and reduce their check counts all by 1.
	*
	* informally: If the a number exists, remove it from solutions to other numbers in it's row.
*/
struct Sudoku_Solver check_row(struct Sudoku_Solver ss) {
	int found_number = 0;
	for (int a = 0; a < SUDOKU_GEN_NUM; a++) {
		for (int b = 0; b < SUDOKU_GEN_NUM; b++) {
			found_number = get_num_xy(ss, a, b);

			// If a number does exist in this spot,
			//   remove it from the relevant checkers along its row 
			if (found_number != 0) {
				for (int x = 0; x < SUDOKU_GEN_NUM; x++) {
					if (ss.node[x][b].row[found_number - 1] != 0) {
						ss.node[x][b].row[found_number - 1] = 0;
						ss.node[x][b].check_r--;
						ss.node[x][b].column[found_number - 1] = 0;
						ss.node[x][b].check_c--;
						ss.node[x][b].block[found_number - 1] = 0;
						ss.node[x][b].check_b--;
					}
				}
			}
		}
	}
	return ss;
}

/*
	* function: check_column
	*
	* in: The sudoku
	* out: The updated sudoku
	*
	* purpose:
	*   Check every number of the sukodu
	*     if a non-0 number exists there, update that coordinate's column.
	*		go through entire 1x9 column, remove that non-0 number from the arrays
	*       of possible solutions and reduce their check counts all by 1.
	*
	* informally: If the a number exists, remove it from solutions to other numbers in it's column.
*/
struct Sudoku_Solver check_column(struct Sudoku_Solver ss) {
	int found_number = 0;
	for (int a = 0; a < SUDOKU_GEN_NUM; a++) {
		for (int b = 0; b < SUDOKU_GEN_NUM; b++) {
			found_number = get_num_xy(ss, a, b);

			// If a number does exist in this spot,
			//   remove it from the relevant checkers down its column 
			if (found_number != 0) {
				for (int x = 0; x < SUDOKU_GEN_NUM; x++) {
					if (ss.node[a][x].column[found_number - 1] != 0) {
						ss.node[a][x].row[found_number - 1] = 0;
						ss.node[a][x].check_r--;
						ss.node[a][x].column[found_number - 1] = 0;
						ss.node[a][x].check_c--;
						ss.node[a][x].block[found_number - 1] = 0;
						ss.node[a][x].check_b--;
					}
				}
			}
		}
	}
	return ss;
}

/*
	* function: check_block
	*
	* in: The sudoku
	* out: The updated sudoku
	*
	* purpose:
	*   Check every number of the sukodu
	*     if a non-0 number exists there, update that coordinate's block.
	*		go through entire 3x3 block, remove that non-0 number from the arrays
	*       of possible solutions and reduce their check counts all by 1.
	*
	* informally: If the a number exists, remove it from solutions to other numbers in it's block.
*/
struct Sudoku_Solver check_block(struct Sudoku_Solver ss) {
	int found_number = 0;
	for (int a = 0; a < SUDOKU_GEN_NUM; a++) {
		for (int b = 0; b < SUDOKU_GEN_NUM; b++) {
			found_number = get_num_xy(ss, a, b);

			// If a number does exist in this spot,
			//   remove it from the relevant checkers inside its block 
			if (found_number != 0) {
				int coord_x = a / 3;
				int coord_y = b / 3;

				for (int x = 0; x < 3; x++) {
					for (int y = 0; y < 3; y++) {
						if (ss.node[coord_x * 3 + x][coord_y * 3 + y].block[found_number - 1] != 0) {
							ss.node[coord_x * 3 + x][coord_y * 3 + y].row[found_number - 1] = 0;
							ss.node[coord_x * 3 + x][coord_y * 3 + y].check_r--;
							ss.node[coord_x * 3 + x][coord_y * 3 + y].column[found_number - 1] = 0;
							ss.node[coord_x * 3 + x][coord_y * 3 + y].check_c--;
							ss.node[coord_x * 3 + x][coord_y * 3 + y].block[found_number - 1] = 0;
							ss.node[coord_x * 3 + x][coord_y * 3 + y].check_b--;
						}
					}
				}
			}
		}
	}
	return ss;
}

/*
	* function: check_sol_r
	*
	* in: The sudoku
	* out: The updated sudoku
	*
	* purpose:
	*   Check every number of the sukodu
	*     if a number 0 is there, examine it's row checker.
	*       if that row checker equal 1,
	*         it means there is only 1 number remaining in the row array,
	*         find the last remaining possible number and
	*         fill it as the main solution of that coordinate and
	*		  update all checkers involved with that number.
	*
	* informally: If the row shows only 1 possible number remaining, fill it.
*/
struct Sudoku_Solver check_sol_r(struct Sudoku_Solver ss) {
	int found_number = 0;
	int last_number = 0;
	for (int a = 0; a < SUDOKU_GEN_NUM; a++) {
		for (int b = 0; b < SUDOKU_GEN_NUM; b++) {
			found_number = get_num_xy(ss, a, b);

			// If a 0 exist in this spot, AND 
			//	 it's row checker has only 1 number possible
			if (found_number == 0 && ss.node[a][b].check_r == 1) {
				for (int x = 0; x < SUDOKU_GEN_NUM; x++) {
					// Find which number is possibly remaining
					//   and enter is as the main number for that Sudoku coordinate
					//   also clean it's row, column, and block checkers.
					if (ss.node[a][b].row[x] != 0) {
						last_number = ss.node[a][b].row[x];
						ss.node[a][b].number = last_number;
						null_rcb_checker(ss, a, b);
					}
				}

			}
		}
	}
	return ss;
}

/*
	* function: check_sol_c
	*
	* in: The sudoku
	* out: The updated sudoku
	*
	* purpose:
	*   Check every number of the sukodu
	*     if a number 0 is there, examine it's column checker.
	*       if that column checker equal 1,
	*         it means there is only 1 number remaining in the column array,
	*         find the last remaining possible number and
	*         fill it as the main solution of that coordinate and
	*		  update all checkers involved with that number.
	*
	* informally: If the column shows only 1 possible number remaining, fill it.
*/
struct Sudoku_Solver check_sol_c(struct Sudoku_Solver ss) {
	int found_number = 0;
	int last_number = 0;
	for (int a = 0; a < SUDOKU_GEN_NUM; a++) {
		for (int b = 0; b < SUDOKU_GEN_NUM; b++) {
			found_number = get_num_xy(ss, a, b);

			// If a 0 exist in this spot, AND 
			//	 it's column checker has only 1 number possible
			if (found_number == 0 && ss.node[a][b].check_c == 1) {
				for (int x = 0; x < SUDOKU_GEN_NUM; x++) {
					// Find which number is possibly remaining
					//   and enter is as the main number for that Sudoku coordinate
					//   also clean it's row, column, and block checkers.
					if (ss.node[a][b].column[x] != 0) {
						last_number = ss.node[a][b].column[x];
						set_num_xy(ss, a, b, last_number);
						null_rcb_checker(ss, a, b);
					}
				}

			}
		}
	}
	return ss;
}

/* 
	* function: check_sol_b
	* 
	* in: The sudoku
	* out: The updated sudoku 
	* 
	* purpose: 
	*   Check every number of the sukodu
	*     if a number 0 is there, examine it's block checker.
	*       if that block checker equal 1, 
	*         it means there is only 1 number remaining in the block array,
	*         find the last remaining possible number and 
	*         fill it as the main solution of that coordinate and
	*		  update all checkers involved with that number.
	*
	* informally: If the block shows only 1 possible number remaining, fill it.
*/
struct Sudoku_Solver check_sol_b(struct Sudoku_Solver ss) {
	int found_number = 0;
	int last_number = 0; 

	for (int a = 0; a < SUDOKU_GEN_NUM; a++) {
		for (int b = 0; b < SUDOKU_GEN_NUM; b++) {
			found_number = get_num_xy(ss, a, b);

			// If a 0 exist in this spot, AND 
			//	 it's block checker has only 1 number possible
			if (found_number == 0 && ss.node[a][b].check_b == 1) {
				for (int x = 0; x < SUDOKU_GEN_NUM; x++) {
					// Find which number is possibly remaining
					//   and enter is as the main number for that Sudoku coordinate
					//   also clean it's row, column, and block checkers.
					if (ss.node[a][b].block[x] != 0) {
						last_number = ss.node[a][b].block[x];
						set_num_xy(ss, a, b, last_number);
						null_rcb_checker(ss, a, b);
					}
				}

			}
		}
	}
	return ss;
}

/*
	* function: displayer
	*
	* in: The sudoku
	* out: -
	*
	* purpose:
	*   Read the sudoku input and convert to 
	*     a display that matches general sudoku use.
	*
	* informally: Display Sudoku in readable format.
*/
void displayer(struct Sudoku_Solver ss) {
	for (int x = 0; x < SUDOKU_GEN_NUM; x++) {
		if (x % 3 == 0) {
			printf("-------------\n"); // creates row dividers
		}
		for (int y = 0; y < SUDOKU_GEN_NUM; y++) {
			if (y % 3 == 0)
				printf("|"); // creates column dividers
			if ((get_num_xy(ss,x,y)) == 0)
				printf("."); // makes 0 numbers into empty segments, like a normal sudoku
			else
				printf("%d", get_num_xy(ss, x, y));
		}
		printf("|\n");
	}
	printf("-------------\n");
}

/*
	* function: set_num_xy
	*
	* in: the Sudoku, coordinate X and Y of the Sudoku node, new value
	* out: the Sudoku
	*
	* purpose: Changes the value of the sudoku number at the X and Y coordinate. 
*/
struct Sudoku_Solver set_num_xy(struct Sudoku_Solver ss, int x, int y, int set) {
	ss.node[x][y].number == set;
}

/*
	* function: get_num_xy
	*
	* in: the Sudoku, coordinate X and Y of the Sudoku node
	* out: a number
	*
	* purpose: Returns the value of the sudoku number at the X and Y coordinate.
*/
int get_num_xy(struct Sudoku_Solver ss, int x, int y) {
	return ss.node[x][y].number;
}

/*
	* function: null_rcb_checker
	*
	* in: the Sudoku, coordinate X and Y of the Sudoku node
	* out: the Sudoku
	*
	* purpose: 
	*	find the solver supporting variables of a Sudoku number at
	*   coordinate (x,y) and set them as empty. This is done typically
	*   because the number is already given or is just found.
*/
struct Sudoku_Solver null_rcb_checker(struct Sudoku_Solver ss, int x, int y) {
	ss.node[x][y].check_r = 0;
	ss.node[x][y].check_c = 0;
	ss.node[x][y].check_b = 0;
	for (int a = 0; a < SUDOKU_GEN_NUM; a++) {
		ss.node[x][y].row[a] = 0;
		ss.node[x][y].column[a] = 0;
		ss.node[x][y].block[a] = 0;
	}
	return ss;
}
