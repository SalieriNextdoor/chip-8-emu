# Chip-8 Emulator

A Chip-8 Emulator, made in C++ and using OpenGL and ALSA.

## Build
First, the directory `src/Include` must be created and library header folders for [glad (for gl 3.3)](https://glad.dav1d.de/), [glm](https://github.com/g-truc/glm) and [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) added. The file `glad.c` must be added to the Libs directory.

Then, access the src directory within a Linux terminal and run `make` to build.

## Build docs
To build the documentation, it's required to have doxygen installed.

Then, access the src directory and run `make docs`.

