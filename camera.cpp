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
  right.Normalize();

  cy::Vec3f up = forward ^ right;
  up.Normalize();

  cy::Vec3f pos = this->getPosition();

  viewMatrix.column[0] = cy::Vec4f(right.x, up.x, -forward.x, 0.0f);
  viewMatrix.column[1] = cy::Vec4f(right.y, up.y, -forward.y, 0.0f);
  viewMatrix.column[2] = cy::Vec4f(right.z, up.z, -forward.z, 0.0f);
  viewMatrix.column[3] = cy::Vec4f(-(pos % right), -(pos % up), pos % forward, 1.0f);

  return viewMatrix;
}

void Camera::updateMatrices()
{
  if(WINDOW.aspect != this->aspect) this->updateAspect(WINDOW.aspect);
  this->viewProjection = this->projectionMatrix * lookAtMatrix();
  this->invertedViewProjection = viewProjection.GetInverse();
}

cy::Matrix4f& Camera::getViewProjection()
{
  return this->viewProjection;
}

Ray Camera::generateRay(cy::Vec2f point)
{
  cy::Vec4f farWorld = invertedViewProjection * cy::Vec4f(point.x, point.y, 1.0f, 1.0f);
  
  farWorld /= farWorld.w;
  cy::Vec3f pos = this->getPosition();

  Ray ray;
  ray.origin = pos;
  ray.direction = (farWorld.XYZ() - pos).GetNormalized();

  return ray;
}

void Camera::updateAspect(float aspect)
{
  this->aspect = aspect;
  this->projectionMatrix = cy::Matrix4f::Perspective(FOV * cy::Deg2Rad<float>(), aspect, N, F);

  this->updateMatrices();
}

void Camera::setPosition(cy::Vec3f pos)
{
  Component::setPosition(pos);
  this->updateMatrices();
}

void Camera::setRotation(cy::Quatf rot)
{
  Component::setRotation(rot);
  this->direction = (rot.ToMatrix4() * cy::Vec4f(this->direction, 1.0f)).XYZ().GetNormalized(); ;
  this->updateMatrices();
}

void Camera::setScale(cy::Vec3f)
{
  return;
}