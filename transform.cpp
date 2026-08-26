#define GLM_FORCE_SWIZZLE
#include <transform.h>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
  this->position = pos;
  this->rotation = rot;
  this->scale = scale;

  this->updateModelMatrix();
}

void Transform::updateModelMatrix()
{
  glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), this->position);

  this->rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
  this->rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f)) * this->rotationMatrix;
  this->rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f)) * this->rotationMatrix;
  
  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), this->scale);

  this->modelMatrix = positionMatrix * rotationMatrix * scaleMatrix;
}

const glm::mat4& Transform::getModelMatrix()
{
  return this->modelMatrix;
}

void Transform::setPosition(glm::vec3 pos)
{
  this->position = pos;
  this->updateModelMatrix();
}

void Transform::setRotation(glm::vec3 rot)
{
  this->rotation = rot;
  this->updateModelMatrix();
}

void Transform::setScale(glm::vec3 scale)
{
  this->scale = scale;
  this->updateModelMatrix();
}

glm::vec3 Transform::getForwardVector()
{
  return -glm::normalize(this->rotationMatrix[2].xyz());
}

glm::vec3 Transform::getRightVector()
{
  return glm::normalize(this->rotationMatrix[0].xyz());
}

glm::vec3 Transform::getUpVector()
{
  return glm::normalize(this->rotationMatrix[1].xyz());
}