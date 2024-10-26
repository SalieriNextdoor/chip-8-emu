#include "emulator.h"
#include <stdexcept>
using namespace emulation;

Emulator::Emulator(const char *filepath) : executor{memory} {
  FILE *file = fopen(filepath, "rb");
  if (file == NULL)
    throw std::invalid_argument(std::string{"Couldn't open ROM file at "} +
                                filepath);
  fseek(file, 0, SEEK_END);
  sz = ftell(file);
  rewind(file);
  if (sz > END_INSTRUCTIONS - START_INSTRUCTIONS + 1) {
    fclose(file);
    throw std::length_error("File is too large");
  }
  fread(&memory[START_INSTRUCTIONS], sizeof(char), sz, file);
  fclose(file);
}

ushort Emulator::fetch() {
  ushort pc = executor.getpc();
  if (executor.valid_instruction())
    return (memory[pc] << 8) | memory[pc + 1];
  else
    throw std::out_of_range("Invalid instruction: pc was " + std::to_string(pc));
}

void Emulator::processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int Emulator::run() {
  ushort next = fetch();
  Instruction inst = parser.read(next);
  executor.execute(next, inst);

  processInput(emuScreen.window);

  return emuScreen.render(executor.getpixels());
}
