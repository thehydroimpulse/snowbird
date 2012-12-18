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
    struct call *array;
    size_t used;
    size_t size;
} CArray;

void initArray(CArray *a, size_t initialSize);

void insertArray(CArray *a, struct call element);

void freeArray(CArray *a);

typedef struct {
    int esp; // Stack Pointer Register;
    int ebp; // Base Pointer Register;
    CArray calls;
} stack;


stack* create_stack(struct VirtualMachine* vm_instance);
int delete_stack(struct VirtualMachine* vm_instance);

void push_stack(struct VirtualMachine* vm_instance);
void pop_stack(struct VirtualMachine* vm_instance);


#endif
