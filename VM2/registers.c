//
//  registers.c
//  VM2
//
//  Created by Daniel Fagnan on 2012-12-17.
//  Copyright (c) 2012 Daniel Fagnan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "registers.h"


void create_registers(Registers* reg) {
    
    for(int i = 0; i<8; i++)
        reg->values[i] = 0;
    
    reg->cross[0] = 'A';
    reg->cross[1] = 'B';
    reg->cross[2] = 'C';
    reg->cross[3] = 'X';
    reg->cross[4] = 'Y';
    reg->cross[5] = 'Z';
    reg->cross[6] = 'I';
    reg->cross[7] = 'J';
}