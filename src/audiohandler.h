/**
 * @file audiohandler.h
 * @copyright
 * Copyright 2024 Rafael Spinassé
 * Licensed under MIT license
 *
 * @brief Declares the audio functionality.
 */

#ifndef CHIP8_AUDIO_HANDLER_H
#define CHIP8_AUDIO_HANDLER_H
#include <atomic>

/**
 * @brief Relates to classes and constants concerning the audio
 * behavior.
 */
namespace audio {
using byte = unsigned char;

/**
 * @brief Defines the methods for audio handling.
 */
class AudioHandler {
  std::atomic<byte> *timer; // executor's sound timer

public:
  std::atomic_bool playing = false;

  /**
   * @brief Construct the audio handler.
   * @param stimer executer's sound timer
   *
   * @see emulation::Executor::countdown
   */
  AudioHandler(std::atomic<byte> *stimer);
  /**
   * @brief Play a monotone soundwave.
   *
   *
   * This function will use the ALSA library to
   * generate a wave according to the audio constants,
   * reproducing a monotone sound.
   *
   * It sets the variable playing to false after finishing.
   *
   * It throws a runtime_error exception if the PCM device
   * could not be opened or written to.
   */
  void playTone();
};

}; // namespace audio

#endif
