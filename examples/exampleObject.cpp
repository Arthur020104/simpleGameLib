#include <exampleObject.h>
#include <../window.h>
void ExampleObject::start()
{
  this->setRotation(this->getRotation() + glm::vec3(0.0f, 90, 0.0f));
  return;
}

void ExampleObject::fixedUpdate()
{
  GameObject::fixedUpdate();
  if(glfwGetKey(WINDOW.window, GLFW_KEY_R) == GLFW_PRESS && !this->rotating)
  {
    this->setRotation(this->getRotation() + glm::vec3(0.0f, 5.0f, 0.0f));
    this->rotating = true;
  }
  if(glfwGetKey(WINDOW.window, GLFW_KEY_R) == GLFW_RELEASE)
  {
    this->rotating = false;
  }
  return;
}

void ExampleObject::afterUpdate()
{
  return;
}

void ExampleObject::beforeUpdate()
{
  return;
}