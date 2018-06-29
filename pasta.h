#ifndef PASTA_H
#define PASTA_H

#include <stdlib.h>

#ifndef PASTA_ALLOCATOR
#	define PASTA_ALLOCATOR malloc
#endif

#ifndef PASTA_REALLOCATOR
#	define PASTA_REALLOCATOR realloc
#endif

#ifndef PASTA_DEALLOCATOR
#	define PASTA_DEALLOCATOR free
#endif

/**
 * Represents an allocated object in the current stack frame
 */
typedef struct pasta_stack_node {
	void *address;
	void *deconstructor;
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
static pasta_stack pasta_create(void);

/**
 * Destroys a given stack frame and all objects allocated in it
 */
static void pasta_destroy(pasta_stack *stack);

/**
 * Registers a given object in the current stack frame
 */
static void pasta_push(pasta_stack *stack, void *address, void *dealloc);

/**
 * Allocates an object in the current stack frame and registers it
 */
static void *pasta_malloc(pasta_stack *stack, size_t size);

/**
 * Reallocates an object in the current stack frame
 */
static void pasta_realloc(pasta_stack *stack, void *address, size_t new_size);

/**
 * Frees an object in the current stack frame prior to ending the stack frame (think about this first)
 */
static void pasta_free(pasta_stack *dtack, void *address);

#endif /* PASTA_H */
