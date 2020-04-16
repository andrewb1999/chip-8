#include "chip8.hpp"
chip8::chip8(const screen &s) :
	opcode {0},
	memory(4096), 
	v(16), 
	i {0},	
	pc {0x200},
	sp {0}, 
	stack(16)
{

}

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