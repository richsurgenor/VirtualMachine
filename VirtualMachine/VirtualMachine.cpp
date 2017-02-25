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

class VirtualMachine {
	// variables
	private:
	std::stack<uint16_t> &stack{*new std::stack<uint16_t>()};
	char *filemem;
	uint16_t *mem;
	uint16_t registers[8]; // extra regs;

	uint16_t a;
	uint16_t b;
	uint16_t c;

	uint16_t REGISTER_CONSTANT = 32768;
	

	public:
	//functions
	VirtualMachine();
	void load_file_into_memory(char*);
	uint16_t& get_register(uint16_t);
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
	stack.push(0); // initialize main stack frame item
	uint16_t cur_address = 0; // 300
	//cout << cur_address;
	int i = 0;
	while (i < 500) {
		execute_instruction(cur_address);
		i++;
	}
	return;
}

uint16_t& VirtualMachine::get_register(uint16_t arg)
{
	int index = mem[arg] - 32768;
	if (index >= 0) {
		return registers[index];
	}
	else {
		cout << "Register index out of bounds..." << endl;
	}
}

uint16_t VirtualMachine::get_value(uint16_t arg)
{
	int index = mem[arg];
	if (index >= 0 && index <= 32767) {
		return index;
	}
	else {
		cout << "Value index invalid..." << endl;
	}
}

// TODO add an auatomatic value function...

bool VirtualMachine::is_register(uint16_t arg)
{
	int index = mem[arg] - 32768;
	return index >= 0 && index <= 7;
}

void VirtualMachine::execute_instruction(uint16_t &address) // pass in address by reference
{
	int num_args;
	switch (+mem[address]) {
	case 0: num_args = 0; // halt
		while (!stack.empty()) stack.pop();
		break;
	case 1: num_args = 2; // set
		a = get_register(++address);
		b = get_value(++address);
		cout << "a was " << a;
		a = b;

		cout << " but is now " << a << endl;
		break;
	case 6: num_args = 1; // jmp
		if (is_register(++address)) {
			a = get_register(address);
		}
		else {
			a = get_value(address);
		}
		
		address = a;
		return; // leave immmediately
		break;
	case 7: num_args = 2; // jt
		if (is_register(++address)) {
			a = get_register(address);
		}
		else {
			a = get_value(address);
		}
		cout << "(1) a is " << a << endl;
		if (a != 0) {
			if (is_register(++address)) {
				b = get_register(address);
			}
			else {
				b = get_value(address);
			}
			address = b;
		}	
		return; // leave immediately
		break;
	case 8: num_args = 2; // jf
		if (is_register(++address)) {
			a = get_register(address);
		}
		else {
			a = get_value(address);
		}
		cout << "(2) a is " << a << endl;
		if (a == 0) {
			if (is_register(++address)) {
				b = get_register(address);
			}
			else {
				b = get_value(address);
			}
			address = b;
		}	
		break;
	case 19: // out
		cout << (char)get_value(++address);
		break;
	case 21: // noop
		break;
	//case 32768:
	//	register_a = mem;
	default:
		//cout << "Instruction not defined..";
		break;
	}
	address++;
	return;
}