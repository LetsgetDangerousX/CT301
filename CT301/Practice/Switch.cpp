//Format : switch (<variable>) {case "" case "" default}

#include <iostream>

int main() {
    int number = 1;

    switch(number){
        case 1:
            std::cout << "The number is one\n";
            break;
        case 2: 
            std::cout << "The number is one\n";
            break;
        default:
            std::cout<< "Out of bounds";
    }
}