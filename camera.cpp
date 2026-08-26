#include <camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <window.h>

Camera::Camera(glm::vec3 initialPos): Component(initialPos)
{
  this->aspect = WINDOW.aspect;
  this->projectionMatrix = glm::perspective(glm::radians(FOV), WINDOW.aspect, N, F);
  this->direction = glm::normalize(direction);
  
  updateMatrices();
}

glm::mat4& Camera::lookAtMatrix()
{
  glm::vec3 forward = this->getForwardVector();
  
  glm::vec3 right = this->getRightVector();

  glm::vec3 up = this->getUpVector();

  glm::vec3 pos = this->getPosition();

  viewMatrix[0] = glm::vec4(right.x, up.x, -forward.x, 0.0f);
  viewMatrix[1] = glm::vec4(right.y, up.y, -forward.y, 0.0f);
  viewMatrix[2] = glm::vec4(right.z, up.z, -forward.z, 0.0f);
  viewMatrix[3] = glm::vec4(-glm::dot(pos, right), -glm::dot(pos, up), glm::dot(pos, forward), 1.0f);

  return viewMatrix;
}

void Camera::updateMatrices()
{
  if(WINDOW.aspect != this->aspect) this->updateAspect(WINDOW.aspect);
  this->viewProjection = this->projectionMatrix * lookAtMatrix();
  this->invertedViewProjection = glm::inverse(this->viewProjection);
}

glm::mat4 Camera::getViewProjection()
{
  return this->viewProjection;
}

glm::mat4 Camera::getView()
{
  return this->viewMatrix;
}

glm::mat4 Camera::getProjection()
{
  return this->projectionMatrix;
}

glm::mat4 Camera::getInvertedViewProjection()
{
  return this->invertedViewProjection;
}

Ray Camera::generateRay(glm::vec2 point)
{
  glm::vec4 farWorld = invertedViewProjection * glm::vec4(point.x, point.y, 1.0f, 1.0f);
  
  farWorld /= farWorld.w;
  glm::vec3 pos = this->getPosition();

  Ray ray;
  ray.origin = pos;
  ray.direction = glm::normalize(glm::vec3(farWorld) - pos);

  return ray;
}

void Camera::updateAspect(float aspect)
{
  this->aspect = aspect;
  this->projectionMatrix = glm::perspective(glm::radians(FOV), aspect, N, F);

  this->updateMatrices();
}

void Camera::setPosition(glm::vec3 pos)
{
  Component::setPosition(pos);
  this->updateMatrices();
}

void Camera::setRotation(glm::vec3 rot)
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

void Camera::setScale(glm::vec3)
{
  return;
}

glm::vec3 Camera::getRightVector()
{
  return this->right;
}

glm::vec3 Camera::getUpVector()
{
  return this->up;
}

glm::vec3 Camera::getForwardVector()
{
  return this->direction;
}