#include <vector>
#include "screen.hpp"

/*
** This class contains all cpu logic for chip8 emulation
*/
class chip8
{
private:
	// Current 2-byte opcode
	unsigned short opcode;

	// 4K memory
	std::vector<unsigned char> memory;

	// 16 8-bit registers
	std::vector<unsigned char> v;

	// Index register and program counter
	unsigned short i;
	unsigned short pc;

	// Stack pointer and execution stack
	unsigned short sp;
	std::vector<unsigned short> stack;

	// Delay and sound 60 Hz timer registers
	unsigned char dt;
	unsigned char st;
public:
	chip8(const screen&);
	void executeCycle();
};