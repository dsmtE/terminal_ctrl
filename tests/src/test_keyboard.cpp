#include "terminal_ctrl.hpp"

#include <iostream>

int main() {
    terminal_ctrl::KeyCode key {terminal_ctrl::blocking_get_key() };
    std::cout << "Key pressed: " << key << std::endl;

    // int k { getch()};
    // std::cout << "Key pressed: " << k;
    // while (kbhit()) {
    //     k = getch();

    //     std::cout << " " << k;
    // }
    // std::cout << std::endl;

    return 0;
}