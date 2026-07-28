#include <transform.h>
#include <cy/cyTriMesh.h>
#include <cy/cyMatrix.h>
#include <cy/cyQuat.h>

Transform::Transform(cy::Vec3f pos, cy::Quatf rot, cy::Vec3f scale)
{
  this->position = pos;
  this->rotation = rot;
  this->scale = scale;

  this->updateModelMatrix();
}

void Transform::updateModelMatrix()
{
  cy::Matrix4f positionMatrix = cy::Matrix4f::Translation(this->position);
  cy::Matrix4f rotationMatrix = this->rotation.ToMatrix4();
  cy::Matrix4f scaleMatrix = cy::Matrix4f::Scale(this->scale);

  this->modelMatrix = positionMatrix * rotationMatrix * scaleMatrix;
}

cy::Matrix4f Transform::getModelMatrix()
{
  updateModelMatrix();
  return this->modelMatrix;
}