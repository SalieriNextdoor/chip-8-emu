#include "emulator.h"
#include "constants.h"
#include "executor.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <stdexcept>
#include <thread>
using namespace emulation;

ushort lastKey = 0;

Emulator::Emulator(const char *filepath, ushort cpf, ushort fps)
    : executor{memory}, cpf{cpf}, fps{fps} {
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

  setFonts(FONT_HEX, sizeof(FONT_HEX));

  std::thread timerThread{[](Executor *executor) { executor->countdown(); },
                          &executor};
  timerThread.detach();
  std::thread interruptThread{[](Executor *executor) { executor->interrupt(); },
                          &executor};
  interruptThread.detach();
}

void Emulator::setFonts(const byte *font, uint length, ushort startAddr) {
  for (uint i = startAddr; i < length; i++)
    memory[i] = font[i];
}

ushort Emulator::fetch() {
  ushort pc = executor.getpc();
  if (executor.valid_instruction())
    return (memory[pc] << 8) | memory[pc + 1];
  else
    throw std::out_of_range("Invalid instruction: pc was " +
                            std::to_string(pc));
}

void Emulator::processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS)
    lastKey |= 1;
  else
    lastKey &= ~1;

  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
    lastKey |= 1 << 1;
  else
    lastKey &= ~(1 << 1);

  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
    lastKey |= 1 << 2;
  else
    lastKey &= ~(1 << 2);

  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
    lastKey |= 1 << 3;
  else
    lastKey &= ~(1 << 3);

  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
    lastKey |= 1 << 4;
  else
    lastKey &= ~(1 << 4);

  if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
    lastKey |= 1 << 5;
  else
    lastKey &= ~(1 << 5);

  if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
    lastKey |= 1 << 6;
  else
    lastKey &= ~(1 << 6);

  if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
    lastKey |= 1 << 7;
  else
    lastKey &= ~(1 << 7);

  if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
    lastKey |= 1 << 8;
  else
    lastKey &= ~(1 << 8);

  if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
    lastKey |= 1 << 9;
  else
    lastKey &= ~(1 << 9);

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    lastKey |= 1 << 10;
  else
    lastKey &= ~(1 << 10);

  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    lastKey |= 1 << 11;
  else
    lastKey &= ~(1 << 11);

  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    lastKey |= 1 << 12;
  else
    lastKey &= ~(1 << 12);

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    lastKey |= 1 << 13;
  else
    lastKey &= ~(1 << 13);

  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    lastKey |= 1 << 14;
  else
    lastKey &= ~(1 << 14);

  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    lastKey |= 1 << 15;
  else
    lastKey &= ~(1 << 15);
}

void Emulator::run_cycle() {
  processInput(emuScreen.window);

  ushort next = fetch();
  Instruction inst = parser.read(next);
  executor.execute(next, inst, lastKey);
}

void Emulator::run() {
  using namespace std::chrono;

  auto last = high_resolution_clock::now();
  for (;;) {
    auto current = high_resolution_clock::now();
    auto elapsed = duration_cast<microseconds>(current - last);

    if (elapsed.count() >= (int)(1.0f / fps * 10e5)) {
      for (ushort i = 0; i < cpf; i++)
        run_cycle();

      if (emuScreen.render(executor.getpixels()))
        return;
      last = current;
    }
    else // Small sleep to prevent high CPU usage
      std::this_thread::sleep_for(microseconds(100));
  }
}
