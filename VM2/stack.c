//
//  stack.c
//  VM2
//
//  Created by Daniel Fagnan on 2012-12-17.
//  Copyright (c) 2012 Daniel Fagnan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "call.h"
#include "registers.h"


void initArray(CArray *a, size_t initialSize) {
    a->array = (struct call *)malloc(initialSize * sizeof(struct call));
    a->used = 0;
    a->size = initialSize;
}

void insertArray(CArray *a, struct call element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array = (struct call *)realloc(a->array, a->size * sizeof(struct call));
    }
    a->array[a->used++] = element;
}

void freeArray(CArray *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

stack* create_stack(struct VirtualMachine* vm) {
    
    printf("** Created a new stack. **\n");
    
    // Determine the overal size of the `stack` struct,
    // initial call struct array, etc...
    int size = 0;
    
    size += sizeof(call) * 1; // Start with a default of 1 item;
    
    stack* local_stack = (stack*)malloc(sizeof(stack) + size);
    
    // Initialize memory allocation for `items` call array:
    initArray(&local_stack->calls, 5);
    
    printf("\nCall Size: %li \nCall Array Size: %li\n", sizeof(CArray), sizeof(local_stack->calls));
    
    return local_stack;
}

// Add another call item on the top of the stack:
void push_stack(struct VirtualMachine* vm_instance) {
    
}

void pop_stack(struct VirtualMachine* vm_instance) {
    
}

int delete_stack(struct VirtualMachine* vm_instance) {
    
    return 0;
}