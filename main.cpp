#include "chip8.hpp"
#include <chrono>
#include <thread>

using namespace std;

// Outer emulation loop
int main(int argc, char *argv[])
{
	chip8 processor;

    processor.readRom("./games/BRIX");
    processor.readSound("./beep.wav");

    while (true) {
        chrono::high_resolution_clock::time_point before = chrono::high_resolution_clock::now();
        processor.executeCycle();
        chrono::high_resolution_clock::time_point after = chrono::high_resolution_clock::now();
        chrono::duration<double, std::micro> time_span = after - before;
        if (time_span.count() < 2000) {
            std::this_thread::sleep_for(
                    chrono::microseconds(2000 - ((int) time_span.count())));
        }
    }
}
