#define GLFW_INCLUDE_NONE
#include <window.h>
#include <string.h>
#include <iostream>
#include <GL/glew.h>
#include <cy/cyGL.h>
#include <cy/cyCore.h>
#include <cy/cyTriMesh.h>
#include <cy/cyMatrix.h>
#include <examples/exampleScene.h>

cy::Vec4f CLEAR_COLOR(0.529f, 0.808f, 0.922f, 1.0f);
WindowController WINDOW = initContext(1280, 720, "Teste 11", CLEAR_COLOR);
std::shared_ptr<Program> DEFAULT_SHADER = std::make_shared<Program>("/home/arthur/Documents/simpleGame/shaders/vertex.vs", "/home/arthur/Documents/simpleGame/shaders/frag.fs");
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
    scene->handleObjectStart();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->beforeDrawing();
    scene->draw();
    scene->aftherDrawing();

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