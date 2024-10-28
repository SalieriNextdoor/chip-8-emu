/**
 * @file util.cpp
 * @copyright
 * Copyright 2024 Rafael Spinassé
 * Licensed under MIT license
 *
 * @brief Implements utility functions.
 */

#include "util.h"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

namespace util {
byte random_byte(byte min, byte max) {
  std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
  return (byte)dist(rng);
}
}; // namespace util
