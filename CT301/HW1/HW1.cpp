#include <iostream>

int main(int argc, char* argv[])
{

    if (argc != 1) {
        std::cerr << "Error: no arguments should be provided" << std::endl;
        return -1;
    }

    std::cout << "Hello World!" << std::endl;
    return 0;
}