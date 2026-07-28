#define GLFW_INCLUDE_NONE
#include <window.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cy/cyGL.h>
#include <cy/cyCore.h>
#include <cy/cyTriMesh.h>
#include <cy/cyMatrix.h>
#include <string>
#include <stdexcept>

WindowController initContext(uint16_t w, uint16_t h, const char* title, cy::Vec4f clearColor)
{
  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) 
    throw std::runtime_error("Failed to initialize GLFW");

  WindowController window(w, h, title, 4, 6);
  
  GLenum err = glewInit();
  if (GLEW_OK != err) 
    throw std::runtime_error(std::string("GLEW initialization failed: ") + (const char *)glewGetErrorString(err));

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  
  glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
  defineGlPreferences();

  CY_GL_REGISTER_DEBUG_CALLBACK;

  return window;
}

WindowController::WindowController(uint16_t w, uint16_t h, const char* title, uint16_t majorV, uint16_t minorV)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorV);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorV);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);  

  
  this->window = glfwCreateWindow(w, h, title, NULL, NULL);
  if (!this->window)
    throw std::runtime_error("Failed to create GLFW window");
  
  glfwMakeContextCurrent(this->window);
  glfwSetWindowUserPointer(this->window, this);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  glViewport(0, 0, w, h);
}

WindowController::~WindowController()
{
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

bool WindowController::shouldClose()
{
  return glfwWindowShouldClose(this->window);
}

void WindowController::swapBuffers()
{
  glfwSwapBuffers(this->window);
}

void WindowController::pollEvents()
{
  glfwPollEvents();
}

void errorCallback(int error, const char* description)
{
  std::cerr <<"Error: " << description << std::endl;
}

void defineGlPreferences()
{
  glfwSwapInterval(0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);                   // Specify that back-facing triangles should be culled (not rendered)
  glFrontFace(GL_CCW);                    // Define counter-clockwise vertex winding as front-facing triangles
  glEnable(GL_BLEND);
}
void frameBufferSizeCallback(GLFWwindow* window, int w, int h)
{
  WindowController* myWindow = (WindowController*)glfwGetWindowUserPointer(window);
  myWindow->w = w; 
  myWindow->h = h;
  glViewport(0, 0, myWindow->w, myWindow->h);
  //should create an variable called ratio  and oldRatio when ratio !== oldRatio, put the bool ratioChanged as true.
  //create an funtion to put ratioChanged = false and oldRatio = ratio(afther all cameras ratio are updated)
  //projMatrix = cy::Matrix4f::Perspective(FOV * cy::Deg2Rad<float>(), float(W)/float(H), N, F);
}