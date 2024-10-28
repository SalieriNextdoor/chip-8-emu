/**
 * @file util.h
 * @copyright
 * Copyright 2024 Rafael Spinassé
 * Licensed under MIT license
 *
 * @brief Declares utility functions.
 */

#ifndef CHIP8_UTIL_H
#define CHIP8_UTIL_H

using byte = unsigned char;
using ushort = unsigned short;

/**
 * @brief Relates to more general utility functions.
 */
namespace util {

/**
 * @brief Get a given 4-bit (nibble) mask of a value.
 * @param val the given value
 * @param n its position from 0 to 3
 * @return the corresponding nibble
 */
byte inline get_nth_nibble(ushort val, ushort n) {
  return (val >> n * 4) & 0xF;
}

/**
 * @brief Get a byte mask of a value.
 * @param val the given value
 * @param n its position (0 or 1)
 * @return the corresponding byte
 */
byte inline get_nth_byte(ushort val, ushort n) { return (val >> n * 4) & 0xFF; }
/*
 * @brief Get the last 12-bits of a value.
 * @param val the given value
 * @return the corresponding address
 */
ushort inline get_addr(ushort val) { return val & 0xFFF; }

/**
 * @brief Get a random value.
 * @param min the minimum value
 * @param max the maximum value
 * @return the computed random value
 */
byte random_byte(byte min = 0, byte max = 255);
}; // namespace util

#endif
