/**
 * @file emulator.h
 * @copyright
 * Copyright 2024 Rafael Spinassé
 * Licensed under MIT license
 *
 * @brief Declares the main emulator functionality.
 */

#ifndef CHIP8_EMULATOR_H
#define CHIP8_EMULATOR_H

#include "constants.h"
#include "executor.h"
#include "parser.h"
#include "screen.h"

/**
 * @brief Relates to classes and constants concerning the emulation
 * behavior.
 */
namespace emulation {

using byte = unsigned char;
using ushort = unsigned short;

/**
 * @brief Defines the methods for the emulation.
 */
class Emulator {
  byte memory[MEM_SIZE] = {0};

  screen::Screen emuScreen;
  Parser parser;
  Executor executor;

  long sz;
  ushort cpf, fps;

  /**
   * @brief Set the fonts for characters 0-F in memory.
   *
   * @param fonts the array of bytes for the characters, 5 bytes for each
   * @param length the length of said array
   * @param startAddr the memory address to begin writing the fonts
   *
   *
   * The characters are of height 5. The width is also 8, encoded
   * within the bit of each byte.
   */
  void setFonts(const byte *fonts, uint length,
                ushort startAddr = FONT_START_ADDR);

  /*
   * @brief Fetch the next operation code.
   * @return the operation code
   *
   *
   * The operation code is fetched and returned according to the
   * current position in memory according to pc.
   *
   * A invalid_argument is thrown if the given file cannot be opened.
   *
   * A length_error exception is thrown if it tries to fetch from an
   * invalid address.
   */
  ushort fetch();

  /**
   * @brief Run a single input, fetch, read and execute cycle.
   */
  void run_cycle();

  /**
   * @brief Process keyboard input.
   *
   *
   * The keys 0-F from the keyboard (or numpad) are mapped to
   * a lastKey variable, each bit of it corresponding to
   * the respective pressed key.
   *
   * It also instructs for the closing of the window
   * if ESC is pressed.
   */
  void processInput(GLFWwindow *window);

public:
  /**
   * @brief Construct the emulator.
   * @param filepath the path to the Chip-8 ROM file
   * @param cpf amount of cycles per frame
   * @param fps amount of frames per second
   *
   *
   * It reads from the given file and loads it in memory.
   * An exception is thrown if the file is too large for memory.
   *
   * It also starts two threads, one for the both the delay and
   * sound timers, and another one for the draw interrupt.
   * @see emulation::Executor::countdown
   * @see emulation::Executor::interrupt 
   */
  Emulator(const char *filepath, ushort cpf, ushort fps);

  /*
   * @brief Run the main emulation execution and rendering loop,
   * in fps frames per second and cps cycles per frame.
   */
  void run();
};
} // namespace emulation

#endif
