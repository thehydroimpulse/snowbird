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


stack* create_stack(struct VirtualMachine* vm) {
    
    printf("** Created a new stack. **\n");
    
    // Determine the overal size of the `stack` struct,
    // initial call struct array, etc...
    int size = 0;
    
    size += sizeof(call) * 1; // Start with a default of 1 item;
    
    stack* local_stack = (stack*)malloc(sizeof(stack) + size);
    
    // Initialize memory allocation for `items` call array:
    local_stack->items = (struct call*)calloc(2, size);
    
    printf("\nCall Size: %li \nCall Array Size: %li\n", sizeof(call), sizeof(local_stack->items));
    
    return local_stack;
}



int delete_stack(struct VirtualMachine* vm_instance) {
    
    return 0;
}