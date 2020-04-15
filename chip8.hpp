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
	std::vector<unsigned char> memory(4096);

	// 16 8-bit registers
	std::vector<unsigned char> v(16);

	// Index register and program counter
	unsigned short I;
	unsigned short pc;

	// Stack pointer and execution stack
	unsigned short sp;
	std::vector<unsigned short> stack(16);
public:
	chip8(screen&);
	void execute();
}