// VirtualMachine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cstdio>
#include <string>
#include <iostream>
#include <stack>
#include <array>
#include <vector>
#include <set>

class VirtualMachine {
	// variables
	private:
	std::stack<uint16_t> &stack{*new std::stack<uint16_t>()};
	char *filemem;
	uint16_t *mem;
	uint16_t registers[8]; // extra regs;
	
	uint16_t REGISTER_CONSTANT = 32768;
	uint16_t* auto_value(uint16_t);

	uint16_t* value = new uint16_t(-1);

	uint16_t* a;
	uint16_t* b;
	uint16_t* c;

	int placeholder = 0;

	public:
	//functions
	VirtualMachine();
	void load_file_into_memory(char*);
	uint16_t* get_register(uint16_t);
	uint16_t get_value(uint16_t);
	bool is_register(uint16_t);
	void execute_program();
	void execute_instruction(uint16_t &);
};

using namespace std;

const int BUFFERSIZE = 16;

VirtualMachine vm;

int main()
{
	
	vm.load_file_into_memory("challenge.bin");
	vm.execute_program();
	return 0;
}

VirtualMachine::VirtualMachine()
{
}

void VirtualMachine::load_file_into_memory(char* filename)
{


	// how should i store my memory??? array seems good because it is a fixed size and indexed access is nice.
	/*
	unbounded stack : start at the beginning of memory and read the first instruction,
	instructions may tell the register to hold a specific address or value.. how to implement?

	register attributes: (store address or value, there needs to be a distinction)

	array - fixed size
	map -
	*/
	streampos size;
	ifstream bin(filename, ios::in | ios::binary | ios::ate);
	if (bin.is_open()) {
		size = bin.tellg();
		filemem = new char[(int)size];
		mem = new uint16_t[(int)size / 2];
		bin.seekg(0, ios::beg);
		bin.read(filemem, size);
		bin.close();
	}
	else {
		cout << "could not find file";
	}

	//cout << size << endl;
	cout << "Address of memblock: " << &filemem << endl;
	int memcount = 0;
	for (int i = 0; i < (int)size; i++) {
		uint8_t lsb, msb;
		msb = filemem[i];
		lsb = filemem[++i];
		mem[memcount] = (lsb << 8) + msb;
		memcount++;
	}

	/*for (int i = 0; i < 50; i++) {
			cout << +mem[i] << endl;
	}*/
}


void VirtualMachine::execute_program()
{
	//stack.push(0); // initialize main stack frame item
	uint16_t cur_address = 0; // 300
	//cout << cur_address;
	int i = 0;
	while (i < 1000) {
		execute_instruction(cur_address);
		//if (i < 400) cout << "i = " << i << endl;
		i++;
	}
	return;
}

uint16_t* VirtualMachine::auto_value(uint16_t arg)
{
	if (is_register(arg)) {
		return get_register(arg);
	}
	else {
		// TODO: investigate memory leak that may be caused by this..
		// do I have to delete these manually? Should I use a smart pointer here?
		value = new uint16_t(get_value(arg));
		return value;
	}
}

uint16_t* VirtualMachine::get_register(uint16_t arg)
{
	int index = mem[arg] - 32768;
	if (index >= 0) {
		return &registers[index];
	}
	else {
		cout << "Register index out of bounds..." << endl;
	}
}

uint16_t VirtualMachine::get_value(uint16_t arg)
{
	int index = mem[arg];
	//if (index >= 0 && index <= 32767) {
		return index;
	//}
	//else {
	//	cout << "Value index invalid..." << endl;
	//}
}

// TODO add an auatomatic value function...

bool VirtualMachine::is_register(uint16_t arg)
{
	int index = mem[arg];
	return index > 32767;
}

/*bool VirtualMachine::contains(*std::array)
{
	return false;
}*/



void VirtualMachine::execute_instruction(uint16_t &address) // pass in address by reference
{
	int num_args;
	// i have a list of registers that i want to access...

	// how to do templates?

	switch (+mem[address]) {
	case 0: num_args = 0; // halt
		//while (!stack.empty()) stack.pop();
		cout << "Terminating program..." << endl;
		break;

	case 1: num_args = 2; // set
		a = auto_value(++address);
		b = auto_value(++address);
		cout << "a was " << *a;
		*a = *b;

		cout << " but is now " << *a << endl;
		break;
	
	case 2: // push
		a = auto_value(++address);

		stack.push(*a);
		break;

	case 3: // pop
		// TODO add error here if stack is empty
		a = auto_value(++address);

		*a = stack.top();
		stack.pop();
		break;
	case 4: // eq
		a = auto_value(++address);
		b = auto_value(++address);
		c = auto_value(++address);

		if (*b == *c) {
			*a = 1;
		} else {
			*a = 0;
		}
		break;

	case 5: num_args = 3; // gt
		a = auto_value(++address);
		b = auto_value(++address);
		c = auto_value(++address);

		if (*b > *c) {
			*a = 1;
		} else {
			*a = 0;
		}
		break;

	case 6: num_args = 1; // jmp
		address = *auto_value(++address);
		return; // leave immmediately
		break;

	case 7: num_args = 2; // jt
		a = auto_value(++address);
		b = auto_value(++address);

		cout << "(1) a is " << *a << endl;
		if (*a != 0) {
			address = *b;
			return; // leave immediately
		}	
		break;

	case 8: num_args = 2; // jf
		a = auto_value(++address);
		b = auto_value(++address);

		cout << "(2) a is " << *a << endl;
		if (*a == 0) {
			address = *b;
			return; // leave immediately
		}	
		break;

	case 9: // add
		a = auto_value(++address); // always register
		b = auto_value(++address); 
		c = auto_value(++address);
		
		*a = (*b + *c) % 32768;
		cout << *b << "+" << *c << "=" << *a << endl;
		break;

	case 10: // mult
		a = auto_value(++address);
		b = auto_value(++address);
		c = auto_value(++address);

		*a = ((*b) * (*c) % 32768);
		cout << *b << "*" << *c << "=" << *a << endl;
		break;

	case 11: // mod
		a = auto_value(++address);
		b = auto_value(++address);
		c = auto_value(++address);

		*a = *b % *c;
		break;

	case 12: // bitwise and
		a = auto_value(++address);
		b = auto_value(++address);
		c = auto_value(++address);

		*a = *b & *c;
		break;

	case 13: // bitwise or
		a = auto_value(++address);
		b = auto_value(++address);
		c = auto_value(++address);

		*a = *b | *c;
		break;
	
	case 14: // bitwise inverse
		a = auto_value(++address);
		b = auto_value(++address);

		*a = ~(*b) & ~(1<<15); // inverts the bits of b and stores in a while leaving the 16th bit alone
		break;

	case 15: // rmem
		// read memory at address <b> and write it to <a>
		a = auto_value(++address);
		b = auto_value(++address); // should return value of address

		*a = mem[*b]; // this could break if the address contained in b was for a register?
		break;

	case 16: // wmem
		// writes the memory from value <b> into address <a>
		// this actually changes the memory of the binary file we read at the beginning.. dangeorus if wrong
		a = auto_value(++address);
		b = auto_value(++address);

		mem[*a] = *b;
		break;

	case 17: // call
		// push next instruction to the stack, then jmp to a
		a = auto_value(++address);
		stack.push(++address);
		address = *a;
		return;
		break;

	case 18: // ret
		// remove the top element from the stack and jump to it. empty stack = halt.
		if (stack.size() != 0) {	
			address = stack.top();
			stack.pop();
			return;
		}
		cout << "Program terminating" << endl;
		break;
		

	case 19: // out
		cout << (char)get_value(++address);
		break;

	case 20: // in
		a = auto_value(++address);
		char sample;
		cin >> sample;
		*a = sample;
		//if ((char)*a == '\n') {

		//}
		break;
			
	case 21: // noop
		placeholder = 0;
		break;

	default:
		placeholder = 0;
		cout << "Instruction " << +mem[address] << " not defined..";
		break;
	}

	address++;
	return;
}