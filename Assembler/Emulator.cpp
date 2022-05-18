#include "stdafx.h"
#include "Emulator.h"


/**/
/*
emulator::InsertMemory()
NAME
	emulator::InsertMemory - Stores the translated code in the
							 specified memory location.
SYNOPSIS
	bool emulator::InsertMemory(int a_location, long long a_contents);
	a_location -> The location where the translated code is to be stored.
	a_contents -> The translated code that is to be stored.
DESCRIPTION
	Stores the translated code in the specified memory location.
RETURNS
	Returns true if the a_contents was successfully stored in the location a_location.
	Returns false otherwise.
*/
/**/
bool emulator::insertMemory(int a_location, long long a_contents) {
	if (a_location >= MEMSZ) {
		return false;
	}

	m_memory[a_location] = a_contents;
	return true;
}
/*bool emulator::insertMemory(int a_location, long long a_contents)*/


/**/
/*
emulator::RunProgram()
NAME
	emulator::RunProgram - Runs the emulator by processing the translated code in the memory.
SYNOPSIS
	bool emulator::RunProgram();
DESCRIPTION
	Goes through each memory location and executes the instruction found.
RETURNS
	Returns true if the code was succesfully emulated.
	Returns false if error was encountered.

*/
/**/
bool emulator::runProgram() {
	//initializing variables that stores opcode, reg1,reg2 and address
	int OpCode = 0; 
	int reg1 = 0;
	int reg2 = 0;
	int address = 0;

	//run until the memory ends
	for (int i = 0; i < MEMSZ; i++) {
		long long contents = (m_memory[i]);
		if (contents == 0) {
			continue;
		}

		//calculating different values from the content
		OpCode = contents / 10'000'000;
		reg1 = (contents / 1'000'000) % 10;
		reg2 = (contents / 100'000) % 10;
		address = contents % 1'000'000;

		switch (OpCode) {
		case 1:
			//Reg <-- c(Reg) + c(ADDR) 
			m_reg[reg1] += m_memory[address];
			break;
		case 2:
			//Reg <-- c(Reg) - c(ADDR)
			m_reg[reg1] -= m_memory[address];
			break;
		case 3:
			//Reg <-- c(Reg) * c(ADDR)
			m_reg[reg1] *= m_memory[address];
			break;
		case 4:
			//Reg <-- c(Reg) / c(ADDR)
			m_reg[reg1] /= m_memory[address];
			break;
		case 5:
			//Reg <-- c(ADDR)
			m_reg[reg1] = m_memory[address];
			break;
		case 6:
			//ADDR <-- c(Reg)
			m_memory[address] = m_reg[reg1];
			break;
		case 7:
			//REG1 <--c(REG1) + c(REG2)
			m_reg[reg1] += m_reg[reg2];
			break;
		case 8:
			//REG1 <--c(REG1) - c(REG2)  
			m_reg[reg1] -= m_reg[reg2];
			break;
		case 9:
			// REG1 <--c(REG1) * c(REG2) 
			m_reg[reg1] *= m_reg[reg2];
			break;
		case 10:
			//REG1 <--c(REG1) / c(REG2)  
			m_reg[reg1] /= m_reg[reg2];
			break;
		case 11:
			// A line is read in and the number found there is recorded in the specified memory address.
			int userInput;
			cout << "?";
			cin >> userInput;
			if (userInput < MEMSZ) {
				m_memory[address] = userInput;
			}
			else {
				cout << "Too large value" << endl;
			}
			break;
		case 12:
			//c(ADDR) is displayed  The register value is ignored.
			cout << m_memory[address] << endl;
			break;
		case 13:
			//go to ADDR for next instruction.  The register value is ignored.
			i = address;
			break;
		case 14:
			//go to ADDR if c(Reg) < 0
			if (m_reg[reg1] < 0) {
				i = address-1;
			}
			break;
		case 15:
			//go to ADDR if c(Reg) = 0
			if (m_reg[reg1] == 0) {
				i = address-1;
			}
			break;
		case 16:
			// go to ADDR if c(Reg) > 0
			if (m_reg[reg1] > 0) {
				i = address-1;
			}
			break;
		case 17:
			// terminate execution.  The register value and address are ignored.
			i = MEMSZ;
			break;
		default:
			cout << "OpCode Error!" << endl;
			return false;
		}
		
	}
	return false;
}
/*bool emulator::runProgram()*/