#pragma once
#include <GLFW/glfw3.h>
#include <cy/cyTriMesh.h>

class WindowController
{
  public:
    uint16_t w,h;

    WindowController(uint16_t w, uint16_t h, const char* title, uint16_t majorV, uint16_t minorV);
    ~WindowController();

    bool shouldClose();
    void pollEvents();
    void swapBuffers();
    
    GLFWwindow* window;
  private:
    
};

WindowController initContext(uint16_t w, uint16_t h, const char* title, cy::Vec4f clearColor);
void frameBufferSizeCallback(GLFWwindow* window, uint16_t w, uint16_t h);
void defineGlPreferences();
void errorCallback(int error, const char* description);
void frameBufferSizeCallback(GLFWwindow* window, int w, int h);