
#include <iostream>

#include "terminal_ctrl.hpp"

int main() {
    using terminal_ctrl::ColorCode;
    using terminal_ctrl::FontStyle;

    terminal_ctrl::save_default_attributes();

    terminal_ctrl::set_title("test styles");

    for (int i {0}; i < 16; i++) {
        terminal_ctrl::set_text_color(static_cast<ColorCode>(i));
        std::cout << color_code_to_string_view(static_cast<ColorCode>(i)) << "(" << i << ") ";
        if(i == 7) std::cout << std::endl;
    }
    std::cout << std::endl;

    // test background colors
    terminal_ctrl::set_text_color(ColorCode::WHITE);
    for (int i {0}; i < 16; i++) {
        terminal_ctrl::set_background_color(static_cast<ColorCode>(i));
        std::cout << color_code_to_string_view(static_cast<ColorCode>(i)) << "(" << i << ") ";
        if(i == 7) {
            std::cout << std::endl;
            terminal_ctrl::set_text_color(ColorCode::BLACK);
        }
    }
    terminal_ctrl::restore_default_attributes();
    std::cout << std::endl;

    for (int i {0}; i < 9; i++) {
        terminal_ctrl::set_font_style(static_cast<FontStyle>(i));
        std::cout << font_style_to_string_view(static_cast<FontStyle>(i)) << std::endl;
        terminal_ctrl::restore_default_attributes();
    }

    // combination  test
    terminal_ctrl::set_text_color(ColorCode::RED);
    terminal_ctrl::set_background_color(ColorCode::LIGHTBLUE);
    terminal_ctrl::set_font_style(FontStyle::ITALIC);
    terminal_ctrl::set_font_style(FontStyle::STRIKETHROUGH);
    std::cout << "Red text on light blue background with italic and strikethrough" << std::endl;
    terminal_ctrl::restore_default_attributes();
}