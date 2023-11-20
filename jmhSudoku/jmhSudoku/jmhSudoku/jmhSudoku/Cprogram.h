#include <stdbool.h>

struct Sudoku_Solver;
typedef struct Sudoku_Solver* Suduku;

void welcome();
void run_solver(struct Sudoku_Solver ss);
struct Sudoku_Solver check_all(struct Sudoku_Solver ss);
struct Sudoku_Solver check_row(struct Sudoku_Solver ss);
struct Sudoku_Solver check_block(struct Sudoku_Solver ss);
struct Sudoku_Solver check_column(struct Sudoku_Solver ss);
struct Sudoku_Solver check_sol_r(struct Sudoku_Solver ss);
struct Sudoku_Solver check_sol_c(struct Sudoku_Solver ss);
struct Sudoku_Solver check_sol_b(struct Sudoku_Solver ss);
struct Sudoku_Solver check_number_sudoku(struct Sudoku_Solver ss);
struct Sudoku_Solver prep_check_arrays(struct Sudoku_Solver ss);
void displayer(struct Sudoku_Solver ss);
struct Sudoku_Solver set_num_xy(struct Sudoku_Solver ss, int x, int y, int set);
int get_num_xy(struct Sudoku_Solver ss, int x, int y);
struct Sudoku_Solver null_rcb_checker(struct Sudoku_Solver ss, int x, int y);
bool int_array_match(int a1[], int a2[]);
int* get_distinct(int a1[]);
struct Sudoku_Solver isolate_rows(struct Sudoku_Solver ss);
struct Sudoku_Solver isolate_columns(struct Sudoku_Solver ss);
struct Sudoku_Solver isolate_block(struct Sudoku_Solver ss);