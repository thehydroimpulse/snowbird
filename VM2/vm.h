//
//  vm.h
//  VM2
//
//  Created by Daniel Fagnan on 2012-12-17.
//  Copyright (c) 2012 Daniel Fagnan. All rights reserved.
//

#ifndef VM2_vm_h
#define VM2_vm_h

struct stack {};
struct Registers {};

/**
 * Create a new VirtualMachine struct with a new typedef.
 * This will contain the whole machine.
 */
typedef struct {
    
    // Store the call stack;
    struct stack* call_stack; // Pointer to the location of the stack
    
    struct Registers* reg;
    
} VirtualMachine;

VirtualMachine* create_vm();
int delete_vm(VirtualMachine* vm);

#endif
