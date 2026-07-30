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

int main(void)
{
  std::string windowTitle = "Teste 11";
  WindowController window = initContext(1280, 720, windowTitle.c_str(), CLEAR_COLOR);

  double lastTime = glfwGetTime();
  double currentTime = glfwGetTime();
  double deltaTime = 0.0f;

  ExampleScene* scene = new ExampleScene();

  while (!window.shouldClose())
  {
    scene->handleObjectStart();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->beforeDrawing();
    scene->draw();
    scene->aftherDrawing();

    window.swapBuffers();
    window.pollEvents();

    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
  }
  
  delete scene;
}