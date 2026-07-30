#include <camera.h>
#include <cy/cyMatrix.h>
#include <window.h>

Camera::Camera(cy::Vec3f initialPos): Component(initialPos)
{
  this->aspect = WINDOW.aspect;
  this->projectionMatrix = cy::Matrix4f::Perspective(FOV * cy::Deg2Rad<float>(), WINDOW.aspect, N, F);
  this->direction = direction.GetNormalized();
  
  updateMatrices();
}

cy::Matrix4f& Camera::lookAtMatrix()
{
  cy::Vec3f forward = this->direction.GetNormalized();
  cy::Vec3f right = this->defaultUp ^ forward;
  cy::Vec3f up = forward ^ right;

  viewMatrix.column[0] = cy::Vec4f(right.x, up.x, -forward.x, 0.0f);
  viewMatrix.column[1] = cy::Vec4f(right.y, up.y, -forward.y, 0.0f);
  viewMatrix.column[2] = cy::Vec4f(right.z, up.z, -forward.z, 0.0f);
  viewMatrix.column[3] = cy::Vec4f(-this->position.x, -this->position.y, -this->position.z, 1.0f);

  return viewMatrix;
}

void Camera::updateMatrices()
{
  this->viewProjection = this->projectionMatrix * lookAtMatrix();
  this->invertedViewProjection = viewProjection.GetInverse();
}

cy::Matrix4f& Camera::getViewProjection()
{
  if(WINDOW.aspect != this->aspect) this->updateAspect(WINDOW.aspect);
  //remove update and track to only update for when changes are made
  updateMatrices();
  return this->viewProjection;
}

Ray Camera::generateRay(cy::Vec2f point)
{
  this->updateMatrices();
  cy::Vec4f farWorld = invertedViewProjection * cy::Vec4f(point.x, point.y, 1.0f, 1.0f);
  
  farWorld /= farWorld.w;

  Ray ray;
  ray.origin = this->position;
  ray.direction = (farWorld.XYZ() - this->position).GetNormalized();

  return ray;
}

void Camera::updateAspect(float aspect)
{
  this->aspect = aspect;
  this->projectionMatrix = cy::Matrix4f::Perspective(FOV * cy::Deg2Rad<float>(), aspect, N, F);

  this->updateMatrices();
}