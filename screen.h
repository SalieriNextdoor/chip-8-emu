#ifndef CHIP8_SCREEN_H
#define CHIP8_SCREEN_H

#include "Include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <cstring>
#include <memory>

#include "constants.h"
#include "shader.h"

namespace screen {
class Screen {
  GLubyte *textureData;
  GLuint VAO, VBO, EBO, PBO, textureId;
  std::unique_ptr<Shader> shaderProgram;

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
   * @params color the color to be painted, a value between 0 and INT_MAX
   * @params width the width of the screen and texture
   * @params height the height of the screen and texture
   */
  int updatePixels(GLubyte *dst, unsigned char *pixels, uint drawColor, uint bgColor,
                   int width, int height);

public:
  GLFWwindow *window;

  Screen();

  int render(unsigned char *pixels);

  ~Screen();
};
}; // namespace screen

#endif
