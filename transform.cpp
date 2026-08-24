#include <transform.h>
#include <cy/cyTriMesh.h>
#include <cy/cyMatrix.h>
#include <cy/cyQuat.h>

Transform::Transform(cy::Vec3f pos, cy::Vec3f rot, cy::Vec3f scale)
{
  this->position = pos;
  this->rotation = rot;
  this->scale = scale;

  this->updateModelMatrix();
}

void Transform::updateModelMatrix()
{
  cy::Matrix4f positionMatrix = cy::Matrix4f::Translation(this->position);

  this->rotationMatrix = cy::Matrix4f::Rotation(cy::Vec3f(1.0f, 0.0f, 0.0f), cy::Deg2Rad<float>() * this->getRotation().x);
  this->rotationMatrix = this->rotationMatrix * cy::Matrix4f::Rotation(cy::Vec3f(0.0f, 1.0f, 0.0f), cy::Deg2Rad<float>() * this->getRotation().y); 
  this->rotationMatrix = this->rotationMatrix * cy::Matrix4f::Rotation(cy::Vec3f(0.0f, 0.0f, 1.0f), cy::Deg2Rad<float>() * this->getRotation().z);

  cy::Matrix4f scaleMatrix = cy::Matrix4f::Scale(this->scale);

  this->modelMatrix = positionMatrix * rotationMatrix * scaleMatrix;
}

cy::Matrix4f Transform::getModelMatrix()
{
  return this->modelMatrix;
}

void Transform::setPosition(cy::Vec3f pos)
{
  this->position = pos;
  this->updateModelMatrix();
}

void Transform::setRotation(cy::Vec3f rot)
{
  this->rotation = rot;
  this->updateModelMatrix();
}

void Transform::setScale(cy::Vec3f scale)
{
  this->scale = scale;
  this->updateModelMatrix();
}

cy::Vec3f Transform::getForwardVector()
{
  return -this->rotationMatrix.Column(2).XYZ().GetNormalized();
}

cy::Vec3f Transform::getRightVector()
{
  return this->rotationMatrix.Column(0).XYZ().GetNormalized();
}

cy::Vec3f Transform::getUpVector()
{
  return this->rotationMatrix.Column(1).XYZ().GetNormalized();
}