#pragma once
#include <cy/cyTriMesh.h>
#include <cy/cyMatrix.h>
#include <cy/cyQuat.h>

class Transform
{
  public:
    Transform(cy::Vec3f pos = cy::Vec3f(0.0f, 0.0f, 0.0f), cy::Vec3f rot = cy::Vec3f(0.0f, 0.0f, 0.0f), cy::Vec3f scale = cy::Vec3f(1.0f, 1.0f, 1.0f));

    void updateModelMatrix();
    cy::Matrix4f getModelMatrix();

    virtual void setPosition(cy::Vec3f pos);
    virtual void setRotation(cy::Vec3f rot);
    virtual void setScale(cy::Vec3f scale);

    virtual cy::Vec3f getPosition() { return this->position; }
    virtual cy::Vec3f getRotation() { return this->rotation; }
    virtual cy::Vec3f getScale() { return this->scale; }
    virtual cy::Vec3f getNormalizedPosition() { return this->position.GetNormalized(); }

    virtual cy::Vec3f getForwardVector();
    virtual cy::Vec3f getRightVector();
    virtual cy::Vec3f getUpVector();

  private:
    cy::Matrix4f modelMatrix, rotationMatrix;

    cy::Vec3f position, scale, rotation;
};