#define GLFW_INCLUDE_NONE
#include <iostream>
#include <window.h>
#include <material.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <examples/exampleScene.h>

glm::vec4 CLEAR_COLOR(0.529f, 0.808f, 0.922f, 1.0f);
WindowController WINDOW = initContext(1280, 720, "Simple game lib", CLEAR_COLOR);

int main(void)
{
  double lastTime = glfwGetTime();
  double currentTime = glfwGetTime();
  double deltaTime = 0.0f;

  double fps = 0.0f;
  uint64_t frameCount = 0;
  double lastFpsTime = glfwGetTime();
  double fpsInterval = 5.0f;

  ExampleScene* scene = new ExampleScene();
  
  while (!WINDOW.shouldClose())
  {
    scene->handleStart();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->beforeUpdate();
    scene->draw();
    scene->afterUpdate();

    WINDOW.swapBuffers();
    WINDOW.pollEvents();

    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    WINDOW.deltaTime = deltaTime;

    frameCount++;

    //log current FPS
    if (currentTime - lastFpsTime >= fpsInterval) {
      fps = frameCount / (currentTime - lastFpsTime);
      std::cout << "Current FPS: " << fps << std::endl;
      frameCount = 0;
      lastFpsTime = currentTime;
    }
  }
  
  delete scene;
}