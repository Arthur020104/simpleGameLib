#pragma once
#include <cy/cyMatrix.h>
#include <component.h>
#include <ray.h>

struct Ray;
class Scene;

class Camera : public Component
{
public:
  Camera(glm::vec3 initialPos);

  glm::mat4 getViewProjection();
  glm::mat4 getInvertedViewProjection();
  glm::mat4 getView();
  glm::mat4 getProjection();

  Ray generateRay(glm::vec2 point);

  void updateAspect(float aspect);

  virtual void setPosition(glm::vec3 pos) override;
  virtual void setRotation(glm::vec3 rot) override;

  virtual glm::vec3 getForwardVector() override;
  virtual glm::vec3 getRightVector() override;
  virtual glm::vec3 getUpVector() override;
  
  void updateMatrices();

private:
  const float N = 0.1f, F = 100000.0f;
  const float FOV = 90.0f;
  float aspect;
  
  glm::vec3 defaultDirection = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 defaultUp        = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 defaultRight     = glm::vec3(1.0f, 0.0f, 0.0f);

  glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up        = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right     = glm::vec3(1.0f, 0.0f, 0.0f);

  glm::mat4 viewMatrix, projectionMatrix, viewProjection, invertedViewProjection;

  glm::mat4& lookAtMatrix();
  
  virtual void setScale(glm::vec3 scale) override;
};
