#include "cstack.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

struct node {
    struct node* prev;
    unsigned int size;
    void* data;
};
struct stack_list {
    struct stack_list* prev;
    struct stack_list* next;
    int descriptor;
    struct node* stack_root;
};
struct stack_list list_root = { NULL, NULL, 0, NULL };


hstack_t stack_new(void)
{
	struct stack_list* new_root = (struct stack_list*)malloc(sizeof(struct stack_list));
	if (new_root) {
		
		struct stack_list* temp;
		temp = &list_root;
		unsigned int free_disc = 0;
		unsigned int disc = 0;
		while (temp->next != NULL) {
			temp = temp->next;
			if (stack_valid_handler(disc) && free_disc == 0) {
				free_disc = 1;
			}
			else if (free_disc != 1) {
				disc++;
			}
		}
		new_root->next = temp->next;
		new_root->prev = temp;
		temp->next = new_root;
		new_root->descriptor = disc;
		new_root->stack_root = (struct node*)malloc(sizeof(struct node));
		if (new_root->stack_root) {
			
			new_root->stack_root->prev = NULL;
			new_root->stack_root->data = NULL;
			new_root->stack_root->size = 0;
			
			return new_root->descriptor;
		}
		else {
			
			return -1;
		}
	}
	else {
		
		return -1;
	}
}

void stack_free(const hstack_t hstack)
{
	if (!stack_valid_handler(hstack))
	{

		struct stack_list* temp;
		temp = &list_root;
		temp = temp->next;
		while (temp->descriptor != hstack) {
			temp = temp->next;
		}
		while (stack_size(hstack) != 0) {
			struct node* stack_r = temp->stack_root;
			while (stack_r->prev != NULL) {
				stack_r = stack_r->prev;
			}
			struct node* last_node = temp->stack_root;
			while (last_node->prev != stack_r) {
				last_node = last_node->prev;
			}
			last_node->prev = stack_r->prev;
			free(stack_r->data);
			free(stack_r);
		}
		free(temp->stack_root);
		struct stack_list* prevtemp = temp->prev;
		if (temp->next != NULL) {
			struct stack_list* fortemp = temp->next;
			prevtemp->next = fortemp;
			fortemp->prev = prevtemp;
		}
		else {
			prevtemp->next = temp->next;
		}
		free(temp);
	}
}

int stack_valid_handler(const hstack_t hstack)
{
	struct stack_list* temp = &list_root;
	int valid_handler = 1;
	while (temp->next != NULL) {
		temp = temp->next;
		if (hstack == temp->descriptor) {
			valid_handler = 0;
		}
	}
	return valid_handler;
}

unsigned int stack_size(const hstack_t hstack)
{
	if (!stack_valid_handler(hstack))
	{
		struct stack_list* temp;
		temp = &list_root;
		temp = temp->next;
		while (temp->descriptor != hstack) {
			temp = temp->next;
		}
		struct node* stack_r = temp->stack_root;
		unsigned int size = 0;
		while (stack_r->prev != NULL) {
			stack_r = stack_r->prev;
			size++;
		}
		return size;
	}
	else {
		return 0;
	}
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
	if (!stack_valid_handler(hstack) && data_in && size > 0) {
		struct stack_list* temp;
		temp = &list_root;
		temp = temp->next;
		while (temp->descriptor != hstack) {
			temp = temp->next;
		}
		struct node* stack_r = temp->stack_root;
		while (stack_r->prev != NULL) {
			stack_r = stack_r->prev;
		}
		struct node* new_node = (struct node*)malloc(sizeof(struct node));
		if (new_node) {
			new_node->prev = stack_r->prev;
			new_node->size = size;
			new_node->data = malloc(size);
			stack_r->prev = new_node;
			if (new_node->data) {
				memcpy(new_node->data, data_in, size);
			}
		}

	}

}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
	if (!stack_valid_handler(hstack) && stack_size(hstack) != 0) {
		struct stack_list* temp;
		temp = &list_root;
		temp = temp->next;
		while (temp->descriptor != hstack) {
			temp = temp->next;
		}
		struct node* stack_r = temp->stack_root;
		while (stack_r->prev != NULL) {
			stack_r = stack_r->prev;
		}
		if (stack_r->size <= size && data_out != NULL) {
			memcpy(data_out, stack_r->data, stack_r->size);
			unsigned int result;
			result = stack_r->size;
			struct node* prev_stack = temp->stack_root;
			while (prev_stack->prev != stack_r) {
				prev_stack = prev_stack->prev;
			}
			prev_stack->prev = stack_r->prev;
			free(stack_r->data);
			free(stack_r);
			return result;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}
