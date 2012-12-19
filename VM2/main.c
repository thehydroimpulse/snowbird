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
#include <stdint.h> // 16-bit integers.

/**enum Operands {
    LOAD0 = 0x01,
    LOAD1,
    ADD,
    SUBSTRACT,
    STORE0,
    STORE1,
    STORE
};**/
     
enum opcodes {
    NA  = 0x00,
    SET = 0x01,
    ADD = 0x02,
    SUB = 0x03,
    MUL = 0x04,
    MLI = 0x05,
    DIV = 0x06,
    DVI = 0x07,
    MOD = 0x08,
    MDI = 0x09,
    AND = 0x0a,
    BOR = 0x0b,
    XOR = 0x0c,
    SHR = 0x0d,
    ASR = 0x0e,
    SHL = 0x0f,
    IFB = 0x10,
    IFC = 0x11,
    IFE = 0x12,
    IFN = 0x13,
    IFG = 0x14,
    IFA = 0x15,
    IFL = 0x16,
    IFU = 0x17,
    ADX = 0x1a,
    SBX = 0x1b,
    STI = 0x1e,
    STD = 0x1f
};


int register_types[8][2] = {
    {'A', 0x00},
    {'B', 0x01},
    {'C', 0x02},
    {'X', 0x03},
    {'Y', 0x04},
    {'Z', 0x05},
    {'I', 0x06},
    {'J', 0x07}
};

typedef struct {
    
    int sp; // Stack Pointer.
    int num_elements; // Number of Elements.
    int max;
    int *memory[]; // Stack Items.
    
} stack;

typedef struct {
    
    int size; // code size;
    // Array of code split by machine code separation / separate instructions.
    int code[7];
    int pc; // Program Counter.
    
} program;

struct registers {
    int16_t values[8][3];
    
};

typedef struct {
    stack* stack; // Current Stack
    //int registers[8];
    struct registers* registers;
    short running;
    program* program;
    //int instruction_reg;
    int16_t opcode;
    short overflow;
    short status;
    short underflow;
    //int temp;
    //int max;
    short num_registers;
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
void   push_stack(stack*, int value);
int    pop_stack(stack*);
int    pick_stack(stack*);
int    reset_stack(stack*);

// Prototypes for the program.
program* create_program();
void     free_program(program*);
void     reset_program(program*);
void     load_program(program*, FILE*, int code[], int size);

int main(int argc, const char * argv[]) {
    
    cpu* g_cpu = create_cpu();
    
    int code[7] = {LOAD0, 0, LOAD1, 7, ADD, STORE0, 255};
    
    // Load program:
    load_program(g_cpu->program, NULL, code, 7);
    
    // Run Program:
    run_cpu(g_cpu);
    
    free_cpu(g_cpu);
}


// create_cpu will initialize the cpu and allocate enough memory.
cpu* create_cpu() {
    
    // Allocate Memory.
    cpu* local_cpu = (cpu*)malloc(sizeof(cpu));
    
    // Allocate / Create a new stack. (priority stack)
    local_cpu->stack = (stack*)create_stack();
    
    local_cpu->program = (program*)create_program();
    
    local_cpu->num_registers = 8;
    
    // Reset Registers & Stack.
    reset_cpu(local_cpu);
    
    // Ready to receive a program's data.
    return local_cpu;
}

void reset_cpu(cpu* cpu_instance) {
    reset_stack(cpu_instance->stack);
    reset_registers(cpu_instance);
    reset_program(cpu_instance->program);
}

void reset_registers(cpu* local_cpu) {
    for(int i = 0; i<local_cpu->num_registers; i++) {
        local_cpu->registers->values[i][0] = register_types[i][0]; // Char Association
        local_cpu->registers->values[i][1] = 0; // Reset the registers' value.
        local_cpu->registers->values[i][2] = register_types[i][1]; // Address Association
    }
}

void free_cpu(cpu* cpu_instance) {
    free_stack(cpu_instance->stack);
    free_program(cpu_instance->program);
    free(cpu_instance);
}

void dump_registers(cpu* cp) {
    
    printf("\n\n\nCPU Registers: \n");
    
    for(int i = 0; i<8; i++)
        printf("Register '%i|%i' [%i] | ", (char)cp->registers->values[i][0], cp->registers->values[i][2], cp->registers->values[i][1]);
    
    printf("\nStatus [%i] | ", cp->status);
    printf("Overflow [%i] | ", cp->overflow);
    printf("Underflow [%i] | ", cp->underflow);
    printf("PC [%i] | ", cp->program->pc);
    //printf("IReg [%i] | ", cp->registers->);
    printf("Instructions {\n");
    for(int i = 0; i<cp->program->size; i++) {
        printf("[%i]", cp->program->code[i]);
    }
    printf("}\n");
}

void run_cpu(cpu* i) {
    i->status = 1;
    i->running = 1;
    while(i->running) {
        // Get the next instruction:
        i->opcode = i->program->code[i->program->pc];
        // Increment the pointer to the next instruction:
        i->program->pc++;
        
        // OpCodes:
        switch(i->opcode) {
            case LOAD0:
                i->registers[0] = i->current_pr->code[ i->program->pc ];
                i->program->pc++;
                break;
            case LOAD1:
                i->registers[1] = i->current_pr->code[ i->program->pc ];
                i->program->pc++;
                break;
            case ADD:
                i->temp = i->registers[0] + i->registers[1];
                if (i->temp > i->max) {
                    i->overflow = 1;
                    i->temp = i->max;
                }
                i->registers[0] = i->temp;
                break;
            // Store in the stack:
            case STORE:
                
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
        
        dump_registers(i);
        
        // Check the size of the instructions left.
        if (i->current_pr->size <= i->stack->pc)
        {
            i->running = 0;
            break;
        }
    }ddddd

}

stack* create_stack() {
    stack* local_stack = (stack*)malloc(sizeof(stack));
    reset_stack(local_stack);
    return local_stack;
}

void reset_stack(stack* local_stack) {
    local_stack->pc = 0;
    local_stack->sp = 0;
}

int push_stack(stack* st, int value) {

    // Push a new item on the stack.
    st->st[st->num_elements++] = value;
    
}

int pop_stack(stack* st) {
    return 1;
}

int pick_stack(stack* st) {
    
}

void free_stack(stack* local_stack) {
    free(local_stack);
}

program* create_program() {
    program* local_pr = (program*)malloc(sizeof(program));
    return local_pr;
}

void load_program(program* local_pr, FILE* file, int code[], int size) {
    
    if (!file)
    {
        for (int i = 0; i<size; i++) {
            local_pr->code[i] = code[i];
        }
        local_pr->size = size;
    } else
    {
        
    }
    
}

void reset_program(program* local_pr) {
    //memset(local_pr->code, 0, local_pr->code.size());
    local_pr->size = 0;
}

void free_program(program* local_pr) {
    free(local_pr);
}

