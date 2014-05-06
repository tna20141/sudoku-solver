#include <stdio.h>
#include <stdlib.h>
#include "guess_stack.c"
#include "square_list.c"

int main() {
	struct guess_stack stack;
	struct guess_info info1;
	struct guess_info info2;
	struct square_list list;
	struct square s1;

	info1.guessed_num = 2;
	stack_init(&stack);
	push(&stack, info1);
	info1.guessed_num = 3;
	push(&stack, info1);
	printf("2=%d\n", stack_get_count(stack));
	info2 = pop(&stack);
	printf("1=%d\n3=%d\n", stack_get_count(stack), info2.guessed_num);

	list_init(&list);
	s1.poss_count = 4;
	append(&list, s1);
	s1.poss_count = 5;
	append(&list, s1);
	printf("2=%d\n", list_get_count(list));
	s1.poss_count = 6;
	append(&list, s1);
	printf("3=%d\n", list_get_count(list));
	printf("6=%d\n", list.head->poss_count);
	printf("5=%d\n", list.head->right->poss_count);
	printf("4=%d\n", list.head->right->right->poss_count);
	printf("0=%ld\n", (long)list.head->right->right->right);
	printf("0=%ld\n", (long)list.head->left);
	delete(&list, list.head->right);
	printf("2=%d\n", list_get_count(list));
	printf("6=%d\n", list.head->poss_count);
	printf("4=%d\n", list.head->right->poss_count);
	printf("0=%ld\n", (long)list.head->right->right);
	delete(&list, list.head);
	printf("4=%d\n", list.head->poss_count);
	printf("0=%ld\n", (long)list.head->right);
	printf("0=%ld\n", (long)list.head->left);


	return 0;
}