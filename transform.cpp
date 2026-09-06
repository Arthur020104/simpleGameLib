#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <transform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Transform::Transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
{
  this->position = pos;
  this->rotation = glm::normalize(rot);
  this->eulerRotation = glm::degrees(glm::eulerAngles(this->rotation));
  this->scale = scale;

  this->updateModelMatrix();
}
//https://stackoverflow.com/questions/17038681/direction-of-rotation-in-glm-matrix-using-quaternions
void Transform::updateModelMatrix()
{
  glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), this->position);

  glm::quat q = glm::normalize(this->rotation);
  this->rotationMatrix = glm::mat4_cast(q);

  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), this->scale);

  this->modelMatrix = positionMatrix * this->rotationMatrix * scaleMatrix;
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
  this->eulerRotation = rot;
  this->rotation = glm::normalize(glm::quat(glm::radians(rot)));
  this->updateModelMatrix();
}

void Transform::setRotation(glm::quat rot)
{
  this->rotation = glm::normalize(rot);
  this->eulerRotation = glm::degrees(glm::eulerAngles(this->rotation)); 
  this->updateModelMatrix();
}

void Transform::setScale(glm::vec3 scale)
{
  this->scale = scale;
  this->updateModelMatrix();
}

glm::vec3 Transform::getRotation()
{
  return this->eulerRotation;
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

const glm::mat4& Transform::getRotationMatrix()
{
  return this->rotationMatrix;
}
