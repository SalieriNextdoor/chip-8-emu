/**
 * @file executor.h
 * @copyright
 * Copyright 2024 Rafael Spinassé
 * Licensed under MIT license
 *
 * @brief Declares the execution functionality.
 */

#ifndef CHIP8_EXECUTOR_H
#define CHIP8_EXECUTOR_H

#include "audiohandler.h"
#include "constants.h"
#include "parser.h"
#include <atomic>

namespace emulation {
using ushort = unsigned short;
using byte = unsigned char;

/**
 * @brief Defines the methods and attributes.
 * related to instruction execution.
 */
class Executor {
  byte *memory;
  ushort stack[STACK_SIZE] = {0};
  byte sp = 0;
  byte nregister[NREGISTERS] = {0};
  ushort iregister = 0;
  ushort pc = START_INSTRUCTIONS;
  
  std::atomic<byte> delayTimer{0}, soundTimer{0};
  audio::AudioHandler audio{&soundTimer};

  byte pixels[screen::PIXEL_MAP_SIZE] = {0};

  std::atomic_bool canDraw = true;
  /**
   * @brief Update the pixels array with the sprite data.
   *
   * @param operation the operation code 
   * @param vx the register for x position 
   * @param vy the register for y position
   * @return whether a set bit was unset
   *
   * The sprite is read sequentially from memory according
   * to the contents of the iregister, each byte representing
   * a row of 8 pixels through its bits, moving down from a 
   * start position. It then is XOR'd against the respective
   * position on the pixels array. The read bytes are properly
   * separated and shifted to XOR against two separate rows, 
   * for the case where the starting x position is not divisible
   * by 8.
   *
   * If the sprite starting coordiantes are outside the screen 
   * coordinates, it wraps back through the other edge. If the sprite 
   * starts within the screen but goes outside which through either of
   * the edges, the outside part is clipped.
   */
  bool draw_sprite(ushort operation, byte vx, byte vy);

public:
  /**
   * @brief Constructor for the executor.
   * @param memory the memory array from the emulator
   * @see emulation::Emulator
   */
  Executor(byte *memory);
  /**
   * @brief Executes an operation according to the given instruction.
   * @param operation the operation code
   * @param instruction the operation label
   * @param lastKey the last key pressed
   * @see emulation::Emulator::processInput
   */
  void execute(ushort operation, Instruction instruction, ushort lastKey);

  /**
   * @brief Check whether the current instruction on pc is valid.
   * @return whether the current instruction is valid
   */
  bool inline valid_instruction() const {
    return pc >= START_INSTRUCTIONS && pc < END_INSTRUCTIONS;
  }
  /**
   * @brief Increment the program counter to the next 16-bit instruction.
   */
  void inline increment_pc() { pc += 2; }
  /**
   * @brief get the value of the current program counter
   * @return value of the current program counter
   */
  ushort getpc() const { return pc; };

  /**
   * @brief Decrement the timers over time.
   *
   *
   * This method will start an infinite loop in which it will decrement
   * the delay timer and sound timer 60 times per second, if the respective
   * timer is bigger than 0.
   *
   * This method must run within a thread, and will sleep for 100 microseconds
   * after each loop to minimize CPU usage.
   */
  void countdown();
  /**
   * @brief Enable sprite drawing every 1/60th of a second.
   *
   * This method will start an infinite loop in which it enable sprite
   * drawing 60 times per second, simulating an interrupt that enables
   * drawing in the original machine.
   *
   * This method must run within a thread, and will sleep for 100 microseconds
   * after each loop to minimize CPU usage.
   */
  void interrupt();

  /**
   * @brief Get the pixels array.
   * @return the pixels array
   */
  byte *getpixels() { return pixels; }
};
}; // namespace emulation

#endif
