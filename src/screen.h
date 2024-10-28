/**
 * @file screen.h
 * @copyright
 * Copyright 2024 Rafael Spinassé
 * Licensed under MIT license
 *
 * @brief Declares the screen functionality.
 */

#ifndef CHIP8_SCREEN_H
#define CHIP8_SCREEN_H

#include "Include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <cstring>
#include <memory>

#include "constants.h"
#include "shader.h"

/**
 * @brief Relates to classes and constants concerning the screen
 * behavior.
 */
namespace screen {
/**
 * @brief Defines the methods for screen rendering.
 */
class Screen {
  GLubyte *textureData;
  GLuint VAO, VBO, EBO, PBO, textureId;
  std::unique_ptr<Shader> shaderProgram;

  /**
   * @brief Regulate the viewport upon resizing the screen.
   */
  void static framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
  /**
   * This function is used with a texture that is overlayed on the screen and
   * has the same size of the screen. It then paints each pixel into either a
   * drawing color or a background color, according to the contents of the
   * pixels array.
   *
   * @param dst an array of bytes mapping the screen-texture. Its size must be
   * the width times the height times the number of channels of the
   * screen-texture.
   * @param pixels an array of bytes mapping the pixels of the screen to either
   * 0 or 1, each bit being a pixel. Its size must be the width times the height
   * of the screen divided by 8.
   * @param drawColor the color to be painted, in hex code 
   * @param bgColor the background color, in hex code
   * @param width the width of the screen and texture
   * @param height the height of the screen and texture
   * @see emulation::Executor::draw_sprites
   */
  int updatePixels(GLubyte *dst, unsigned char *pixels, uint drawColor, uint bgColor,
                   int width, int height);

public:
  GLFWwindow *window;

  /**
   * @brief Initialize the screen rendering.
   *
   *
   * This method will initialize GLFW and set up buffers, shaders
   * and the texture.
   *
   * It will throw runtime_error if GLFW or GLAD cannot be loaded
   */
  Screen();

  /**
   * @brief Render the screen.
   *
   * @param pixels an array representing pixels, each entry being a row
   * of 8 pixels, one per bit
   * @return whether or not the program should close
   *
   * 
   * This method will do a render cycle, reading from the pixels array 
   * and updating it accordingly.
   * @see screen::Screen::updatePixels
   */
  int render(unsigned char *pixels);

  /**
   * @brief Delete all buffers created for screen.
   */
  ~Screen();
};
}; // namespace screen

#endif
