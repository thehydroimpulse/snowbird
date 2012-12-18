# E4 - A simplistic virtual machine in C

E4 is more of a personal research project then anything else. I wondered what, or how a virtual machine works, and more specifically, how CPUs work in general. 

I'm started with virtualizing a real, old CPU, then once I can successfully virtualize it, i'll try and build the real CPU. 

Choices:
* AGC (Apollo Guidance Computer)
 ** More specifically the block 2 version

* DCPU-16 (Mainly because of the vast resources on it, AGC is pretty old - as it's the first integrated circuit computer)


## TODO:

* Split the system in individual files.
	* cpu.h/cpu.c
	* stack.h/stack.c
	* program.h/program.c
	* ...
* Emulate a real CPU (I'm not emulating anything real atm)
* Add the appropriate opcodes
* Ability to read a program from a file. Either text or binary.
* Ability to write assembly code and compile it into correct machine code.


## IDEAS:

* Create a simple programming language with the VM
	* Somewhat similar to C/JavaScript
	* Steps:
		* Parser/Lexer
		* Compiler 
			* First Compile to Assembly
			* Assembly -> Machine Code

The language would be somewhat simple, nothing huge.


# License

Copyright (C) 2012-2013 Daniel Fagnan

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.