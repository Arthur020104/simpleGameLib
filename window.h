#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class WindowController
{
  public:
    uint16_t w,h;

    WindowController(uint16_t w, uint16_t h, const char* title, uint16_t majorV, uint16_t minorV);
    ~WindowController();

    bool shouldClose();
    void pollEvents();
    void swapBuffers();
    double getTime();

    glm::vec2 getMousePos(bool ndc = false);

    float aspect = 0.0f;

    double deltaTime = 0.0f;
    bool updateCameras = true;
    GLFWwindow* window;
  private:

    
};

extern WindowController WINDOW;

WindowController initContext(uint16_t w, uint16_t h, const char* title, glm::vec4 clearColor);
void frameBufferSizeCallback(GLFWwindow* window, uint16_t w, uint16_t h);
void defineGlPreferences();
void errorCallback(int error, const char* description);
void frameBufferSizeCallback(GLFWwindow* window, int w, int h);