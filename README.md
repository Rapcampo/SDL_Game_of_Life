# Game of Life

John Conway's Game of Life implemented in C using SDL as a graphics library.

## Contents

- [Description](#description)
- [Build](#build)
  - [Requirements](#requirements)
  - [Installation](#installation)
  - [Troubleshooting](#troubleshotting)
- [Usage](#usage)
- [References](#references)

## Description

This small project was implemented with the goal of learning to use the SDL library as a reference for future goal projects, as well as doing my first project with cellula automata.
It was implemented using only C and SDL, no other external libraries. A few extra features of this project are:

- Cells have simple shaders choices for different looks
- Random Cell density (choosen randomly between 3) and random cell placement each run
- Pause functionality with `space bar` which maintains the hue changes
- Hardware Acceleration support (Vulkan prefered, openGL as fallback)
- Wayland (prefered) and X11 support
- Multiple resolutions support 
- Window resizing and window fractional scalling support

## Build

To be able to execute this program correctly you will need the following

### Requirements

- A Linux Operating System
- GCC
- SDL3
- Makefile

### Installation

1. Clone the project reposoritory

```bash
git clone git@github.com:Rapcampo/SDL_Game_of_Life.git game_of_life
cd game_of_life
```

2. on the root of the repository

```bash
make
```

This is it, `make run` can be used for a quick run of the program in a set resolution. `make re` for recompiling and `make fclean` for cleaning up objects and program.

### Troubleshooting

If there is an error `SDL\SDL3_main.h` include not found, make sure SDL is correctly seen by pkg-config and returns something with `pkg-config --libs sdl3 && pkg-config --cflags sdl3` as it may not be able to find the location correclty.

## Usage

A run of the program can have the following format `./life [width] [height] <shader>` where width and height are the requested resolutions, and shader as an optional hue of choice.
The resolution can be anything between 640x480 all the way up to 3840x2160, as long as the resolution is cleanly divisible by 10.

It is worth noting that resolution passed as an argument is a **request** and therefore the window generated will depend on your actual usable monitor resolution as a safety measure. The board will still be generated with the requested resolution and downscalled to the window size (or upscalled in the case of increasing window size after running the program).

Shaders have a few options, which can be seen by running `./life` without any arguments, which will give you the format and shader options list.

A regular run of the program will give you a few stats of the current run of it. Like so:

```bash
Life git:main ❯ life 1080 1920 2                                                                                      ✹
	Game stats
Name: Game of Life
Window size: 1080x1440
Video driver: wayland
Renderer: gpu
Requested resolution: 1080x1920
Shader Option: Purple
Board size: 270x480
Current Generation: 25
```

## References

- [Game of life on Wikipedia](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
- [SDL3 guide by Mike Shah](https://www.youtube.com/watch?v=IEduyVGt67g&list=PLvv0ScY6vfd-RZSmGbLkZvkgec6lJ0BfX&index=3&pp=iAQB)
- [Beginner Game Programming V3.0](https://lazyfoo.net/tutorials/SDL3/index.php)
- [SDL3 Wiki Documentation](https://wiki.libsdl.org/SDL3/CategoryMain)
