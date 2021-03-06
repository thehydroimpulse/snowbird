//
//  main.c
//  VM2
//
//  Created by Daniel Fagnan on 2012-12-18.
//  Copyright (c) 2012 Daniel Fagnan. All rights reserved.
//

/**
 * Include all the files we need from the C library set.
 **/
// We need functions like printf
#include <stdio.h>
// Memory management (malloc, free, etc...)
#include <stdlib.h>
// Memset
#include <string.h>
// 16-bit integers.
#include <stdint.h>

#define COLORS_ESCAPE "\033["
#define COLORS_RESET  COLORS_ESCAPE "0m"
#define COLORS_RESET_FG COLORS_RESET
#define COLORS_RESET_BG  COLORS_RESET

#define COLORS_PINK COLORS_ESCAPE "31m"
#define COLORS_PURPLE COLORS_ESCAPE "31m"
#define COLORS_ORANGE COLORS_ESCAPE "31m"
#define COLORS_LIGHT_BLUE COLORS_ESCAPE "31m"
#define COLORS_BLACK COLORS_ESCAPE "30m"

#define COLORS_BG_BLACK COLORS_ESCAPE "46m"
#define COLORS_BG_WHITE COLORS_ESCAPE "47m"
#define COLORS_BG_RED "41m"


#define DEBUGOPCODE 0

// Types:
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

/**
 * Convert decimal integers to a full base 2 (binary) number.
 */
void getBin(int num, char *str)
{
    *(str+5) = '\0';
    int mask = 0x10 << 1;
    while(mask >>= 1)
        *str++ = !!(mask & num) + '0';
}

/**
 * Initialize a new enumerable for all the normal opcodes.
 * Each opcode will have a specific hexadecimal (binary) representation
 * that are completely unique in opcode indices. 
 * Special opcodes will be separate, as some, are included as extra 
 * parameters.
 */
enum opcodes {
    // Not Applicable. NULL
    NA  = 0x00,
    // Set a memory address.
    // @params: b, a
    //          b = Memory Address
    //          a = Value
    // @note:   This only works with registers and not stack memory.
    // @todo:   Add stack memory support. (though a little confused on the difference
    //          between this and PUSH...
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
    {'A', 0x00, 0x08}, // First.
    {'B', 0x01, 0x09},
    {'C', 0x02, 0x0a},
    {'X', 0x03, 0x0b},
    {'Y', 0x04, 0x0c},
    {'Z', 0x05, 0x0d},
    {'I', 0x06, 0x0e},
    {'J', 0x07, 0x0f} // Last.
};

// [0x1b] = SP's Value
enum addr {
    SP = 0x1b,
    PC = 0x1c,
    EX = 0x1d
};

// Global Memory Mappings.
// You would use this table to lookup a memory address.
struct memory_map_t{
    // type of address; register = 0 / stack = 1
    // We need the type to then discover the real address.
    short type;
    // Store the real address anyways. It'll be faster/more effecient.
    // 0x001 -> 0x0001 (Register Type)
    // 0x002 -> 0x0005A (Stack Type)
    u16 addr;
};

typedef struct {
    u16 values[8][4];
    // Special Registers:
    u16 pc; // Program Counter.
    u16 sp; // Stack Pointer.
    u32     ex; // Excess (Big).
    u16 ia; // Interrupt Address.
} registers;

typedef struct {
    
    u16 sp; // Stack Pointer.
    u16 num_elements; // Number of Elements.
    u16 starting_addr[2];
    // 1 -> ADDR MEM, 2-> VALUE
    //struct memory_map_t* memory_map; // The index will become the lookup memory address (i.e 0x01)
    u16 **memory; // Stack Items.
    
} stack;

typedef struct {
    
    int size; // code size;
    // Array of code split by machine code separation / separate instructions.
    u16 code[]; // Array
    
} program;


typedef struct {
    stack* stack; // Current Stack
    registers* registers;
    short running;
    program* program;
    u16 opcode;
    short overflow;
    short status;
    short underflow;
    short num_registers;
} cpu;

// Prototypes for creating and deleting the CPU.
cpu* create_cpu();
void free_cpu(cpu*);
void reset_cpu(cpu*);
void reset_registers(cpu*);
void run_cpu(cpu*);

void dump_registers(cpu*, u8 n);
void dump_memory(cpu*);

// Prototypes for the stack.
stack*     create_stack();
void       free_stack(stack*);
void       push_stack(stack*, int value);
u16        pop_stack(stack*);
u16        pick_stack(stack*);
u16        peek_stack(stack*);
void       reset_stack(stack*);

registers* create_registers();
void       free_registers(registers*);

// Prototypes for the program.
program* create_program();
void     free_program(program*);
void     reset_program(program*);
void     load_program(program*, char path[]);


u16     get_arg(cpu* local_cpu, u8 n);
u16*    get_args(cpu* local_cpu, u8 n);
void    next_instruction(cpu* local_cpu, u16 n);
void    prev_instruction(cpu* local_cpu);
u16     get_opcode(cpu* local_cpu);

void    set_register(cpu* local_cpu, u16 address, u16 value);
void    set_register_ex(cpu* local_cpu, u32 value);
u16     get_register_value(cpu* local_cpu, u16 address, u8 type);
u32     get_register_ex_value(cpu* local_cpu);

u16     get_pc(cpu* local_cpu);
u16     get_code(cpu* local_cpu, u16 n);

void    debug_opcode(cpu* local_cpu, char *opcode, u16 n);

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
    return local_cpu;
}

void reset_cpu(cpu* cpu_instance) {
    reset_stack(cpu_instance->stack);
    reset_registers(cpu_instance);
    reset_program(cpu_instance->program);
}

void reset_registers(cpu* local_cpu) {
    for(int i = 0; i<local_cpu->num_registers; i++) {
        local_cpu->registers->values[i][0] = (u16)register_types[i][1]; // Address Association
        local_cpu->registers->values[i][1] = (u16)register_types[i][2]; // Address Association
        local_cpu->registers->values[i][2] = (u16)register_types[i][0]; // Char Association
        local_cpu->registers->values[i][3] = 0; // Reset the registers' value.
    }
}

void free_cpu(cpu* cpu_instance) {
    free_stack(cpu_instance->stack);
    //free_registers(cpu_instance->registers);
    //free_program(cpu_instance->program);
    free(cpu_instance);
}


void run_cpu(cpu* i) {
    i->status = 1;
    i->running = 1;
    
    //printf("\n\t---------------------------------------------------------------------\n");
    while(i->running) {
        // Get the next instruction:
        i->opcode = get_opcode(i);
        // Increment the pointer to the next instruction:
        next_instruction(i, 1);
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
            // @todo: Add support for stack memory addresses.
            //
            case SET:
            // Create a new scope so we can create variables, etc...
            {
                // Use b as the index of the register array.
                // Because the registers' addreses range from [0x00-0x07], we can
                // treat them as integers, converting them to [0-7].
                set_register(i, get_arg(i, 0), get_arg(i, 1));
                debug_opcode(i, "SET", 2);
                next_instruction(i, 2); // 2 arguments.
                break;
            }
            case ADD:
            {
                set_register_ex(i, get_register_value(i, get_arg(i, 0), 3) + get_arg(i, 1));
                if (get_register_ex_value(i) > 0xffff)
                {
                    // Overflow:
                    set_register_ex(i, 0x001);
                    i->overflow = 1;
                    set_register(i, get_arg(i, 0), 0x00);
                }
                else
                {
                    set_register(i, get_arg(i, 0), get_register_ex_value(i));
                    set_register_ex(i, 0x000);
                }
                
                debug_opcode(i, "ADD", 2);                
                next_instruction(i, 2); // 2 arguments.
                break;
            }
            case SUB:
            {
                
                set_register_ex(i, get_register_value(i, get_arg(i, 0), 3) - get_arg(i, 1));
                
                if ( get_arg(i, 1) > get_register_value(i, get_arg(i, 0), 3))
                {
                    // Overflow:
                    set_register_ex(i, 0xffff);
                    i->underflow = 1;
                    set_register(i, get_arg(i, 0), 0x00);
                }
                else
                {
                    set_register(i, get_arg(i, 0), get_register_ex_value(i));
                    set_register_ex(i, 0x000);
                }
                
                debug_opcode(i, "SUB", 2);
                next_instruction(i, 2); // 2 arguments.
                break;
            }
            // Store in the stack:
            case MUL:
                set_register_ex(i, get_register_value(i, get_arg(i, 0), 3) * get_arg(i, 1));
                set_register(i, get_arg(i, 0), get_register_ex_value(i));
                set_register_ex(i, get_register_ex_value(i) >> 16);
                debug_opcode(i, "MUL", 2);
                next_instruction(i, 2); // 2 arguments.                
                break;
            case MLI:
            
                break;
            case DIV:
                
                if (get_arg(i, 1) == 0)
                {
                    set_register_ex(i, 0);
                    set_register(i, get_arg(i, 0), 0);
                }
                else
                {
                    set_register_ex(i, get_register_value(i, get_arg(i, 0), 3) / get_arg(i, 1));
                    set_register(i, get_arg(i, 0), get_register_ex_value(i));
                }
                debug_opcode(i, "DIV", 2);
                next_instruction(i, 2); // 2 arguments.
                break;
            case DVI:

                break;
            default:
                printf("\n\n%s\n\n\n\t\tInstruction Fault at: [%s%s%i%s%s] -> 0x%x\n\n\n%s\n",
                       COLORS_BG_RED,
                       COLORS_BG_BLACK,
                       COLORS_LIGHT_BLUE,
                       i->registers->pc,
                       COLORS_RESET,
                       COLORS_BG_RED,
                       i->program->code[i->registers->pc],
                       COLORS_RESET
                );
                i->status = 0;
                dump_registers(i, 0);
                return;
                break;
        }
        
        // Check the size of the instructions left.
        if (i->registers->pc >= i->program->size)
        {
            i->running = 0;
            break;
        }
    }

}

void dump_registers(cpu* cp, u8 n) {
    
    printf("\n\n\t\t\t%s%s\t\t\tData Dump:\t\t\t\t%s\n", COLORS_BG_WHITE, COLORS_BLACK, COLORS_RESET);
    
    //for(int i = 0; i<8; i++)
    //    printf("Register '%i|%i' [%i] | ", (char)cp->registers->values[i][0], cp->registers->values[i][2], cp->registers->values[i][1]);
    

    printf("\n\t\t\t%s Status %s [%i] %s%s|%s ",
           COLORS_BG_BLACK,
           COLORS_RESET,
           cp->status,
           COLORS_BG_BLACK,
           COLORS_ORANGE,
           COLORS_RESET
    );
    printf("%s Overflow %s [%i] %s%s|%s ",
           COLORS_BG_BLACK,
           COLORS_RESET,
           cp->overflow,
           COLORS_BG_BLACK,
           COLORS_ORANGE,
           COLORS_RESET
    );
    printf("%s Underflow %s [%i] %s%s|%s ",
           COLORS_BG_BLACK,
           COLORS_RESET,
           cp->underflow,
           COLORS_BG_BLACK,
           COLORS_ORANGE,
           COLORS_RESET
    );
    printf("%s PC %s [%i] %s%s|%s ",
           COLORS_BG_BLACK,
           COLORS_RESET,
           cp->registers->pc + n,
           COLORS_BG_BLACK,
           COLORS_ORANGE,
           COLORS_RESET
    );
    printf("%s EX %s [%i] %s%s|%s ",
           COLORS_BG_BLACK,
           COLORS_RESET,
           cp->registers->ex,
           COLORS_BG_BLACK,
           COLORS_ORANGE,
           COLORS_RESET
    );
    
    printf("\n\n\t\t\t%s    Generic Registers     %s\n\n",
        COLORS_BG_BLACK,
        COLORS_RESET
    );
    
    
    for (int k = 0; k<8; k++) {
        //cp->registers->values[k]
        
        printf("\t\t\t[%s%s %i %s]-> Address: [0x0%x - %c], Value: [0x0%x], Decimal: [%i]\n",
               COLORS_ORANGE,
               COLORS_BG_WHITE,
               k,
               COLORS_RESET,
               cp->registers->values[k][0],
               (char)cp->registers->values[k][2],
               cp->registers->values[k][3],
               cp->registers->values[k][3]
        );
    }
    
    
    /**printf("\n\n\t\t\t%s    Instructions     %s\n\n",
           COLORS_BG_BLACK,
           COLORS_RESET
    );
    for(int i = 0; i<cp->program->size; i++) {
        char str[20];
        getBin(cp->program->code[i], str);
        if (((cp->registers->pc + n) - 1) == i)
        {
            printf("\t\t\t[%s%s %s %s] <-- %s[PC]%s ",
                   COLORS_BLACK,
                   COLORS_BG_WHITE,
                   str,
                   COLORS_RESET,
                   COLORS_ORANGE,
                   COLORS_RESET
            );
        }
        else
        {
            printf("\t\t\t[%s%s%s%s] ",
                   COLORS_BLACK,
                   COLORS_BG_WHITE,
                   str,
                   COLORS_RESET
                   );
        }
    }**/
    printf("\n\n");
}

void dump_memory(cpu* local_cpu) {
    
}

stack* create_stack() {
    stack* local_stack = (stack*)malloc(sizeof(stack));
    
    // Initialize the memory_map array:
    // Start with 12 element sized array.
    //local_stack->memory_map = (struct memory_map_t*)malloc(sizeof(struct memory_map_t) * 12 * 2);
    
    // 327,680 elements of 16bits each. 5MB
    local_stack->memory = (u16**)malloc(65535 * sizeof(u16*));

    u16 previous_addr = 0xffff;

    for (int i = 0; i<65535; i++) {
        local_stack->memory[i] = malloc(2*sizeof(u16));
        local_stack->memory[i][0] = previous_addr - 1;
        local_stack->memory[i][1] = 0;
    }

    //reset_stack(local_stack);
    return local_stack;
}

void reset_stack(stack* local_stack) {
    local_stack->starting_addr[0] = 0xffff;
    local_stack->starting_addr[1] = 0;
    local_stack->sp            = local_stack->starting_addr[0];
    
    /** Initialize the stack **/
    //local_stack->memory[local_stack->starting_addr] = 0; // Initialize the starting address.
}

void push_stack(stack* st, int value) {
    // Push a new item on the stack.
    //st->sp = st->sp++; // Increment the pointer.
    //st->memory[st->sp] = (u16*)value;
}

u16 pop_stack(stack* st) {
    // Remove the top most value;
    // We can't actually remove it,
    // so we'll nullify it, but return the old value.
    //u16 old_value = *st->memory[st->sp];
    //st->memory[st->sp] = NULL;
    //st->sp = st->sp - 1;
    return 1;
}

u16 pick_stack(stack* st) { // Doesn't touch SP
    return 0;
}

u16 peek_stack(stack* st) { // Doesn't touch SP
    return 0;
}

void free_stack(stack* local_stack) {

    for(int i = 0; i<65535; i++) {
        free(local_stack->memory[i]);
    }

    free(local_stack->memory);
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
    
        int i = 0;
        int num_values;
        unsigned int num[80];
        FILE *file = fopen(path, "r");
        if (path) {
            
            printf("%s** Loading Program... **%s\n", COLORS_BG_BLACK, COLORS_RESET_BG);
            
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
            
            printf("\n%s** Done Loading Program. **%s\n\n", COLORS_BG_BLACK, COLORS_RESET_BG);
            
            //hex = (u16)"0x" + *buf;
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

u16 get_arg(cpu* local_cpu, u8 n) {
    return get_code(local_cpu, get_pc(local_cpu) + n);
}

void next_instruction(cpu* local_cpu, u16 n)  {
    local_cpu->registers->pc += n;
}

void prev_instruction(cpu* local_cpu) {
    
}

u16 get_opcode(cpu* local_cpu) {
    return get_code(local_cpu, get_pc(local_cpu));
}

void set_register(cpu* local_cpu, u16 address, u16 value) {
    local_cpu->registers->values[address][3] = value;
}

void set_register_ex(cpu* local_cpu, u32 value) {
    local_cpu->registers->ex = value;
}

u16 get_register_value(cpu* local_cpu, u16 address, u8 type) {
    return local_cpu->registers->values[address][type];
}

u32 get_register_ex_value(cpu* local_cpu) {
    return local_cpu->registers->ex;
}

u16 get_pc(cpu* local_cpu) {
    return local_cpu->registers->pc;
}

u16 get_code(cpu* local_cpu, u16 n) {
    return local_cpu->program->code[n];
}

u16* get_args(cpu* local_cpu, u8 n) {
    u16* arrg = (u16*)malloc(sizeof(u16) * n);
    
    for (int i = 0; i<n; i++) {
        arrg[i] = get_arg(local_cpu, i);
    }
    return arrg;
}

void debug_opcode(cpu* local_cpu, char *opcode, u16 n) {
#ifdef DEBUGOPCODE
    // Get the n arguments.
    u16 *args = get_args(local_cpu, n);
    
    printf("%s\n\n\tInstruction/OpCode\n\n%s", COLORS_BG_BLACK, COLORS_RESET_BG);

    printf(
           "\t%s%s%s [%s%c%s] %s%s0x0%x%s, 0x0%x\n", // String & Replacement Flags
           COLORS_BG_BLACK,
           opcode,
           COLORS_RESET_BG,
           COLORS_LIGHT_BLUE,
           get_register_value(local_cpu, args[0], 2),
           COLORS_RESET,
           COLORS_BLACK,
           COLORS_BG_WHITE,
           get_register_value(local_cpu, args[0], 0),
           COLORS_RESET,
           get_register_value(local_cpu, args[0], 3)
    );
    
    dump_registers(local_cpu, n);
    
    printf("%s\n\n\tEND\n\n%s", COLORS_BG_BLACK, COLORS_RESET_BG);
    
    free(args);
#endif
}


int main(int argc, char * argv[]) {
   
    cpu* local_cpu = create_cpu();

    // Load program:
    load_program(local_cpu->program, "C:\\Users\\Daniel\\Documents\\Projects\\snowbird\\programs\\div");
    
    // Run Program:
    run_cpu(local_cpu);
    
    free_cpu(local_cpu);
}
