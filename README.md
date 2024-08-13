# Terminal Ctrl

Terminal Ctrl is a opinionated header-only terminal control library for C++.

Terminal Ctrl was created to be a simple, lightweight and easy to use bases on my needs, it's not a full terminal control library, it's just a simple one.

It's a lot inspired by other libraries like:
- [tercontrol](https://github.com/ZackeryRSmith/tercontrol)
- [rogueutil](https://github.com/sakhmatd/rogueutil)
- [cpp-terminal](https://github.com/jupyter-xeus/cpp-terminal)

But none of them were exactly what I wanted (C oriented, missing some features, not working with windows API, etc), so I decided to create my own.

I'm open to suggestions and pull requests, but I don't want to make it a full terminal control library, I want to keep it simple and easy to use.

Key control is not implemented yet, but it's one of the features that I want to implement in the future.

TerminalCtrl is compatible with Windows and UNIX/POSIX based systems.

## Features

- [x] No dependencies (other then C++'s standard library)
- [x] Set/get the cursor position
- [x] Hide/show the cursor
- [x] Foreground color (16 base colors)
- [x] Background color (16 base colors)
- [x] Text attributes (bold, dim, italic, underscore, crossed, reversed, blink, invisible)
- [x] Clear all terminal
- [x] Get the terminal size
- [ ] blocking and non-blocking key input
- [ ] print at given position without moving the cursor

It uses the Windows API on windows and ANSI escape codes to control the terminal on linux.

## Known issues

- Getting the cursor position work only on windows, Unable to make it work on linux yet.
## Installation

It's a cmake project, so you can use `target_link_libraries` to link it to your project.

It doesn't support installation on the operating system.

sample cmake code using fetch to add it to your project:

```cmake
FetchContent_Declare(
    terminal_ctrl
    GIT_REPOSITORY https://github.com/dsmtE/terminal_ctrl
    GIT_TAG "origin/main"
)

FetchContent_MakeAvailable(terminal_ctrl)
target_link_libraries(${PROJECT_NAME} PRIVATE terminal_ctrl)
```
