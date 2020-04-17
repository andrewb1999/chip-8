#include <chrono>
#include <random>
#include <string>
#include <vector>
#include "screen.hpp"

/*
** This class contains all cpu logic for chip8 emulation
*/
class chip8
{
private:
	// Constants
	const unsigned short START_PC = 0x200;
	const unsigned int MEMORY_SIZE = 4096;
	const unsigned int NUM_REGISTERS = 16;
	const unsigned int STACK_SIZE = 16;
	const unsigned int NUM_FONTS = 80;
	const unsigned int FONTSET_START = 0x50;
	const unsigned int NUM_KEYS = 16;
	const unsigned int SPRITE_BYTES = 5;

	// Fontset
	unsigned char fontset[80] =
	{
	    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	    0x20, 0x60, 0x20, 0x20, 0x70, //1
	    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	    0x90, 0x90, 0xF0, 0x10, 0x10, //4
	    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	    0xF0, 0x10, 0x20, 0x40, 0x40, //7
	    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};
 
	// Current 2-byte opcode
	unsigned short opcode;

	// 4K memory
	std::vector<unsigned char> memory;

	// 16 8-bit registers
	std::vector<unsigned char> v;

	// Index register and program counter
	unsigned short I;
	unsigned short pc;

	// Stack pointer and execution stack
	unsigned short sp;
	std::vector<unsigned short> stack;

	// Delay and sound timer registers
	unsigned char dt;
	unsigned char st;

	// Keypad
	std::vector<unsigned char> keys;
public:
	// Initialize cpu
	chip8(const screen&);

	// Execute one cycle of cpu
	void executeCycle();

	// Read a ROM file into memory
	void readRom(const std::string&);

	// Clears the screen
	void cls();

	// Return from subroutine
	void ret();

	// Jump to location
	void jp();

	// Call subroutine
	void call();

	// Skip next instruction (equal to value)
	void seVal();

	// Skip next instruction (not equal to value)
	void sneVal();

	// Skip next instruction (equal regs)
	void seReg();

	// Load value into register
	void ldVal();

	// Add value to register
	void addVal();

	// Load register value into another register
	void ldReg();

	// Or register with another register
	void orReg();

	// And register with another register
	void andReg();

	// Xor register with another register
	void xorReg();

	// Add register with another register
	void addReg();

	// Subtract a register from this register
	void subReg();

	// Shift bits of a register right 1
	void shrReg();

	// Subtract this register from another one
	void subnReg();

	// Shift bits of a register left 1
	void shlReg();

	// Skip next instruction (not equal regs)
	void sneReg();

	// Load value into index register
	void ldI();

	// Jump to location offset by register value V0
	void jpReg();

	// And random byte with value
	void rnd();

	// Draw sprite to screen
	void drw();

	// Skip next instruction (key pressed)
	void skp();

	// Skip next instruction (key not pressed)
	void sknp();

	// Load delay timer into register
	void ldDt();

	// Wait for key press, store value into register
	void ldKey();

	// Load register into delay timer
	void dtLd();

	// Load register into sound timer
	void stLd();

	// Add I with a register
	void Iadd();

	// Set I to be location of sprite from a register
	void Ild();

	// Set I, I+1, and I+2 in memory to be BCD representation of a register
	void Ildbcd();

	// Store the values of registers v[0]...v[x] into memory
	void strRegs();

	// Load values from memory into registers v[0]...v[x]
	void ldRegs();
};