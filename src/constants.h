#ifndef CHIP8_CONSTANTS_H
#define CHIP8_CONSTANTS_H
#include "Include/glad/glad.h"

// Emulation Constants
namespace emulation {
using ushort = unsigned short;
using uint = unsigned int;
using byte = unsigned char;

const ushort CYCLES_PER_FRAME = 11;
const ushort FRAMES_PER_SECOND = 60;

const ushort START_INSTRUCTIONS = 0x200;
const ushort END_INSTRUCTIONS = 0xFFF;
const uint MEM_SIZE = 4096;
const uint STACK_SIZE = 16;
const uint NREGISTERS = 16;

const uint SPRITE_WIDTH = 8;

const byte FONT_HEX[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};
const ushort FONT_START_ADDR = 0x00;
const byte FONT_HEIGHT = 0x05;
}; // namespace emulation

// Screen Constants
namespace screen {
const int W_WIDTH = 64;
const int W_HEIGHT = 32;
const int SCR_SIZE_MULT = 15; // upscale screen for visibility purposes
const int CHANNEL_COUNT = 4;
const int DATA_SIZE = W_WIDTH * W_HEIGHT * CHANNEL_COUNT;
const GLenum PIXEL_FMT = GL_BGRA;
const unsigned int PIXEL_MAP_SIZE = W_WIDTH * W_HEIGHT / emulation::SPRITE_WIDTH;
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

// Audio Constants
namespace audio {
const char *const AUDIO_DEVICE = "default";
const unsigned int SAMPLE_RATE = 44100;
const int FREQUENCY = 440;
const int AMPLITUDE = 30000;
}; // namespace audio

#endif
