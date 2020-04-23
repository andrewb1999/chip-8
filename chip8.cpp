#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "chip8.hpp"

// Initialize cpu
chip8::chip8() :
	opcode {0},
	memory(MEMORY_SIZE, 0), 
	v(NUM_REGISTERS, 0), 
	I {0},	
	pc {START_PC},
	sp {0}, 
	stack(STACK_SIZE, 0),
	dt {0},
	st {0},
	keys(NUM_KEYS, 0),
    disp()
{
	// Load in fontset into memory
	for (unsigned int i = 0; i < NUM_FONTS; i++) {
		memory[FONTSET_START + i] = fontset[i];
	}

	// Clear display
	disp.disp_clear();

	// Seed rng
	srand(time(0));
}

// Execute one cycle of cpu
void chip8::executeCycle()
{
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	executeInsn(opcode);

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

// Execute insn
void chip8::executeInsn(unsigned short opcode)
{
	unsigned short msnibble = opcode >> 12u;
	unsigned short lsnibble = opcode & 0x000Fu;
	unsigned short lsbyte = opcode & 0x00FFu;
	switch (msnibble) {
		case 0x0:
			switch (lsnibble) {
				case 0x0:
					cls();
					break;
				case 0xE:
					ret();
					break;
				default:
					std::cout << "Unsupported opcode\n";
					break;
			}
			break;
		case 0x1:
			jp();
			break;
		case 0x2:
			call();
			break;
		case 0x3:
			seVal();
			break;
		case 0x4:
			sneVal();
			break;
		case 0x5:
			seReg();
			break;
		case 0x6:
			ldVal();
			break;
		case 0x7:
			addVal();
			break;
		case 0x8:
			switch (lsnibble) {
				case 0x0:
					ldReg();
					break;
				case 0x1:
					orReg();
					break;
				case 0x2: 
					andReg();
					break;
				case 0x3: 
					xorReg();
					break;
				case 0x4:
					addReg();
					break;
				case 0x5:
					subReg();
					break;
				case 0x6: 
					shrReg();
					break;
				case 0x7:
					subnReg();
					break;
				case 0xE:
					shlReg();
				default:
					std::cout << "Unsupported opcode\n";
					break;
			}
			break;
		case 0x9:
			sneReg();
			break;
		case 0xA:
			ldI();
			break;
		case 0xB:
			jpReg();
			break;
		case 0xC:
			rnd();
			break;
		case 0xD:
			drw();
			break;
		case 0xE:
			switch (lsnibble) {
				case 0xE:
					skp();
					break;
				case 0x1:
					sknp();
					break;
				default:
					std::cout << "Unsupported opcode\n";
					break;
			}
			break;
		case 0xF:
			switch (lsbyte) {
				case 0x07:
					ldDt();
					break;
				case 0x0A:
					ldKey();
					break;
				case 0x15:
					dtLd();
					break;
				case 0x18:
					stLd();
					break;
				case 0x1E:
					Iadd();
					break;
				case 0x29:
					Ild();
					break;
				case 0x33:
					Ildbcd();
					break;
				case 0x55:
					strRegs();
					break;
				case 0x65:
					ldRegs();
					break;
				default:
					std::cout << "Unsupported opcode\n";
					break;
			}
			break;
		default:
			std::cout << "Unsupported opcode\n";
			break;

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
    disp.disp_clear();
    pc += 2;
}

// Return from subroutine
void chip8::ret()
{
	sp--;
	pc = stack[sp];
	pc += 2;
}

// Jump to location 
void chip8::jp()
{
	pc = opcode & 0x0FFFu;
}

// Call subroutine
void chip8::call()
{
	stack[sp] = pc;
	sp++;
	pc = opcode & 0x0FFFu;
}

// Skip next instruction (equal to value)
void chip8::seVal()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short kk = opcode & 0x00FFu;

	if (v[x] == kk) {
		pc += 4;
	} else {
		pc += 2;
	}
}

// Skip next instruction (not equal to value)
void chip8::sneVal()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short kk = opcode & 0x00FFu;

	if (v[x] != kk) {
		pc += 4;
	} else {
		pc += 2;
	}
}

// Skip next instruction (equal regs)
void chip8::seReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	if (v[x] == v[y]) {
		pc += 4;
	} else {
		pc += 2;
	}
}

// Load value into register 
void chip8::ldVal()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short kk = opcode & 0x00FFu;

	v[x] = kk;
	pc += 2;
}

// Add value to register
void chip8::addVal()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short kk = opcode & 0x00FFu;

	v[x] += kk;	
	pc += 2;
}

// Load register value into another register
void chip8::ldReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[x] = v[y];
	pc += 2;
}

// Or register with another register
void chip8::orReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[x] |= v[y];
	pc += 2;
}

// And register with another register
void chip8::andReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[x] &= v[y];
	pc += 2;
}

// Xor register with another register
void chip8::xorReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[x] ^= v[y];	
	pc += 2;
}

// Add register with another register
void chip8::addReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	unsigned short sum = v[x] + v[y];
	v[0xF] = (sum > 0x00FFu);
	v[x] = sum & 0x00FFu;
	pc += 2;
}

// Subtract a register value
void chip8::subReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	v[0xF] = (v[x] > v[y]);
	v[x] -= v[y];
	pc += 2;
}

// Shift bits of a register right 1
void chip8::shrReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	v[0xF] = v[x] & 0x1u;
	v[x] >>= 1;
	pc += 2;
}

// Subtract this register from another one
void chip8::subnReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;	
	v[0xF] = (v[y] > v[x]);
	v[x] = v[y] - v[x];
	pc += 2;
}

// Shift bits of a register left 1
void chip8::shlReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	v[0xF] = v[x] >> 7u;
	v[x] <<= 1;
	pc += 2;
}

// Skip next instruction (not equal regs)
void chip8::sneReg()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;
	unsigned short y = (opcode & 0x00F0u) >> 4u;

	if (v[x] != v[y]) {
		pc += 4;
	} else {
		pc += 2;
	}
}

// Load value into index register
void chip8::ldI() 
{
	I = opcode & 0x0FFFu;
	pc += 2;
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
	pc += 2;
}

// Draw sprite to screen
void chip8::drw()
{
    unsigned short vx = (opcode & 0x0F00u) >> 8u;
    unsigned short vy = (opcode & 0x00F0u) >> 4u;
    unsigned short n = (opcode & 0x000Fu);

    auto begin = memory.begin() + I;
    auto end = begin + n + 1;
    sprite s {begin, end};
	disp.draw(s, vx, vy, n, v[15]);

	pc += 2;
}

// Skip next instruction (key pressed)
void chip8::skp() 
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	if (keys[v[x]]) {
		pc += 4;
	} else {
		pc += 2;
	}
}

// Skip next instruction (key not pressed)
void chip8::sknp() 
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	if (!keys[v[x]]) {
		pc += 4;
	} else {
		pc += 2;
	}
}

// Load delay timer into register
void chip8::ldDt()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	v[x] = dt;

	pc += 2;
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
		return;
	}

	pc += 2;
}

// Load register into delay timer
void chip8::dtLd() 
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	dt = v[x];

	pc += 2;
}

// Load register into sound timer
void chip8::stLd()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	st = v[x];

	pc += 2;
}

// Add I with a register
void chip8::Iadd()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	I += v[x];

	pc += 2;
}

// Set I to be location of sprite
void chip8::Ild()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	I = FONTSET_START + (v[x] * SPRITE_BYTES);

	pc += 2;
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

	pc += 2;
}

// Store the values of registers v[0]...v[x] into memory
void chip8::strRegs()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	for (unsigned short i = 0; i <= x; i++) {
		memory[I + i] = v[i];
	}

	pc += 2;
}

// Load values from memory into registers v[0]...v[x]
void chip8::ldRegs()
{
	unsigned short x = (opcode & 0x0F00u) >> 8u;

	for (unsigned short i = 0; i <= x; i++) {
		v[i] = memory[I + i];
	}

	pc += 2;
}
