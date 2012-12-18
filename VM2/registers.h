//
//  registers.h
//  VM2
//
//  Created by Daniel Fagnan on 2012-12-17.
//  Copyright (c) 2012 Daniel Fagnan. All rights reserved.
//

#ifndef VM2_registers_h
#define VM2_registers_h

typedef struct {
    short int n; // dumby. Because we can't have a flexible array member inside an empty struct.
    int values[];
} Registers;

Registers* create_registers();

#endif
