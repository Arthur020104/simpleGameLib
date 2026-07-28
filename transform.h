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

    cy::Vec3f position, scale;
    cy::Quatf rotation;
  private:
    
    cy::Matrix4f modelMatrix;
};