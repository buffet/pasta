#ifndef PASTA_H
#define PASTA_H

#include <stdlib.h>

#ifndef PASTA_ALLOCATE
#	define PASTA_ALLOCATE malloc
#endif

#ifndef PASTA_REALLOCATE
#	define PASTA_REALLOCATE realloc
#endif

#ifndef PASTA_DEALLOCATE
#	define PASTA_DEALLOCATE free
#endif

typedef void (*free_f)(void*);

/**
 * Represents an allocated object in the current stack frame
 */
typedef struct pasta_stack_node {
	void *address;
	free_f deconstructor;
	struct pasta_stack_node *next;
} pasta_stack_node;

#define PASTA_EMPTY_STACK { NULL }

/**
 * Represents a stack frame
 */
typedef struct pasta_stack {
	pasta_stack_node *head;
} pasta_stack;

/**
 * Creates an empty stack frame. See PASTA_EMPTY_STACK
 */
static pasta_stack pasta_create(void)
{
	pasta_stack stack = PASTA_EMPTY_STACK;
	return stack;
}

/**
 * Destroys a given stack frame and all objects allocated in it
 */
static void pasta_destroy(pasta_stack *stack)
{
	pasta_stack_node *current_node = stack->head;

	for (; current_node != NULL; current_node = current_node->next) {
		current_node->deconstructor(current_node->address);
		// TODO: Deallocate node
	}
}

/**
 * Registers a given object in the current stack frame
 */
static void pasta_push(pasta_stack *stack, void *address, free_f deconstructor);

/**
 * Allocates an object in the current stack frame and registers it
 */
static void *pasta_malloc(pasta_stack *stack, size_t size)
{
	void *result = PASTA_ALLOCATE(size);
	pasta_push(stack, result, PASTA_DEALLOCATE);
	return result;
}

/**
 * Reallocates an object in the current stack frame
 */
static void pasta_realloc(pasta_stack *stack, void *address, size_t new_size)
{
	pasta_stack_node *current_node = stack->head;

	for (; current_node != NULL; current_node = current_node->next) {
		if (current_node->address == address) {
			current_node->address = PASTA_REALLOCATE(current_node->address, new_size);
			return;
		}
	}
}

/**
 * Frees an object in the current stack frame prior to ending the stack frame (think about this first)
 */
static void pasta_free(pasta_stack *stack, void *address)
{
	pasta_stack_node **node_ptr = &stack->head;
	pasta_stack_node *current_node = stack->head;

	for (; current_node != NULL; current_node = current_node->next) {
		if (current_node->address == address) {
			current_node->deconstructor(current_node);
			*node_ptr = current_node->next;
			// TODO: Deallocate node
			return;
		}
	}
}

#endif /* PASTA_H */
