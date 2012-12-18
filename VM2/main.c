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
    
    int size; // code size;
    // Array of code split by machine code separation / separate instructions.
    int code[];
    
} program;

typedef struct {
    
    stack* stack; // Current Stack
    int registers[8];
    int running;
    program* current_pr;
    int instruction_reg;
    int overflow;
    int status;
    int underflow;
    int temp;
    int max;
    
} cpu;

// Prototypes for creating and deleting the CPU.
cpu* create_cpu();
void free_cpu(cpu*);
void reset_cpu(cpu*);
void reset_registers(cpu*);
void run_cpu(cpu*);

void dump_registers(cpu*);

// Prototypes for the stack.
stack* create_stack();
void   free_stack(stack*);
void   reset_stack(stack*);

// Prototypes for the program.
program* create_program();
void     free_program(program*);
void     reset_program(program*);
void     load_program(program*, FILE*);

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
    cpu_instance->max = 255;
}

void reset_registers(cpu* cpu_instance) {
    for(int i = 0; i<8; i++)
        cpu_instance->registers[ i ] = 0;
}

void free_cpu(cpu* cpu_instance) {
   
    
    free_stack(cpu_instance->stack);
    free_program(cpu_instance->current_pr);
    free(cpu_instance);
    
   
}

void dump_registers(cpu* cp) {
    
    printf("CPU Registers: \n");
    
    for(int i = 0; i<8; i++)
        printf("Register %i [%i]\n", i, cp->registers[i]);
    
    printf("Status [%i]\n", cp->status);
    printf("Overflow [%i]\n", cp->overflow);
    printf("Underflow [%i]\n", cp->underflow);
    printf("PC [%i]\n", cp->stack->pc);
    printf("IReg [%i]\n", cp->instruction_reg);
}

void run_cpu(cpu* i) {
    i->status = 1;
    while(i->running) {
        // Get the next instruction:
        i->instruction_reg = i->current_pr->code[i->stack->pc];
        // Increment the pointer to the next instruction:
        i->stack->pc++;
        
        // OpCodes:
        switch(i->instruction_reg) {
            case LOAD0:
                i->registers[0] = i->current_pr->code[ i->stack->pc ];
                i->stack->pc++;
                break;
            case LOAD1:
                i->registers[1] = i->current_pr->code[ i->stack->pc ];
                i->stack->pc++;
                break;
            case ADD:
                i->temp = i->registers[0] + i->registers[1];
                if (i->temp > i->max) {
                    i->overflow = 1;
                    i->temp = i->max;
                }
                i->registers[0] = i->temp;
                break;
            case SUBSTRACT:
                i->temp = i->registers[0] - i->registers[1];
                if (i->temp < 0) {
                    i->underflow = 1;
                    i->temp = 0;
                }
                i->registers[0] = i->temp;
                break;
            case STORE0:
                i->current_pr->code[i->stack->pc] = i->registers[0];
                i->stack->pc++;
                break;
            case STORE1:
                i->current_pr->code[i->stack->pc] = i->registers[1];
                i->stack->pc++;
                break;
            default:
                printf("Instruction Fault at: %i", i->stack->pc);
                i->status = 0;
                dump_registers(i);
                return;
                break;
        }
        
        // Check the size of the instructions left.
        if (i->current_pr->size <= i->stack->pc)
        {
            i->running = 0;
            break;
        }
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


void free_stack(stack* local_stack) {
    free(local_stack);
}

program* create_program() {
    program* local_pr = (program*)malloc(sizeof(program));
    return local_pr;
}

void load_program(program* local_pr, FILE* file) {
    
}

void reset_program(program* local_pr) {
    //memset(local_pr->code, 0, local_pr->code.size());
    local_pr->size = 0;
}

void free_program(program* local_pr) {
    free(local_pr);
}

