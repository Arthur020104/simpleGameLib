#include <fpsCamera.h>
#include <window.h>

void FPSCamera::start()
{ 
  return;
}

void FPSCamera::afterUpdate()
{
  return;
}

void FPSCamera::beforeUpdate()
{
  glm::vec3 moveVector = glm::vec3(0.0f, 0.0f, 0.0f);

  if(glfwGetKey(WINDOW.window, GLFW_KEY_E) == GLFW_PRESS) moveVector += this->getUpVector();
  if(glfwGetKey(WINDOW.window, GLFW_KEY_Q) == GLFW_PRESS) moveVector -= this->getUpVector();
  if(glfwGetKey(WINDOW.window, GLFW_KEY_D) == GLFW_PRESS) moveVector += this->getRightVector();
  if(glfwGetKey(WINDOW.window, GLFW_KEY_A) == GLFW_PRESS) moveVector -= this->getRightVector();
  if(glfwGetKey(WINDOW.window, GLFW_KEY_W) == GLFW_PRESS) moveVector += this->getForwardVector();
  if(glfwGetKey(WINDOW.window, GLFW_KEY_S) == GLFW_PRESS) moveVector -= this->getForwardVector();

  moveVector *= this->velocity * WINDOW.deltaTime;
  this->setPosition(moveVector + this->getPosition());

  glm::vec2 mousePos = WINDOW.getMousePos();

  if (this->firstMouse)
  {
    this->lastMousePos = mousePos;
    this->firstMouse = false;
  }

  glm::vec2 mouseDelta = (mousePos - this->lastMousePos) * this->sensitivity;
  this->lastMousePos = mousePos;

  glm::vec3 currentRot = this->getRotation();

  float pitch = std::clamp(currentRot.x - mouseDelta.y, -89.0f, 89.0f);

  float yaw = currentRot.y - mouseDelta.x;

  this->setRotation(glm::vec3(pitch, yaw, 0.0f));
}