#include "screen.h"
#include "constants.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
using namespace screen;

void Screen::framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height) {
  glViewport(0, 0, width, height);
}

Screen::Screen() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(W_WIDTH * SCR_SIZE_MULT, W_HEIGHT * SCR_SIZE_MULT,
                            windowName, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  glViewport(0, 0, W_WIDTH * SCR_SIZE_MULT, W_HEIGHT * SCR_SIZE_MULT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  textureData = new GLubyte[DATA_SIZE];
  memset(textureData, 0, DATA_SIZE);

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, W_WIDTH, W_HEIGHT, 0, PIXEL_FMT,
               GL_UNSIGNED_BYTE, (GLvoid *)textureData);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenVertexArrays(1, &VAO);

  shaderProgram = std::make_unique<Shader>(vertexShader, fragmentShader);
  shaderProgram->setInt("texture1", 0);

  glGenBuffers(1, &EBO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glGenBuffers(1, &PBO);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

int Screen::updatePixels(GLubyte *dst, char *pixels,
                         uint drawColor = DRAW_COLOR, uint bgColor = BG_COLOR,
                         int width = W_WIDTH, int height = W_HEIGHT) {
  if (!dst || !pixels)
    return -1;

  int *ptr = (int *)dst;
  for (int i = 0; i < height; ++i)
    for (int j = 0; j < width; ++j)
      *ptr++ = *pixels++ == 1 ? drawColor : bgColor;

  return 0;
}

int Screen::render(char *pixels) {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, textureId);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W_WIDTH, W_HEIGHT, PIXEL_FMT,
                  GL_UNSIGNED_BYTE, 0);

  glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
  GLubyte *ptr = (GLubyte *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  if (ptr) {
    updatePixels(ptr, pixels);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
  }
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  shaderProgram->use();
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(window);
  glfwPollEvents();

  return glfwWindowShouldClose(window);
}

Screen::~Screen() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &PBO);
  glDeleteTextures(1, &textureId);
  delete[] textureData;
  shaderProgram.~unique_ptr();

  glfwTerminate();
}
