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
	char *memblock;
	//uint8_t lsb, msb;

	streampos size;
	ifstream bin(filename, ios::in | ios::binary | ios::ate);
	if (bin.is_open()) {

		size = bin.tellg();
		memblock = new char[size];
		bin.seekg(0, ios::beg);
		bin.read(memblock, size);
		bin.close();
	}
	else {
		cout << "could not find file";
	}

	cout << size << endl;
	cout << "Address of memblock: " << &memblock << endl;
	for (int i = 0; i < size; i++) {
		uint8_t lsb, msb;
		uint16_t num;
		msb = memblock[i];
		//cout << lsb << endl;
		lsb = memblock[++i];
		num = (lsb << 8) + msb;
		//cout << num << endl;
	}
	//cout << (int)memblock[0] << endl;
	//cout << (int)memblock[1] << endl;
	//cout << (int)memblock

}
