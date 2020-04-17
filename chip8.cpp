#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "chip8.hpp"

// Initialize cpu
chip8::chip8(const screen &s) :
	opcode {0},
	memory(MEMORY_SIZE, 0), 
	v(NUM_REGISTERS, 0), 
	I {0},	
	pc {START_PC},
	sp {0}, 
	stack(STACK_SIZE, 0),
	dt {0},
	st {0},
	keys(NUM_KEYS, 0)
{
	// Load in fontset into memory
	for (unsigned int i = 0; i < NUM_FONTS; i++) {
		memory[FONTSET_START + i] = fontset[i];
	}

	// Seed rng
	srand(time(0));
}

// Execute one cycle of cpu
void chip8::executeCycle()
{
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	if (dt > 0) {
		dt--;
	}

	if (st > 0) {
		if (st == 1) {
			// TODO: Do something with sound
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

// Skip next instruction (equal to value)
void chip8::seVal()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short kk = opcode & 0x00FFu;

	if (v[x] == kk) {
		pc += 2;
	}
}

// Skip next instruction (not equal to value)
void chip8::sneVal()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short kk = opcode & 0x00FFu;

	if (v[x] != kk) {
		pc += 2;
	}
}

// Skip next instruction (equal regs)
void chip8::seReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	if (v[x] == v[y]) {
		pc += 2;
	}
}

// Load value into register 
void chip8::ldVal()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short kk = opcode & 0x00FFu;

	v[x] = kk;
}

// Add value to register
void chip8::addVal()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short kk = opcode & 0x00FFu;

	v[x] += kk;	
}

// Load register value into another register
void chip8::ldReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[x] = v[y];
}

// Or register with another register
void chip8::orReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[x] |= v[y];
}

// And register with another register
void chip8::andReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[x] &= v[y];
}

// Xor register with another register
void chip8::xorReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[x] ^= v[y];	
}

// Add register with another register
void chip8::addReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	unsigned short sum = v[x] + v[y];
	v[0xF] = (sum > 0x00FFu);
	v[x] = sum & 0x00FFu;
}

// Subtract a register value
void chip8::subReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[0xF] = (v[x] > v[y]);
	v[x] -= v[y];
}

// Shift bits of a register right 1
void chip8::shrReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	v[0xF] = v[x] & 0x1u;
	v[x] >>= 1;
}

// Subtract this register from another one
void chip8::subnReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;	
	v[0xF] = (v[y] > v[x]);
	v[x] = v[y] - v[x];
}

// Shift bits of a register left 1
void chip8::shlReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	v[0xF] = v[x] >> 7u;
	v[x] <<= 1;
}

// Skip next instruction (not equal regs)
void chip8::sneReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	if (v[x] != v[y]) {
		pc += 2;
	}
}

// Load value into index register
void chip8::ldI() 
{
	I = opcode & 0x0FFFu;
}

// Jump to location offset by register value V0
void chip8::jpReg()
{
	pc = (opcode & 0x0FFFu) + v[0];
}

// And random byte with value
void chip8::rnd()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short kk = opcode & 0x00FFu;

	v[x] = (rand() % 256) & kk;
}

// Draw sprite to screen
void chip8::drw()
{
	// TODO
}

// Skip next instruction (key pressed)
void chip8::skp() 
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	if (keys[v[x]]) {
		pc += 2;
	}
}

// Skip next instruction (key not pressed)
void chip8::sknp() 
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	if (!keys[v[x]]) {
		pc += 2;
	}
}

// Load delay timer into register
void chip8::ldDt()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	v[x] = dt;
}

// Wait for key press, store value into register
void chip8::ldKey()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	unsigned int keyPressed = 0;
	for (unsigned int i = 0; i < NUM_KEYS; i++) {
		if (keys[i]) {
			v[x] = 0;
			keyPressed = 1;
			break;
		}
	}

	if (!keyPressed) {
		pc -= 2;
	}
}

// Load register into delay timer
void chip8::dtLd() 
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	dt = v[x];
}

// Load register into sound timer
void chip8::stLd()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	st = v[x];
}

// Add I with a register
void chip8::Iadd()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	I += v[x];
}

// Set I to be location of sprite
void chip8::Ild()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	I = FONTSET_START + (v[x] * SPRITE_BYTES);
}

// Set I, I+1, and I+2 in memory to be BCD representation of a register
void chip8::Ildbcd()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	int n = v[x];

	for (unsigned short i = 0; i <= 2; i++) {
		unsigned short j = 2 - i;
		memory[I + j] = n % 10;
		n /= 10;
	}
}

// Store the values of registers v[0]...v[x] into memory
void chip8::strRegs()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	for (unsigned short i = 0; i <= x; i++) {
		memory[I + i] = v[i];
	}
}

// Load values from memory into registers v[0]...v[x]
void chip8::ldRegs()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	for (unsigned short i = 0; i <= x; i++) {
		v[i] = memory[I + i];
	}
}