//
//  vm.c
//  VM2
//
//  Created by Daniel Fagnan on 2012-12-17.
//  Copyright (c) 2012 Daniel Fagnan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "stack.h"
#include "registers.h"

VirtualMachine* create_vm() {
    // Create the virtual machine within the heap.s
    printf("size of virtualmachine %li", sizeof(VirtualMachine));
    VirtualMachine* local_vm = (VirtualMachine*)malloc(sizeof(VirtualMachine));
    //printf("%p", local_vm);
    
    printf("** Created a new virtual machine. **\n");
    
    // Initialize the registers;
    
    local_vm->call_stack = (struct stack*)create_stack((struct VirtualMachine*)local_vm);
    
    return NULL;
}

int delete_vm() {
    return 0;
}