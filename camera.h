#pragma once
#include <cy/cyMatrix.h>
#include <component.h>
#include <ray.h>

struct Ray;
class Scene;

class Camera : public Component
{
public:
  Camera(cy::Vec3f initialPos);

  cy::Matrix4f getViewProjection();
  cy::Matrix4f getInvertedViewProjection();
  cy::Matrix4f getView();
  cy::Matrix4f getProjection();

  Ray generateRay(cy::Vec2f point);

  void updateAspect(float aspect);

  virtual void setPosition(cy::Vec3f pos) override;
  virtual void setRotation(cy::Quatf rot) override;
  
  void updateMatrices();

private:
  const float N = 0.1f, F = 100000.0f;
  const float FOV = 103.0f;
  float aspect;
  
  cy::Vec3f direction = cy::Vec3f(0.0f, 0.0f, -1.0f), defaultUp = cy::Vec3f(0.0f, 1.0f, 0.0f);

  cy::Matrix4f viewMatrix, projectionMatrix, viewProjection, invertedViewProjection;

  cy::Matrix4f& lookAtMatrix();
  
  virtual void setScale(cy::Vec3f scale) override;
};
