#ifndef CHIP8_CONSTANTS_H
#define CHIP8_CONSTANTS_H
#include "Include/glad/glad.h"

// Screen Constants
namespace screen {
const int W_WIDTH = 64;
const int W_HEIGHT = 32;
const int SCR_SIZE_MULT = 15; // upscale screen for visibility purposes
const int CHANNEL_COUNT = 4;
const int DATA_SIZE = W_WIDTH * W_HEIGHT * CHANNEL_COUNT;
const GLenum PIXEL_FMT = GL_BGRA;
const unsigned int DRAW_COLOR = 0xFFCC01;
const unsigned int BG_COLOR = 0x996700;
const float vertices[] = {
    // First triangle (top-right, bottom-right, bottom-left)
    1, 1, 0, 1, 1,   // Position (1, 1, 0), Texture (1, 1)
    1, -1, 0, 1, 0,  // Position (1, -1, 0), Texture (1, 0)
    -1, -1, 0, 0, 0, // Position (-1, -1, 0), Texture (0, 0)

    // Second triangle (bottom-left, top-left, top-right)
    -1, 1, 0, 0, 1, // Position (-1, 1, 0), Texture (0, 1)
};
const GLuint indices[] = {0, 1, 3, 1, 2, 3};
const char *const vertexShader = "./shaders/screen/shader.vs";
const char *const fragmentShader = "./shaders/screen/shader.fs";
const char *const windowName = "Chip-8 Emulator";
} // namespace screen

// Emulation Constants
namespace emulation {
using ushort = unsigned short;
using uint = unsigned int;
const ushort START_INSTRUCTIONS = 0x200;
const ushort END_INSTRUCTIONS = 0xFFF;
const uint MEM_SIZE = 4096;
const uint STACK_SIZE = 16;
const uint NREGISTERS = 16;

const uint SPRITE_WIDTH = 8;
}; // namespace emulation

#endif
