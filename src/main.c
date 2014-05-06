
/*
 * Sudoku Solver v1.0
 * Author: TNA
 * Description: solve a sudoku problem input from keyboard or file
 */

/*
 * Include files
 */

#include <stdio.h>
#include <stdlib.h>
#include "guess_stack.c"
#include "square_list.c"

/*
 * Global variables
 */

/* the sudoku 9x9 matrix */
int sudoku_matrix[9][9];

/* the guess stack */
struct guess_stack stack;

/* the square list */
struct square_list list;

/* whether we should traverse again or not */
int traverse_again;

/* whether we should put the next fill in stack or not */
int put_in_stack;

/*
 * Function prototypes
 */

void init();
void input_matrix();
void input_matrix_from_file(const char*);
int  check_input();
void output_result(int);
void build_square_list();
void build_square_poss(struct square*, int, int);
void refresh_list();
void traverse();
void find_smallest_number_of_poss_square();
void guess(struct square*);
int  check(int, int, int);
void clean_up();

/*
 * Function definitions
 */

/* main function */
int main(int argc, char** argv) {
	init();
	if (argc > 1)
		input_matrix_from_file(argv[1]);
	else
		input_matrix();
	if (!check_input()) {
		printf("Invalid input!\n");
		clean_up();
		exit(0);
	}
	build_square_list();
	while (1) {
		if (traverse_again == 1)
			traverse();
		else {
			find_smallest_number_of_poss_square();
			guess(list.smallest);
		}
	}
	return 0;
}

/* initialize the program's components */
void init() {
	int x, y;

	for (x = 0; x < 9; x++)
		for (y = 0; y < 9; y++)
			sudoku_matrix[x][y] = 0;
	stack_init(&stack);
	list_init(&list);
	traverse_again = 0;
	put_in_stack = 0;
}

/* input the initial sudoku matrix from keyboard */
void input_matrix() {
	int x, y, i, n, v;

	printf("Input the number of squares: ");
	scanf("%d", &n);
	for (i = 0; i < n; i++) {
		printf("Square %d:\n", i+1);
		printf("Input vertical postiion: ");
		scanf("%d", &x);
		printf("Input horizontal position: ");
		scanf("%d", &y);
		printf("Input number: ");
		scanf("%d", &v);
		sudoku_matrix[x-1][y-1] = v;
	}
}

void input_matrix_from_file(const char* filename) {
	FILE* f;
	int i, j, temp;

	f = fopen(filename, "rb");
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++) {
			fscanf(f, "%d", &temp);
			sudoku_matrix[i][j] = temp;
		}
	fclose(f);
}

/* check if the input sudoku matrix is valid */
int check_input() {
	int x, y, temp;

	for (x = 0; x < 9; x++)
		for (y = 0; y < 9; y++)
			if (sudoku_matrix[x][y] != 0) {
				temp = sudoku_matrix[x][y];
				sudoku_matrix[x][y] = 0;
				if (!check(x, y, temp)) {
					sudoku_matrix[x][y] = temp;
					return 0;
				}
				sudoku_matrix[x][y] = temp;
			}
	return 1;
}

/* output the result to screen */
void output_result(int solved) {
	int x, y;

	/* if the problem has no solution */
	if (solved == 0) {
		printf("The sudoku problem can't be solved\n");
	}
	/* else, print the solved sudoku matrix */
	else {
		printf("Result:\n");
		for (x = 0; x < 9; x++) {
			for (y = 0; y < 9; y++)
				printf("%d ", sudoku_matrix[x][y]);
			printf("\n");
		}
	}
}

/* build the square list based on the initial sudoku matrix */
void build_square_list() {
	int x, y, i;
	struct square s;

	for (x = 0; x < 9; x++)
		for (y = 0; y < 9; y++)
			/* if the square needs to be solved */
			if (sudoku_matrix[x][y] == 0) {
				/* build square's possibilities */
				for (i = 0; i < 9; i++) {
					s.invalid[i][0] = 0;
					s.invalid[i][1] = 0;
				}
				build_square_poss(&s, x, y);
				/* if there's only 1 possibility, that's the answer */
				if (s.poss_count == 1) {
					sudoku_matrix[x][y] = s.poss[0];
					traverse_again = 1;
				}
				/* if there's no possibility, the problem is unsolvable */
				else if (s.poss_count == 0) {
					output_result(0);
					clean_up();
					exit(0);
				}
				/* if the number of possibilities >= 2, put the square in the list */
				else {
					append(&list, s);
				}
			}
}

void build_square_poss(struct square* s, int x, int y) {
	int i, j;
	int temp;
	int valid[9];

	s->coord.x = x;
	s->coord.y = y;
	s->poss_count = 0;
	for(i = 0; i < 9; i++)
		valid[i] = 1;
	/* check every line, row and 3x3 matrix */
	for (i = 0; i < 9; i++) {
		temp = sudoku_matrix[x][i];
		if (temp != 0)
			valid[temp-1] = 0;
		temp = sudoku_matrix[i][y];
		if (temp != 0)
			valid[temp-1] = 0;
	}
	for (i = x-x%3; i < x-x%3+3; i++)
		for (j = y-y%3; j < y-y%3+3; j++) {
			temp = sudoku_matrix[i][j];
			if (temp != 0)
				valid[temp-1] = 0;
		}
	/* remove the possible numbers that are in the invalid list */
	for (i = 0; i < 9; i++) {
		if (s->invalid[i][0] == 1) {
			if (s->invalid[i][1] < stack.guess_level+2)
				valid[i] = 0;
			else {
				s->invalid[i][0] = 0;
				s->invalid[i][1] = 0;
			}
		}
	}
	/* build square poss */
	for (i = 0; i < 9; i++)
		if (valid[i]) {
			s->poss[s->poss_count] = i+1;
			s->poss_count++;
		}
}

/* refresh the square list based on the current sudoku matrix */
void refresh_list() {
	struct square* itr;

	itr = list.head;
	while (itr != NULL) {
		build_square_poss(itr, itr->coord.x, itr->coord.y);
		itr = itr->right;
	}
}

/* traverse through the square list for any solvable square */
void traverse() {
	struct square* itr;
	struct guess_info info;
	int i, j;
	
	traverse_again = 0;
	itr = list.head;
	while (itr != NULL) {
		/* for every square's possibility */
		for (i = 0; i < itr->poss_count; i++) {
			/* if it's not valid anymore, remove it */
			if (!check(itr->coord.x, itr->coord.y, itr->poss[i])) {
				itr->poss_count--;
				for (j = i; j < itr->poss_count; j++)
					itr->poss[j] = itr->poss[j+1];
				i--;
			}
		}
		/* if there's only 1 possibility, that's the answer */
		if (itr->poss_count == 1) {
			sudoku_matrix[itr->coord.x][itr->coord.y] = itr->poss[0];
			traverse_again = 1;
			/* put the guess info in the stack if need to,
			   then remove the square from the list */
			if (put_in_stack == 1) {
				info.guessed_num = itr->poss[0];
				info.square_ptr = itr;
				info.guessed = 0;
				push(&stack, info);
				delete(&list, itr);
			}
			else {
				hard_delete(&list, itr);
			}
			/* if there's no more square to solve, the process is complete */
			if (list.head == NULL) {
				clean_up();
				printf("Done! Here's the result:\n");
				output_result(1);
				exit(0);
			}
			/* start all over */
			return;
		}
		/* if there's no possibility */
		else if (itr->poss_count == 0) {
			/* if no guess has been made, it means the problem is unsolvable */
			if (put_in_stack == 0) {
				output_result(0);
				clean_up(0);
				exit(0);
			}
			/* else, the last guess is wrong, undo it */
			else {
				printf("[DEBUG] undo\n");
				traverse_again = 1;
				do {
					info = pop(&stack);
					sudoku_matrix[
						info.square_ptr->coord.x
					][
						info.square_ptr->coord.y
					] = 0;
					attach(&list, info.square_ptr);
				}
				while (info.guessed == 0);
				/* put the guessed number to the invalid list */
				info.square_ptr->invalid[info.guessed_num-1][0] = 1;
				info.square_ptr->invalid[info.guessed_num-1][1] = stack.guess_level;
				stack.guess_level--;
				/* refresh list state */
				refresh_list(&list);
				/* if the stack is empty, all guesses are undone */
				if (stack.count == 0)
					put_in_stack = 0;
				/* start all over */
				return;
			}
		}
		/* continue traversing */
		itr = itr->right;
	}
}

/* find from the list a square with the smallest number of possibilities */
void find_smallest_number_of_poss_square() {
	struct square* itr;

	list.smallest_poss_count = 10;
	itr = list.head;
	do {
		if (list.smallest_poss_count > itr->poss_count) {
			list.smallest_poss_count = itr->poss_count;
			list.smallest = itr;
		}
		itr = itr->right;
	}
	while (itr != NULL);
	printf("[DEBUG] smallest poss count: %d\n", list.smallest_poss_count);
}

/* guess a number to fill in from the square's possibilities list */
void guess(struct square* s) {
	int guessed_num;
	struct guess_info info;

	/* guessed number is the 1st one in the possibilities list */
	guessed_num = s->poss[0];
	printf(
		"[DEBUG] guessed_num: %d, x: %d, y: %d, guess level: %d\n",
		guessed_num,
		s->coord.x,
		s->coord.y,
		stack.guess_level
	);
	sudoku_matrix[s->coord.x][s->coord.y] = guessed_num;
	/* push guest info to stack */
	info.guessed_num = guessed_num;
	info.square_ptr = s;
	info.guessed = 1;
	push(&stack, info);
	stack.guess_level++;
	/* remove the guessed square from list */
	delete(&list, s);
	/* set put_in_stack to 1 */
	put_in_stack = 1;
	/* set traverse_again to 1 */
	traverse_again = 1;
}

/* check if a number can be filled in a certain square */
int check(int x, int y, int number) {
	int i, j;

	for (i = 0; i < 9; i++)
		if (sudoku_matrix[x][i] == number)
			return 0;
	for (i = 0; i < 9; i++)
		if (sudoku_matrix[i][y] == number)
			return 0;
	for (i = x-x%3; i < x-x%3+3; i++)
		for (j = y-y%3; j < y-y%3+3; j++)
			if (sudoku_matrix[i][j] == number)
				return 0;
	return 1;
}

/* clean up before exit */
void clean_up() {
	destroy_stack(&stack);
	destroy_list(&list);
}
