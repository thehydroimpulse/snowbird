#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include <vector>
#include <iterator>
#include <list>
#include <map>
#include <iostream>

typedef unsigned char byte;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

using namespace std;

namespace Snow {

	enum {

		EMPTY = 0,
		ADD   = 1,
		SUB   = 2,
		MUL   = 3,
		DIV   = 4,
		STORE = 5,
		PUSH  = 6,
		POP   = 7,
		PEEK  = 8

	};

	class Registers {

	public:
		Registers(){}
		~Registers(){}
	private:
		u32 _sp;
		u32 _pc;
		u32 _reg[];
	};

	class Stack {
	private:
		map<u32, u32> _memory;
		u32 _size;
	public:
		Stack() {
			
		}
		~Stack() {}

		u32 const& size() const;
		void size(u32 const& newSize) {
			this->_size = newSize;



		}

		/**
		* Pushes a new item on top of the stack machine.
		* The stack is currently implemented using "vectors".
		* @param {u32} i - Value wanted to be pushed
		**/
		void push(u32 i) {

		}
		/**
		* Pops the top-most item off the stack and 
		* returns that value, but also removes it from
		* the stack.
		* @return u32
		**/
		u32 pop() {
			return 1;
		}
		/**
		* Peeks at the top-most stack value and returns the 
		* value. The difference between POP and PEEK is that
		* peek won't remove the stack element, only "look" or "
		* peek" at it.
		**/
		u32 peek() {
			return 1;
		}

	};

	class Bird {
	public:
		Bird() {
			this->_stack = new Stack();
			this->_registers = new Registers();

			// Set the stack size: 
			this->_stack->size(0x1000);

		}
		~Bird() {}
	private:
		Stack *_stack;
		Registers *_registers;
	};

};

int main(int argc, const char * argv[]) {


	/** Initialize a new instance of the virtual machine **/
	Snow::Bird *virtualMachine = new Snow::Bird();
	/** Delete the memory allocation **/
	delete virtualMachine;
	/** Temp Hack **/
	system("PAUSE");
	return 0;
}