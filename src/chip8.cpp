#define GLFW_INCLUDE_NONE
#include "constants.h"
#include "emulator.h"
#include <iostream>

int main(int argc, char *argv[]) {
  ushort cpf = emulation::CYCLES_PER_FRAME;
  ushort fps = emulation::FRAMES_PER_SECOND;

  if (argc == 1) {
    std::cerr << "Usage: chip8 FILE [CPF] [FPS]" << std::endl;
    return 1;
  }
  if (argc > 2)
    cpf = atoi(argv[2]);
  if (argc > 3)
    fps = atoi(argv[3]);

  emulation::Emulator chip8{argv[1], cpf, fps};
  chip8.run();
  
  return 0;
}
