#pragma once

#include <string_view>
#include <iostream>
#include <cstdio>

#ifdef _WIN32
	#include <windows.h>  // for WinAPI and Sleep()
    #include <minwindef.h>
    #include <wincon.h>
#else
	#include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
	#include <time.h>   /* for nanosleep() */
#endif

namespace terminal_ctrl {

enum class ColorCode {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	GREY,
	DARKGREY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
};

enum class FontStyle {
    NORMAL,
    BOLD,
    FAINT,
    ITALIC,
    UNDERLINE,
    BLINK,
    INVERSE,
    HIDDEN,
    STRIKETHROUGH
};

constexpr std::string_view ansi_code_from_font_style(FontStyle const style) {
    switch (style)
    {
        case FontStyle::NORMAL: return "\033[0m";
        case FontStyle::BOLD: return "\033[1m";
        case FontStyle::FAINT: return "\033[2m";
        case FontStyle::ITALIC: return "\033[3m";
        case FontStyle::UNDERLINE: return "\033[4m";
        case FontStyle::BLINK: return "\033[5m";
        case FontStyle::INVERSE: return "\033[7m";
        case FontStyle::HIDDEN: return "\033[8m";
        case FontStyle::STRIKETHROUGH: return "\033[9m";
        default: return "\033[0m"; // Normal
    }
}

constexpr std::string_view color_code_to_string_view(ColorCode const color_code) {
    switch (color_code) {
        case ColorCode::BLACK: return "BLACK";
        case ColorCode::BLUE: return "BLUE";
        case ColorCode::GREEN: return "GREEN";
        case ColorCode::RED: return "RED";
        case ColorCode::CYAN: return "CYAN";
        case ColorCode::MAGENTA: return "MAGENTA";
        case ColorCode::BROWN: return "BROWN";
        case ColorCode::GREY: return "GREY";
        case ColorCode::DARKGREY: return "DARKGREY";
        case ColorCode::LIGHTBLUE: return "LIGHTBLUE";
        case ColorCode::LIGHTGREEN: return "LIGHTGREEN";
        case ColorCode::LIGHTCYAN: return "LIGHTCYAN";
        case ColorCode::LIGHTRED: return "LIGHTRED";
        case ColorCode::LIGHTMAGENTA: return "LIGHTMAGENTA";
        case ColorCode::YELLOW: return "YELLOW";
        case ColorCode::WHITE: return "WHITE";
    }
    return "UNKNOWN";
}

constexpr std::string_view ansi_code_from_color_code(ColorCode const color) {
    switch (color)
    {
        case ColorCode::BLACK: return "\033[22;30m";
        case ColorCode::RED: return "\033[22;31m";
        case ColorCode::GREEN: return "\033[22;32m";
        case ColorCode::BROWN: return "\033[22;33m";
        case ColorCode::BLUE: return "\033[22;34m";
        case ColorCode::MAGENTA: return "\033[22;35m";
        case ColorCode::CYAN: return "\033[22;36m";
        case ColorCode::GREY: return "\033[22;37m";

        case ColorCode::DARKGREY: return "\033[1;30m";
        case ColorCode::LIGHTRED: return "\033[1;31m";
        case ColorCode::LIGHTGREEN: return "\033[1;32m";
        case ColorCode::YELLOW: return "\033[1;33m";
        case ColorCode::LIGHTBLUE: return "\033[1;34m";
        case ColorCode::LIGHTMAGENTA: return "\033[1;35m";
        case ColorCode::LIGHTCYAN: return "\033[1;36m";
        case ColorCode::WHITE: return "\033[1;37m";

        default: return "\033[22;37m"; // Grey
    }
}

constexpr std::string_view background_ansi_code_from_color_code(ColorCode const color) {
    switch (color)
    {
        case ColorCode::BLACK: return "\033[40m";
        case ColorCode::RED: return "\033[41m";
        case ColorCode::GREEN: return "\033[42m";
        case ColorCode::YELLOW: return "\033[43m";
        case ColorCode::BLUE: return "\033[44m";
        case ColorCode::MAGENTA: return "\033[45m";
        case ColorCode::CYAN: return "\033[46m";
        case ColorCode::WHITE: return "\033[47m";
        default: return "\033[40m"; // Black
    }
}

constexpr std::string_view font_style_to_string_view(FontStyle const style) {
	switch (style)
	{
		case FontStyle::NORMAL: return "NORMAL";
		case FontStyle::BOLD: return "BOLD";
		case FontStyle::FAINT: return "FAINT";
		case FontStyle::ITALIC: return "ITALIC";
		case FontStyle::UNDERLINE: return "UNDERLINE";
		case FontStyle::BLINK: return "BLINK";
		case FontStyle::INVERSE: return "INVERSE";
		case FontStyle::HIDDEN: return "HIDDEN";
		case FontStyle::STRIKETHROUGH: return "STRIKETHROUGH";
	}
	return "UNKNOWN";
}

#if !defined(_WIN32)
// Constant for ANSI sequences on non-Windows systems
constexpr std::string_view ANSI_CLS {"\033[2J\033[3J"};
constexpr std::string_view ANSI_ATTRIBUTE_RESET {"\033[0m"};

constexpr std::string_view ANSI_CONSOLE_TITLE_PRE {"\033]0};"};
constexpr std::string_view ANSI_CONSOLE_TITLE_POST {"\007"};

constexpr std::string_view ANSI_CURSOR_HIDE {"\033[?25l"};
constexpr std::string_view ANSI_CURSOR_SHOW {"\033[?25h"};
constexpr std::string_view ANSI_CURSOR_HOME {"\033[H"};
#endif

inline bool get_cursor_position(unsigned int& x, unsigned int& y) {
#if defined(_WIN32)
    const HANDLE consoleHandle { GetStdHandle(STD_OUTPUT_HANDLE) };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(consoleHandle, &csbi)) return false;
    x = static_cast<unsigned int>(csbi.dwCursorPosition.X) + 1;
    y = static_cast<unsigned int>(csbi.dwCursorPosition.Y) + 1;
#else
    //Use ANSI "\033[6n"; will print something like: "\033[X;YR" to the console

    char buf[30]={0};
    int i, pow;
    char ch {};

    struct termios term, restore;

    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(0, TCSANOW, &term);

    write(1, "\033[6n", 4);

    for(int i = 0, ch = 0; ch != 'R'; i++)
    {
        if (!read(0, &ch, 1)) {
            tcsetattr(0, TCSANOW, &restore);
            // Error reading response from terminal
            return false;
        }
        buf[i] = ch;
    }

    if (i < 2) {
        tcsetattr(0, TCSANOW, &restore);
        // Error reading response from terminal
        return false;
    }

    x = 0;
    for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10) {
        x += ( buf[i] - '0' ) * pow;
    }

    y = 0;
    for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10) {
        y += ( buf[i] - '0' ) * pow;
    }

    // sscanf(buff, "\x1b[%d;%dR", x, y);

    tcsetattr(0, TCSANOW, &restore);
#endif
    return true;
}

inline bool set_cursor_position(unsigned int const x, unsigned int const y) {
#if defined(_WIN32)
    const HANDLE consoleHandle { GetStdHandle(STD_OUTPUT_HANDLE) };
    // Windows uses 0-based coordinates
    COORD coord { static_cast<SHORT>(x-1), static_cast<SHORT>(y-1) };
    return SetConsoleCursorPosition(consoleHandle, coord);
#else
    // Use ANSI escape codes
    printf("\033[%d;%dH", y, x);
    return true;
#endif
}

inline bool get_terminal_size(unsigned int &width, unsigned int &height) {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) return false;
    width = static_cast<unsigned int>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    height = static_cast<unsigned int>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#else

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;

#ifdef TIOCGSIZE
    // ttysize should be obsolete, but it's still used in some systems
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    width = ts.ts_cols;
    height = ts.ts_lines;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    width = ts.ws_col;
    height = ts.ws_row;
#else
    // Error: unable to get terminal size
    return false;
#endif // TIOCGSIZE
#endif // _WIN32
    return true;
}

// Based on https://learn.microsoft.com/en-us/windows/console/clearing-the-screen
inline bool clear_screen() {
#if defined(_WIN32)
    const HANDLE consoleHandle { GetStdHandle(STD_OUTPUT_HANDLE) };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD chars_written;
    COORD upper_left { 0, 0 };

    if (!GetConsoleScreenBufferInfo(consoleHandle, &csbi)) return false;
    const DWORD console_size { static_cast<DWORD>(csbi.dwSize.X) * static_cast<DWORD>(csbi.dwSize.Y) };
    if (!FillConsoleOutputCharacter(consoleHandle, ' ', console_size, upper_left, &chars_written)) return false;
    if (!FillConsoleOutputAttribute(consoleHandle, csbi.wAttributes, console_size, upper_left, &chars_written)) return false;
    SetConsoleCursorPosition(consoleHandle, upper_left);
#else
    std::cout << ANSI_CLS << ANSI_CURSOR_HOME;
#endif
    return true;
}

inline void sleep(unsigned int const milliseconds) {
#if defined(_WIN32)
    Sleep(milliseconds);
#else
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000L;
    nanosleep(&ts, nullptr);
#endif
}

inline void set_title(std::string_view const& title) {
#if defined(_WIN32)
    SetConsoleTitle(title.data());
#else
    std::cout << ANSI_CONSOLE_TITLE_PRE << title << ANSI_CONSOLE_TITLE_POST;
#endif
}


inline void set_cursor_visibility(bool visible) {
#if defined(_WIN32)
	const HANDLE consoleHandle { GetStdHandle(STD_OUTPUT_HANDLE) };
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(consoleHandle, &structCursorInfo);
	structCursorInfo.bVisible = visible;
	SetConsoleCursorInfo(consoleHandle, &structCursorInfo);
#else
	std::cout << (visible ? ANSI_CURSOR_SHOW : ANSI_CURSOR_HIDE);
#endif
}

inline void set_text_color(ColorCode const color) {
#if defined(_WIN32)
    const HANDLE consoleHandle { GetStdHandle(STD_OUTPUT_HANDLE) };
    CONSOLE_SCREEN_BUFFER_INFO csbi {};
	GetConsoleScreenBufferInfo(consoleHandle, &csbi);
    // Foreground colors take up the least significant byte
	SetConsoleTextAttribute(consoleHandle, (csbi.wAttributes & 0xFFF0) | static_cast<WORD>(color));
#else
    std::cout << ansi_code_from_color_code(color);
#endif
}

inline void set_background_color(ColorCode const color) {
#if defined(_WIN32)
    const HANDLE consoleHandle { GetStdHandle(STD_OUTPUT_HANDLE) };
    CONSOLE_SCREEN_BUFFER_INFO csbi {};
    GetConsoleScreenBufferInfo(consoleHandle, &csbi);
    // Background colors take the second least significant byte
    SetConsoleTextAttribute(consoleHandle, (csbi.wAttributes & 0xFF0F) | (static_cast<WORD>(color) << 4));
#else
    std::cout << background_ansi_code_from_color_code(color);
#endif
}

inline void set_font_style(FontStyle const style) {
    std::cout << ansi_code_from_font_style(style);
}

// Must be call at the beginning of the program in order to save the default attributes and work properly on windows
inline unsigned int save_default_attributes()
{
#if defined(_WIN32)
	static bool initialized {false};
	static WORD attributes;

	if (!initialized) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		attributes = csbi.wAttributes;
		initialized = true;
	}
	return attributes;
#else
    return 0;
#endif
}

inline void restore_default_attributes() {
#if defined(_WIN32)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), save_default_attributes());
#else
    std::cout << ANSI_ATTRIBUTE_RESET;
#endif
}

} // namespace TerminalCtrl
