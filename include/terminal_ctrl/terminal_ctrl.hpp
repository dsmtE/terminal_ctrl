#pragma once

#include <string_view>
#include <iostream>
#include <cstdio>
#include <optional>

#ifdef _WIN32
	#include <windows.h>  // for WinAPI and Sleep()
    #include <minwindef.h>
    #include <wincon.h>
    #include <conio.h>    // for getch() and kbhit()

    #define getch _getch
	#define kbhit _kbhit
#else
	#include <termios.h>
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

constexpr std::string_view ANSI_ATTRIBUTE_RESET {"\033[0m"};

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

enum class KeyCode {

    KEY_a,
    KEY_b,
    KEY_c,
    KEY_d,
    KEY_e,
    KEY_f,
    KEY_g,
    KEY_h,
    KEY_i,
    KEY_j,
    KEY_k,
    KEY_l,
    KEY_m,
    KEY_n,
    KEY_o,
    KEY_p,
    KEY_q,
    KEY_r,
    KEY_s,
    KEY_t,
    KEY_u,
    KEY_v,
    KEY_w,
    KEY_x,
    KEY_y,
    KEY_z,

    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

	KEY_ESCAPE,
	KEY_ENTER,
	KEY_SPACE,

	KEY_INSERT,
	KEY_HOME,
	KEY_PGUP,
	KEY_DELETE,
	KEY_END,
	KEY_PGDOWN,
    KEY_TAB,

	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,

    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

	KEY_NUMDEL,
	KEY_NUMPAD0,
	KEY_NUMPAD1,
	KEY_NUMPAD2,
	KEY_NUMPAD3,
	KEY_NUMPAD4,
	KEY_NUMPAD5,
	KEY_NUMPAD6,
	KEY_NUMPAD7,
	KEY_NUMPAD8,
	KEY_NUMPAD9,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
};

constexpr std::string_view KeyCodeToString(KeyCode const key) {
    switch (key) {
        case KeyCode::KEY_a: return "a";
        case KeyCode::KEY_b: return "b";
        case KeyCode::KEY_c: return "c";
        case KeyCode::KEY_d: return "d";
        case KeyCode::KEY_e: return "e";
        case KeyCode::KEY_f: return "f";
        case KeyCode::KEY_g: return "g";
        case KeyCode::KEY_h: return "h";
        case KeyCode::KEY_i: return "i";
        case KeyCode::KEY_j: return "j";
        case KeyCode::KEY_k: return "k";
        case KeyCode::KEY_l: return "l";
        case KeyCode::KEY_m: return "m";
        case KeyCode::KEY_n: return "n";
        case KeyCode::KEY_o: return "o";
        case KeyCode::KEY_p: return "p";
        case KeyCode::KEY_q: return "q";
        case KeyCode::KEY_r: return "r";
        case KeyCode::KEY_s: return "s";
        case KeyCode::KEY_t: return "t";
        case KeyCode::KEY_u: return "u";
        case KeyCode::KEY_v: return "v";
        case KeyCode::KEY_w: return "w";
        case KeyCode::KEY_x: return "x";
        case KeyCode::KEY_y: return "y";
        case KeyCode::KEY_z: return "z";

        case KeyCode::KEY_A: return "A";
        case KeyCode::KEY_B: return "B";
        case KeyCode::KEY_C: return "C";
        case KeyCode::KEY_D: return "D";
        case KeyCode::KEY_E: return "E";
        case KeyCode::KEY_F: return "F";
        case KeyCode::KEY_G: return "G";
        case KeyCode::KEY_H: return "H";
        case KeyCode::KEY_I: return "I";
        case KeyCode::KEY_J: return "J";
        case KeyCode::KEY_K: return "K";
        case KeyCode::KEY_L: return "L";
        case KeyCode::KEY_M: return "M";
        case KeyCode::KEY_N: return "N";
        case KeyCode::KEY_O: return "O";
        case KeyCode::KEY_P: return "P";
        case KeyCode::KEY_Q: return "Q";
        case KeyCode::KEY_R: return "R";
        case KeyCode::KEY_S: return "S";
        case KeyCode::KEY_T: return "T";
        case KeyCode::KEY_U: return "U";
        case KeyCode::KEY_V: return "V";
        case KeyCode::KEY_W: return "W";
        case KeyCode::KEY_X: return "X";
        case KeyCode::KEY_Y: return "Y";
        case KeyCode::KEY_Z: return "Z";

        case KeyCode::KEY_ESCAPE: return "ESCAPE";
        case KeyCode::KEY_ENTER: return "ENTER";
        case KeyCode::KEY_SPACE: return "SPACE";

        case KeyCode::KEY_INSERT: return "INSERT";
        case KeyCode::KEY_HOME: return "HOME";
        case KeyCode::KEY_PGUP: return "PGUP";
        case KeyCode::KEY_DELETE: return "DELETE";
        case KeyCode::KEY_END: return "END";
        case KeyCode::KEY_PGDOWN: return "PGDOWN";
        case KeyCode::KEY_TAB: return "TAB";

        case KeyCode::KEY_UP: return "UP";
        case KeyCode::KEY_DOWN: return "DOWN";
        case KeyCode::KEY_LEFT: return "LEFT";
        case KeyCode::KEY_RIGHT: return "RIGHT";

        case KeyCode::KEY_0: return "0";
        case KeyCode::KEY_1: return "1";
        case KeyCode::KEY_2: return "2";
        case KeyCode::KEY_3: return "3";
        case KeyCode::KEY_4: return "4";
        case KeyCode::KEY_5: return "5";
        case KeyCode::KEY_6: return "6";
        case KeyCode::KEY_7: return "7";
        case KeyCode::KEY_8: return "8";
        case KeyCode::KEY_9: return "9";

        case KeyCode::KEY_NUMDEL: return "NUMDEL";
        case KeyCode::KEY_NUMPAD0: return "NUMPAD0";
        case KeyCode::KEY_NUMPAD1: return "NUMPAD1";
        case KeyCode::KEY_NUMPAD2: return "NUMPAD2";
        case KeyCode::KEY_NUMPAD3: return "NUMPAD3";
        case KeyCode::KEY_NUMPAD4: return "NUMPAD4";
        case KeyCode::KEY_NUMPAD5: return "NUMPAD5";
        case KeyCode::KEY_NUMPAD6: return "NUMPAD6";
        case KeyCode::KEY_NUMPAD7: return "NUMPAD7";
        case KeyCode::KEY_NUMPAD8: return "NUMPAD8";
        case KeyCode::KEY_NUMPAD9: return "NUMPAD9";

        case KeyCode::KEY_F1 : return "F1";
        case KeyCode::KEY_F2 : return "F2";
        case KeyCode::KEY_F3 : return "F3";
        case KeyCode::KEY_F4 : return "F4";
        case KeyCode::KEY_F5 : return "F5";
        case KeyCode::KEY_F6 : return "F6";
        case KeyCode::KEY_F7 : return "F7";
        case KeyCode::KEY_F8 : return "F8";
        case KeyCode::KEY_F9 : return "F9";
        case KeyCode::KEY_F10: return "F10";
        case KeyCode::KEY_F11: return "F11";
        case KeyCode::KEY_F12: return "F12";

        default: return "UNKNOWN";
    }
}

// Steam operator for KeyCode
inline std::ostream& operator<<(std::ostream& os, KeyCode const key) {
    return os << KeyCodeToString(key);
}

/* Functions covered by Window's conio.h */
#ifndef _WIN32

/**
 * @brief Get a charater without waiting on a Return
 * @details Windows has this functionality in conio.h
 * @return The character
 */
int getch(void) {
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
/**
 * @brief Determines if a button was pressed.
 * @details Windows has this in conio.h
 * @return Number of characters read
 */
int kbhit(void) {
	static struct termios oldt, newt;
	int cnt = 0;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag    &= ~(ICANON | ECHO);
	newt.c_iflag     = 0; /* input mode */
	newt.c_oflag     = 0; /* output mode */
	newt.c_cc[VMIN]  = 1; /* minimum time to wait */
	newt.c_cc[VTIME] = 1; /* minimum characters to wait for */
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ioctl(0, FIONREAD, &cnt); /* Read count */
	struct timeval tv;
	tv.tv_sec  = 0;
	tv.tv_usec = 100;
	select(STDIN_FILENO+1, NULL, NULL, NULL, &tv); /* A small time delay */
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return cnt; /* Return number of characters */
}
#endif /* _WIN32 */


inline KeyCode blocking_get_key() {
#ifndef _WIN32
	int cnt { kbhit() }; // for ANSI escapes processing
#endif
    int k { getch() };
    if(k >= 97 && k < 123) { //lowercase
        return static_cast<KeyCode>(static_cast<int>(KeyCode::KEY_a) + k - 97);
    }

    if(k >= 65 && k < 91) { //uppercase
        return static_cast<KeyCode>(static_cast<int>(KeyCode::KEY_A) + k - 65);
    }

	switch(k) {
        case 0: {
            int kk;
            switch (kk = getch()) {
            case 71: return KeyCode::KEY_NUMPAD7;
            case 72: return KeyCode::KEY_NUMPAD8;
            case 73: return KeyCode::KEY_NUMPAD9;
            case 75: return KeyCode::KEY_NUMPAD4;
            case 77: return KeyCode::KEY_NUMPAD6;
            case 79: return KeyCode::KEY_NUMPAD1;
            case 80: return KeyCode::KEY_NUMPAD2;
            case 81: return KeyCode::KEY_NUMPAD3;
            case 82: return KeyCode::KEY_NUMPAD0;
            case 83: return KeyCode::KEY_NUMDEL;
            default:
                return static_cast<KeyCode>(kk-59+static_cast<int>(KeyCode::KEY_F1)); // Function keys
            }
        }
        case 224: {
            int kk;
            switch (kk = getch()) {
            case 71: return KeyCode::KEY_HOME;
            case 72: return KeyCode::KEY_UP;
            case 73: return KeyCode::KEY_PGUP;
            case 75: return KeyCode::KEY_LEFT;
            case 77: return KeyCode::KEY_RIGHT;
            case 79: return KeyCode::KEY_END;
            case 80: return KeyCode::KEY_DOWN;
            case 81: return KeyCode::KEY_PGDOWN;
            case 82: return KeyCode::KEY_INSERT;
            case 83: return KeyCode::KEY_DELETE;
            default:
                return static_cast<KeyCode>(kk-123+static_cast<int>(KeyCode::KEY_F1)); // Function keys
            }
        }
        case 9: return KeyCode::KEY_TAB;
        case 13: return KeyCode::KEY_ENTER;
#ifdef _WIN32
	    case 27: return KeyCode::KEY_ESCAPE;
#else // _WIN32

        case 48: return KeyCode::KEY_0;
        case 49: return KeyCode::KEY_1;
        case 50: return KeyCode::KEY_2;
        case 51: return KeyCode::KEY_3;
        case 52: return KeyCode::KEY_4;
        case 53: return KeyCode::KEY_5;
        case 54: return KeyCode::KEY_6;
        case 55: return KeyCode::KEY_7;
        case 56: return KeyCode::KEY_8;
        case 57: return KeyCode::KEY_9;

        case 155: // single-character CSI
        case 27: {
            // Process ANSI escape sequences 
            if (cnt >= 3 && getch() == '[') {
                switch (k = getch()) {
                case 'A': return KeyCode::KEY_UP;
                case 'B': return KeyCode::KEY_DOWN;
                case 'C': return KeyCode::KEY_RIGHT;
                case 'D': return KeyCode::KEY_LEFT;
                default: return KeyCode::KEY_ESCAPE;
                }
            } else return KeyCode::KEY_ESCAPE;
        }
#endif // _WIN32
    default:
        return static_cast<KeyCode>(k);
    }
}

} // namespace TerminalCtrl