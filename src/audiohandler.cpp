#include "audiohandler.h"
#include "constants.h"
#include <alsa/asoundlib.h>
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace audio;

AudioHandler::AudioHandler(std::atomic<byte> *stimer) : timer{stimer} {}

void AudioHandler::playTone() {
  snd_pcm_t *pcmHandle;
  snd_pcm_hw_params_t *params;
  int pcm;

  if ((pcm = snd_pcm_open(&pcmHandle, AUDIO_DEVICE, SND_PCM_STREAM_PLAYBACK,
                          0)) < 0)
    throw std::runtime_error("ERROR: Can't open \"" +
                             std::string{AUDIO_DEVICE} + "\" PCM device. " +
                             std::string{snd_strerror(pcm)});

  snd_pcm_hw_params_alloca(&params);
  snd_pcm_hw_params_any(pcmHandle, params);
  snd_pcm_hw_params_set_access(pcmHandle, params,
                               SND_PCM_ACCESS_RW_INTERLEAVED);
  snd_pcm_hw_params_set_format(pcmHandle, params, SND_PCM_FORMAT_S16_LE);
  snd_pcm_hw_params_set_channels(pcmHandle, params, 1);
  snd_pcm_hw_params_set_rate(pcmHandle, params, SAMPLE_RATE, 0);

  if ((pcm = snd_pcm_hw_params(pcmHandle, params)) < 0) {
    snd_pcm_close(pcmHandle);
    throw std::runtime_error("ERROR: Can't set hardware parameters. " +
                             std::string{snd_strerror(pcm)});
  }

  int frameCount = SAMPLE_RATE / 10;
  int16_t *buffer = new int16_t[frameCount];

  unsigned int sampleIndex = 0;

  while (*timer > 0) {
    for (int i = 0; i < frameCount; ++i) {
      buffer[i] = AMPLITUDE *
                  std::sin(2 * M_PI * FREQUENCY * sampleIndex++ / SAMPLE_RATE);
      if (sampleIndex >= SAMPLE_RATE)
        sampleIndex = 0; // Loop back to prevent overflow
    }

    // Write buffer to PCM device continuously
    if ((pcm = snd_pcm_writei(pcmHandle, buffer, frameCount)) == -EPIPE) {
      // Handle buffer underrun
      snd_pcm_prepare(pcmHandle);
    } else if (pcm < 0) {
      snd_pcm_drop(pcmHandle);
      delete[] buffer;
      snd_pcm_close(pcmHandle);
      throw std::runtime_error("ERROR: Can't write to PCM device. " +
                               std::string{snd_strerror(pcm)});
    }
  }

  snd_pcm_drop(pcmHandle);
  delete[] buffer;
  snd_pcm_close(pcmHandle);
  playing = false;
}
