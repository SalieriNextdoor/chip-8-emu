#ifndef CHIP8_AUDIO_HANDLER_H
#define CHIP8_AUDIO_HANDLER_H
#include <atomic>

namespace audio {
using byte = unsigned char;

class AudioHandler {
  std::atomic<byte> *timer;

public:
  std::atomic_bool playing = false;

  AudioHandler(std::atomic<byte> *stimer);
  void playTone();
};

}; // namespace audio

#endif
