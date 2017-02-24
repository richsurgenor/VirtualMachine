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
	int registers[3];

	public:
	//functions
	VirtualMachine();
	void load_file_into_memory(char*);
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

	for (int i = 0; i < 50; i++) {
			cout << +mem[i] << endl;
	}
}


void VirtualMachine::execute_program()
{
	stack.push(0); // initialize main stack frame item
	uint16_t cur_address = 0;
	//cout << cur_address;
	int i = 0;
	while (i < 400) {
		execute_instruction(cur_address);
		i++;
	}
	return;
}

void VirtualMachine::execute_instruction(uint16_t &address) // pass in address by reference
{
	int argument;
	switch (+mem[address]) {
	case 19:
		argument = ++address;
		cout << (char) mem[argument];
		break;
	case 21:
		break;
	default:
		cout << "Instruction not defined..";
	}
	address++;
	return;
}