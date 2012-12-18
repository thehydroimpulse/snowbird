//
//  main.c
//  VM2
//
//  Created by Daniel Fagnan on 2012-12-18.
//  Copyright (c) 2012 Daniel Fagnan. All rights reserved.
//

/** Includes **/
#include <stdio.h> // printf
#include <stdlib.h> // malloc/free
#include <string.h> // memset

enum Operands {
    LOAD0 = 0,
    LOAD1,
    ADD,
    SUBSTRACT,
    STORE0,
    STORE1
};

typedef struct {
    
    int pc; // Program Counter.
    int sp; // Stack Pointer.
    int st[]; // Stack Items.
    
} stack;


typedef struct {
    
    stack* stack; // Current Stack
    int registers[8];
    int running;
    program* current_pr;
    
} cpu;

typedef struct {
    
    int size; // code size;
    unsigned long int code; // code;
    
} program;

// Prototypes for creating and deleting the CPU.
cpu* create_cpu();
void free_cpu(cpu*);
void reset_cpu(cpu*);
void reset_registers(cpu*);
void run_cpu(cpu*);


// Prototypes for the stack.
stack* create_stack();
void   free_stack(stack*);
void   reset_stack(stack*);

// Prototypes for the program.
program* create_program();
void     free_program(program*);
void     reset_program(program*);

int main(int argc, const char * argv[]) {
    
    
    
}


// create_cpu will initialize the cpu and allocate enough memory.
cpu* create_cpu() {
    
    // Allocate Memory.
    cpu* local_cpu = (cpu*)malloc(sizeof(cpu));
    
    // Allocate / Create a new stack. (priority stack)
    local_cpu->stack = (stack*)create_stack();
    
    // Reset Registers & Stack.
    reset_cpu(local_cpu);
    
    // Ready to receive a program's data.
    return local_cpu;
}

void reset_cpu(cpu* cpu_instance) {
    reset_stack(cpu_instance->stack);
    reset_registers(cpu_instance);
}

void reset_registers(cpu* cpu_instance) {
    for(int i = 0; i<8; i++)
        cpu_instance->registers[ i ] = 0;
}

void free_cpu(cpu* cpu_instance) {
   
    
    free(cpu_instance->stack);
    free(cpu_instance);
    
   
}

void run_cpu(cpu* cpu_instance) {
    
    while(cpu_instance->running) {
        
        
        
    }
    
}

stack* create_stack() {
    
    stack* local_stack = (stack*)malloc(sizeof(stack));
    
    return local_stack;
}

void reset_stack(stack* local_stack) {
    local_stack->pc = 0;
    local_stack->sp = 0;
}



