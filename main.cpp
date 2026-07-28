#define GLFW_INCLUDE_NONE
#include <window.h>
#include <string.h>
#include <iostream>
#include <utils.h>
#include <GL/glew.h>
#include <cy/cyGL.h>
#include <cy/cyCore.h>
#include <cy/cyTriMesh.h>
#include <cy/cyMatrix.h>
#include <mesh.h>
#include <gameObject.h>
#include <program.h>
#include <camera.h>

cy::Vec4f CLEAR_COLOR(0.529f, 0.808f, 0.922f, 1.0f);

int main(void)
{
  std::string windowTitle = "Teste 11";
  WindowController window = initContext(1280, 720, windowTitle.c_str(), CLEAR_COLOR);

  float wf = 1.0f / 1280.0f;
  float hf = 1.0f / 720.0f;

  Program shaderProgram("/home/arthur/Documents/simpleGame/shaders/vertex.vs", "/home/arthur/Documents/simpleGame/shaders/frag.fs");
  std::string teapot = "/home/arthur/Documents/simpleGame/obj/teapot.obj";
  
  Mesh *mesh = loadMesh(teapot);
  GameObject teapotObj(mesh, &shaderProgram);
 
  Camera cam(cy::Vec3f(0.0f, 5.0f, 30.0f));
  double i, j;
  Ray r;

  while (!window.shouldClose())
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    teapotObj.draw(cam);
    if(glfwGetKey(window.window, GLFW_KEY_I) == GLFW_PRESS)
    {
      glfwGetCursorPos(window.window, &i, &j);
      
      // Convert screen coordinates [0, W]x[0, H] directly to NDC [-1, 1]x[-1, 1]
      float x = (i * wf) * 2.0f - 1.0f;
      float y = 1.0f - (j * hf) * 2.0f; // Inverted Y axis

      std::cout << "NDC Position: " << x << ", " << y << std::endl;

      r = cam.generateRay(cy::Vec2f(x, y));
      if(teapotObj.intersect(r))
      {
        teapotObj.isSelected = true;
        std::cout << "Teapot selected!" << std::endl;
        std::cout << "Hit point: " << r.hits[0].point.x << ", " << r.hits[0].point.y << ", " << r.hits[0].point.z << std::endl;
      }
    }
    else if(glfwGetKey(window.window, GLFW_KEY_U) == GLFW_PRESS)
    {
      teapotObj.isSelected = false;
    }

    window.swapBuffers();
    window.pollEvents();
  }
}