// VirtualMachine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cstdio>
#include <string>
#include <iostream>

class VirtualMachine {
public:
	VirtualMachine();

	void VirtualMachine::load_file_into_memory(char*);
};

using namespace std;

const int BUFFERSIZE = 16;

VirtualMachine vm;

int main()
{
	
	vm.load_file_into_memory("challenge.bin");
	return 0;
}

VirtualMachine::VirtualMachine()
{

}

void VirtualMachine::load_file_into_memory(char* filename)
{
	char *filemem;
	uint16_t *mem;

	streampos size;
	ifstream bin(filename, ios::in | ios::binary | ios::ate);
	if (bin.is_open()) {
		size = bin.tellg();
		filemem = new char[size];
		mem = new uint16_t[size / 2];
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
	for (int i = 0; i < (int) size; i++) {
		uint8_t lsb, msb;
	    msb = filemem[i];
		lsb = filemem[++i];
		mem[memcount] = (lsb << 8) + msb;
		memcount++;
	}

	for (int i = 0; i < ((int) size / 2); i++) {
		cout << +mem[i] << endl;
	}

}
