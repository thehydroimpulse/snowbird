//
//  stack.h
//  VM2
//
//  Created by Daniel Fagnan on 2012-12-17.
//  Copyright (c) 2012 Daniel Fagnan. All rights reserved.
//

#ifndef VM2_stack_h
#define VM2_stack_h

struct VirtualMachine {};
struct call {};

typedef struct {
    
    short int n; // dumby;
    struct call *items;
    
} stack;


stack* create_stack(struct VirtualMachine* vm_instance);
int delete_stack(struct VirtualMachine* vm_instance);

#endif
