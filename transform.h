#pragma once
#include <cy/cyTriMesh.h>
#include <cy/cyMatrix.h>
#include <cy/cyQuat.h>

class Transform
{
  public:
    Transform(cy::Vec3f pos = cy::Vec3f(0.0f, 0.0f, 0.0f), cy::Quatf rot = cy::Quatf(1.0f, 0.0f, 0.0f, 0.0f), cy::Vec3f scale = cy::Vec3f(1.0f, 1.0f, 1.0f));

    void updateModelMatrix();
    cy::Matrix4f getModelMatrix();

    virtual void setPosition(cy::Vec3f pos);
    virtual void setRotation(cy::Quatf rot);
    virtual void setScale(cy::Vec3f scale);

    cy::Vec3f getPosition() { return this->position; }
    cy::Quatf getRotation() { return this->rotation; }
    cy::Vec3f getScale() { return this->scale; }

  private:
    cy::Matrix4f modelMatrix;

    cy::Vec3f position, scale;
    cy::Quatf rotation;
};