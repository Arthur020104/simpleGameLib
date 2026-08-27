#include <fpsCamera.h>
#include <window.h>
#include <glm/gtc/matrix_transform.hpp>

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

void FPSCamera::setRotation(glm::vec3 rot)
{
  Component::setRotation(rot);

  float radX = glm::radians(rot.x);
  float radY = glm::radians(rot.y);

  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radY, glm::vec3(0.0f, 1.0f, 0.0f));
  rotationMatrix = glm::rotate(rotationMatrix, radX, glm::vec3(1.0f, 0.0f, 0.0f));

  this->direction = glm::normalize(rotationMatrix * glm::vec4(this->defaultDirection, 0.0f));
  this->up = glm::normalize(rotationMatrix * glm::vec4(this->defaultUp, 0.0f));
  this->right = glm::normalize(rotationMatrix * glm::vec4(this->defaultRight, 0.0f));

  this->updateMatrices();
}