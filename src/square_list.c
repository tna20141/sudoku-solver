
/*
 * This module implements a linked list of remaining squares to be solved in a sudoku problem
 */

#ifndef SQUARE_LIST_C
#define SQUARE_LIST_C

/*
 *Include files
 */

#include <stdlib.h>

/*
 * Struct definitions
 */

/* struct coordinate of a square in the sudoku matrix */
struct coordinate {
	/* position on vertical axis */
	int x;
	/* position on horizontal axis */
	int y;
};

/* square struct, contains info about the possibilities of solving the square */
struct square {
	/* list of numbers that can be filled into the square */
	int poss[9];
	/* number of elements in the above list */
	int poss_count;
	/* list of numbers wrongly guessed and their guess level */
	int invalid[9][2];
	/* coordinate of the square */
	struct coordinate coord;
	/* pointers to the adjacent squares in the list. Note that the list is a doubly linked */
	struct square* left;
	struct square* right;
};

/* the linked list of squares to be solved */
struct square_list {
	/* pointer to the head of the list */
	struct square* head;
	/* pointer to the square with the smallest possibilities count */
	struct square* smallest;
	/* the current smallest possibilities count */
	int smallest_poss_count;
	/* number of squares in the list */
	int count;
};

/*
 * Function prototypes
 */

void list_init(struct square_list*);
void append(struct square_list*, struct square);
void delete(struct square_list*, struct square*);
void hard_delete(struct square_list*, struct square*);
void destroy_list(struct square_list*);

/*
 * Function definitions
 */

/* initialize the square list */
void list_init(struct square_list* list) {
	list->head = NULL;
	list->smallest = NULL;
	list->smallest_poss_count = 10;
	list->count = 0;
}

/* append a square to the head of the list */
void append(struct square_list* list, struct square s) {
	struct square* new;

	new = (struct square*)malloc(sizeof(struct square));
	*new = s;
	new->right = list->head;
	new->left = NULL;
	if (list->head != NULL)
		list->head->left = new;
	list->head = new;
	list->count++;
 }

/* attach a memory-allocated square to the head of the list */
void attach(struct square_list* list, struct square* s) {
	s->right = list->head;
	s->left = NULL;
	if (list->head != NULL)
		list->head->left = s;
	list->head = s;
	list->count++;
}

/* remove a square from the list */
void delete(struct square_list* list, struct square* s_ptr) {
	if (s_ptr->left != NULL)
		s_ptr->left->right = s_ptr->right;
	if (s_ptr->right != NULL)
		s_ptr->right->left = s_ptr->left;
	if (list->head == s_ptr)
		list->head = s_ptr->right;
	list->count--;
}

/* remove a square from the list and also deallocate the square's memory */
void hard_delete(struct square_list* list, struct square* s_ptr) {
	delete(list, s_ptr);
	free(s_ptr);
}

/* free memory allocated to the list */
void destroy_list(struct square_list* list) {
	int i;

	for (i = 0; i < list->count; i++)
		hard_delete(list, list->head);
}

#endif