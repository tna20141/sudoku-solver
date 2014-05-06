
/*
 * This module implements a LIFO stack, whose elements contain info about a guessed square.
 * It is used to remember the guessed information so the sudoku matrix's state can be rolled back
 */

#ifndef GUESS_STACK_C
#define GUESS_STACK_C

/* 
 * Include files
 */

#include <stdlib.h>

/*
 * Struct definitions
 */ 

/* guess info struct, contains info a about a guessed square */
struct guess_info {
	/* the number that is guessed fo fill in the square */
	int guessed_num;
	/* pointer to the square from the square list that is guessed */
	struct square* square_ptr;
	/* a boolean attribute, indicates that the square is guessed or not */
	int guessed;
};

/* guess element struct, wraps the guess_info struct, serves as elements of the guess stack */
struct guess_element {
	/* guess info struct */
	struct guess_info info;
	/* pointer to the next element, since the stack in a linked list */
	struct guess_element* next;
};


/* guess stack struct */
struct guess_stack {
	/* pointer to the top of the stack */
	struct guess_element* top_ptr;
	/* number of elements in stack */
	int count;
	/* the number of guesses made */
	int guess_level;
};

/*
 * Function prototypes
 */

void 				stack_init(struct guess_stack*);
void 				push(struct guess_stack*, struct guess_info);
struct guess_info 	pop(struct guess_stack*);
void				destroy_stack(struct guess_stack*);

/*
 * Function definitions
 */

/* initialize the guess stack */
 void stack_init(struct guess_stack* stack) {
 	stack->top_ptr = NULL;
 	stack->count = 0;
 	stack->guess_level = 0;
 }

 /* push an element into the stack */
 void push(struct guess_stack* stack, struct guess_info info) {
 	struct guess_element* new;

 	new = (struct guess_element*)malloc(sizeof(struct guess_element));
 	new->info = info;
 	new->next = stack->top_ptr;
 	stack->top_ptr = new;
 	stack->count++;
 }

 /* pop an element out of the stack and return it */
 struct guess_info pop(struct guess_stack* stack) {
 	struct guess_info returned_info;
 	struct guess_element* old_top_ptr;

 	returned_info = stack->top_ptr->info;
 	old_top_ptr = stack->top_ptr;
 	stack->top_ptr = stack->top_ptr->next;
 	stack->count--;
 	free(old_top_ptr);
 	return returned_info;
 }

/* free memory allocated to the stack */
void destroy_stack(struct guess_stack* stack) {
	int i;

	for (i = 0; i < stack->count; i++)
		pop(stack);
}

#endif