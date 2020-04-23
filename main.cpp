#include "chip8.hpp"

// Outer emulation loop
int main(int argc, char *argv[])
{
	chip8 processor;

    processor.readRom("./games/PONG");

    while (true) {
        processor.executeCycle();
    }
}
