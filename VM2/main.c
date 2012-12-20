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

#define XCODE_COLORS_ESCAPE_MAC "\033["
#define XCODE_COLORS_ESCAPE  XCODE_COLORS_ESCAPE_MAC
#define XCODE_COLORS_RESET_FG XCODE_COLORS_ESCAPE "fg;"
#define XCODE_COLORS_RESET_BG  XCODE_COLORS_ESCAPE "bg;"
#define XCODE_COLORS_RESET     XCODE_COLORS_ESCAPE ";"

#define XCODE_COLORS_PINK XCODE_COLORS_ESCAPE "fg180,22,95;"
#define XCODE_COLORS_PURPLE XCODE_COLORS_ESCAPE "fg100,52,165;"
#define XCODE_COLORS_ORANGE XCODE_COLORS_ESCAPE "fg219,102,39;"
#define XCODE_COLORS_LIGHT_BLUE XCODE_COLORS_ESCAPE "fg66,181,219;"
#define XCODE_COLORS_BLACK XCODE_COLORS_ESCAPE "fg00,00,00;"

#define XCODE_COLORS_BG_BLACK XCODE_COLORS_ESCAPE "bg0,0,0;"
#define XCODE_COLORS_BG_WHITE XCODE_COLORS_ESCAPE "bg255,255,255;"

void getBin(int num, char *str)
{
    *(str+5) = '\0';
    int mask = 0x10 << 1;
    while(mask >>= 1)
        *str++ = !!(mask & num) + '0';
}


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

// 1) Register Char Association
// 2) Register Address
// 3) Register Value Address
//    If you would use "[0x08]" you would be accessing
//    the value of the A register. "[0x09]" = B ...
uint16_t register_types[8][3] = {
    {'A', 0x00, 0x08},
    {'B', 0x01, 0x09},
    {'C', 0x02, 0x0a},
    {'X', 0x03, 0x0b},
    {'Y', 0x04, 0x0c},
    {'Z', 0x05, 0x0d},
    {'I', 0x06, 0x0e},
    {'J', 0x07, 0x0f}
};

// [0x1b] = SP's Value
enum addr {
    SP = 0x1b,
    PC = 0x1c,
    EX = 0x1d
};

typedef struct {
    int16_t values[8][4];
} registers;

typedef struct {
    
    int sp; // Stack Pointer.
    int num_elements; // Number of Elements.
    int max;
    int starting_addr;
    // 1 -> ADDR MEM, 2-> VALUE
    int16_t *memory[]; // Stack Items.
    
} stack;

typedef struct {
    
    int size; // code size;
    // Array of code split by machine code separation / separate instructions.
    int pc; // Program Counter.
    int16_t code[]; // Array
    
} program;


typedef struct {
    stack* stack; // Current Stack
    //int registers[8];
    registers* registers;
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
stack*     create_stack();
void       free_stack(stack*);
void       push_stack(stack*, int value);
int16_t    pop_stack(stack*);
int16_t    pick_stack(stack*);
int16_t    peek_stack(stack*);
void       reset_stack(stack*);

registers* create_registers();
void       free_registers(registers*);

// Prototypes for the program.
program* create_program();
void     free_program(program*);
void     reset_program(program*);
void     load_program(program*, char path[]);

int main(int argc, const char * argv[]) {
    
    cpu* local_cpu = create_cpu();

    // Load program:
    load_program(local_cpu->program, "/Users/Daniel/Desktop/p2.hex");
    
    // Run Program:
    run_cpu(local_cpu);
    
    free_cpu(local_cpu);
}


// create_cpu will initialize the cpu and allocate enough memory.
cpu* create_cpu() {
    
    // Allocate Memory.
    cpu* local_cpu = (cpu*)malloc(sizeof(cpu));
    
    // Allocate / Create a new stack. (priority stack)
    local_cpu->stack = (stack*)create_stack();
    
    local_cpu->program = (program*)create_program();
    
    local_cpu->registers = (registers*)create_registers();
    
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
        local_cpu->registers->values[i][0] = (int16_t)register_types[i][1]; // Address Association
        local_cpu->registers->values[i][1] = (int16_t)register_types[i][2]; // Address Association
        local_cpu->registers->values[i][2] = (int16_t)register_types[i][0]; // Char Association
        local_cpu->registers->values[i][3] = 0; // Reset the registers' value.
    }
}

void free_cpu(cpu* cpu_instance) {
    free_stack(cpu_instance->stack);
    free_program(cpu_instance->program);
    free(cpu_instance);
}

void dump_registers(cpu* cp) {
    
    printf("\n\n\n%sCPU Registers:%s \n", XCODE_COLORS_BG_BLACK, XCODE_COLORS_RESET_BG);
    
    //for(int i = 0; i<8; i++)
    //    printf("Register '%i|%i' [%i] | ", (char)cp->registers->values[i][0], cp->registers->values[i][2], cp->registers->values[i][1]);
    

    printf("\nStatus [%i] | ", cp->status);
    printf("Overflow [%i] | ", cp->overflow);
    printf("Underflow [%i] | ", cp->underflow);
    printf("PC [%i] | ", cp->program->pc);
    //printf("IReg [%i] | ", cp->registers->);
    printf("\nInstructions {\n");
    for(int i = 0; i<cp->program->size; i++) {
        char str[20];
        getBin(cp->program->code[i], str);
        if ((cp->program->pc - 1) == i)
        {
            printf("\t[%s%s%s%s] <-- %s[PC]%s %s(Program Counter)%s\n",
                   XCODE_COLORS_BLACK,
                   XCODE_COLORS_BG_WHITE,
                   str,
                   XCODE_COLORS_RESET,
                   XCODE_COLORS_ORANGE, XCODE_COLORS_RESET,
                   XCODE_COLORS_LIGHT_BLUE, XCODE_COLORS_RESET);
        }
        else
        {
            printf("\t[%s%s%s%s]\n",
                   XCODE_COLORS_BLACK,
                   XCODE_COLORS_BG_WHITE,
                   str,
                   XCODE_COLORS_RESET
            );
        }
    }
    printf("}\n");
}

void run_cpu(cpu* i) {
    i->status = 1;
    i->running = 1;
    printf("%s\n\n\tInstructions\n\n%s", XCODE_COLORS_BG_BLACK, XCODE_COLORS_RESET_BG);
    printf("\n---------------------------------------------------------------------\n");
    while(i->running) {
        // Get the next instruction:
        i->opcode = i->program->code[i->program->pc];
        // Increment the pointer to the next instruction:
        i->program->pc++;
        
        // OpCodes:
        switch(i->opcode) {
            //
            // @opcode: SET
            // @args: 2
            //    b: Register Address
            //    a: Set Value
            // @description:
            //      This opcode takes 2 parameters, b and a. It'll set [a] (value of a) to b (the memory address of
            //      the pointer)
            //
            case SET:
            // Create a new scope so we can create variables, etc...
            {
                // Use b as the index of the register array.
                // Because the registers' addreses range from [0x00-0x07], we can
                // treat them as integers, converting them to [0-7].
                i->registers->values[(short)
                    i->program->code[ i->program->pc++ ]
                ][3] = i->program->code[ i->program->pc++ ];
                // Print the resulting opcode and it's effect:
                printf(
                       "\t%sSET%s [%s%c%s] %s%s0x0%x%s, 0x0%x\n", // String & Replacement Flags
                       XCODE_COLORS_BG_BLACK,
                       XCODE_COLORS_RESET_BG,
                       XCODE_COLORS_LIGHT_BLUE,
                       (int)i->registers->values[(short)i->program->code[ i->program->pc-1]][2], // Fetch the register's associating letter.
                       XCODE_COLORS_RESET,
                       XCODE_COLORS_BLACK,
                       XCODE_COLORS_BG_WHITE,
                       i->program->code[ i->program->pc-1], // Register's address.
                       XCODE_COLORS_RESET,
                       i->registers->values[(short)i->program->code[ i->program->pc-1]][3] // Registers' value ([b])
                );
                break;
            }
            case ADD:
                //i->registers[1] = i->current_pr->code[ i->program->pc ];
                //i->program->pc++;
                break;
            case SUB:
                break;
            // Store in the stack:
            case MUL:
                
                break;
            case MLI:
            
                break;
            case DIV:

                break;
            case DVI:

                break;
            default:
                printf("Instruction Fault at: [%i] -> %i", i->program->pc, i->program->code[i->program->pc]);
                i->status = 0;
                dump_registers(i);
                return;
                break;
        }
        
        dump_registers(i);
        
        // Check the size of the instructions left.
        if (i->program->code[i->program->pc] == (int)NULL)
        {
            i->running = 0;
            break;
        }
    }

}

stack* create_stack() {
    stack* local_stack = (stack*)malloc(sizeof(stack));
    reset_stack(local_stack);
    return local_stack;
}

void reset_stack(stack* local_stack) {
    local_stack->starting_addr = 0xffff;
    local_stack->sp            = local_stack->starting_addr;
    
    /** Initialize the stack **/
    local_stack->memory[local_stack->starting_addr] = NULL; // Initialize the starting address.
}

void push_stack(stack* st, int value) {
    // Push a new item on the stack.
    st->sp = st->sp++; // Increment the pointer.
    st->memory[st->sp] = (int16_t*)value;
}

int16_t pop_stack(stack* st) {
    // Remove the top most value;
    // We can't actually remove it,
    // so we'll nullify it, but return the old value.
    int16_t old_value = *st->memory[st->sp];
    st->memory[st->sp] = NULL;
    st->sp = st->sp - 1;
    return old_value;
}

int16_t pick_stack(stack* st) { // Doesn't touch SP
    return 0;
}

int16_t peek_stack(stack* st) { // Doesn't touch SP
    return 0;
}

void free_stack(stack* local_stack) {
    free(local_stack);
}

registers* create_registers() {
    registers* local_reg = (registers*)malloc(sizeof(registers));
    return local_reg;
}

void free_registers(registers* local_reg) {
    free(local_reg);
}

program* create_program() {
    program* local_pr = (program*)malloc(sizeof(program));
    return local_pr;
}

void load_program(program* local_pr, char path[]) {
    
    
        //10000
        //10
        //char buf[40];
        int i = 0;
        //int rv;
        int num_values;
        unsigned int num[80];
        //int16_t hex = 0;
        FILE *file = fopen(path, "rt");
        if (path) {
            
            printf("%s** Loading Program... **%s\n", XCODE_COLORS_BG_BLACK, XCODE_COLORS_RESET_BG);
            
            while (i < 80 && fscanf(file,"%x",&num[i]) != EOF)
                i++;
            
            fclose(file);
            
            num_values = i;
            
            printf("Successfully read program data: [");
            for (i = 0; i < num_values; i++)
            {
                local_pr->code[i] = num[i];
                printf(" 0x%x", num[i]);
            }
            
            local_pr->size = num_values;
        
            printf(" ]");
            
            //printf("0x%x", *buf);
            
            printf("\n%s** Done Loading Program. **%s\n\n", XCODE_COLORS_BG_BLACK, XCODE_COLORS_RESET_BG);
            
            //hex = (int16_t)"0x" + *buf;
        } else {
            printf("\n** ERROR ** [ Unable to load program file. ]\n");
        }
    
}

void reset_program(program* local_pr) {
    //memset(local_pr->code, 0, local_pr->code.size());
    local_pr->size = 0;
}

void free_program(program* local_pr) {
    free(local_pr);
}