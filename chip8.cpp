#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "chip8.hpp"

// Initialize cpu
chip8::chip8(const screen &s) :
	opcode {0},
	memory(MEMORY_SIZE, 0), 
	v(NUM_REGISTERS, 0), 
	i {0},	
	pc {START_PC},
	sp {0}, 
	stack(STACK_SIZE, 0),
	dt {0},
	st {0}
{
	// Load in fontset into memory
	for (unsigned int i = 0; i < NUM_FONTS; i++) {
		memory[FONTSET_START + i] = fontset[i];
	}
}

// Execute one cycle of cpu
void chip8::executeCycle()
{
	opcode = (memory[pc] << 8) | memory[pc + 1];

	if (dt > 0) {
		dt--;
	}

	if (st > 0) {
		if (st == 1) {
			// Do something with sound
		}
		st--;
	}
}

// Read a ROM file into memory
void chip8::readRom(const std::string &filename) 
{
	std::ifstream ifs (filename, std::ios::binary);

	if (ifs) {
		// Copy file into a buffer, then write buffer to memory
		std::vector<unsigned char> buff(std::istreambuf_iterator<char>(ifs), {});
		for (unsigned int i = 0; i < buff.size(); i++) {
			memory[START_PC + i] = buff[i];
		}
	}
}

// The following instructions are implemented based off technical reference: 
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#1nnn

// Clears the screen
void chip8::cls()
{
	// TODO
}

// Return from subroutine
void chip8::ret()
{
	pc = stack[sp];
	sp--;
}

// Jump to location 
void chip8::jp()
{
	pc = opcode & 0x0FFFu;
}

// Call subroutine
void chip8::call()
{
	sp++;
	stack[sp] = pc;
	pc = opcode & 0x0FFFu;
}

// Conditional skip next instruction
void chip8::se()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short byte = opcode & 0x00FFu;

	if (v[x] == byte) {
		pc += 2;
	}
}
