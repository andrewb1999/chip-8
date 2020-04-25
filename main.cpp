#include "chip8.hpp"
#include <chrono>
#include <stdexcept>
#include <thread>
#include <iostream>

using namespace std;

// Outer emulation loop
int main(int argc, char *argv[])
{
    if (argc != 2) {
        cout << "Please include ROM file" << endl;
        return 1;
    }

	chip8 processor;
    
    try {
        processor.readRom(argv[1]);
    } catch (const std::runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }
    processor.readSound("./beep.wav");

    bool end = false;

    while (!end) {
    	end = processor.pollInput();
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
