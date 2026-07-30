#pragma once
#include <cy/cyMatrix.h>
#include <transform.h>
#include <ray.h>
#include <scene.h>

struct Ray;
class Scene;

class Camera : public Transform
{
public:
  Camera(cy::Vec3f initialPos);

  cy::Matrix4f& getViewProjection();

  Ray generateRay(cy::Vec2f point);

  Scene* scene;
private:
  const float N = 0.1f, F = 500.0f;
  const float FOV = 103.0f;
  
  cy::Vec3f direction = cy::Vec3f(0.0f, 0.0f, -1.0f), defaultUp = cy::Vec3f(0.0f, 1.0f, 0.0f);

  cy::Matrix4f viewMatrix, projectionMatrix, viewProjection, invertedViewProjection;

  void updateMatrices();

  cy::Matrix4f& lookAtMatrix();
};

