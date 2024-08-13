
#include <iostream>
#include <string>

#include "terminal_ctrl.hpp"

int main() {
    std::string const hello {"Hello, World!"};
    std::cout << hello << std::endl;

    terminal_ctrl::clear_screen();

    terminal_ctrl::set_cursor_position(4, 3);
    std::cout << hello;

    unsigned int x, y;
    terminal_ctrl::get_cursor_position(x, y);

    bool const success {x == (4 + hello.size()) && y == 3};

    std::cout << std::endl << "Cursor position: " << x << ", " << y << " (" << (success ? "success" : "failure") << ")" << std::endl;

    unsigned int width, height;
    terminal_ctrl::get_terminal_size(width, height);
    std::cout << "Terminal size: " << width << "x" << height << std::endl;
}