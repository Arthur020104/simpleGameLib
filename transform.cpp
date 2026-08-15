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

  cy::Matrix4f objRotMat = cy::Matrix4f::Rotation(cy::Vec3f(1.0f, 0.0f, 0.0f), cy::Deg2Rad<float>() * this->getRotation().x); // rotate around X (pitch)
    objRotMat = objRotMat * cy::Matrix4f::Rotation(cy::Vec3f(0.0f, 1.0f, 0.0f), cy::Deg2Rad<float>() * this->getRotation().y); // rotate around Y (yaw)
    objRotMat = objRotMat * cy::Matrix4f::Rotation(cy::Vec3f(0.0f, 0.0f, 1.0f), cy::Deg2Rad<float>() * this->getRotation().z); // rotate around Z (roll)
  cy::Matrix4f scaleMatrix = cy::Matrix4f::Scale(this->scale);

  this->modelMatrix = positionMatrix * objRotMat * scaleMatrix;
}

cy::Matrix4f Transform::getModelMatrix()
{
  this->updateModelMatrix();
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
