#pragma once
#include <movementController.h>
#include <exampleObject.h>
#include <fpsCamera.h>
#include <Libs/box3d/include/box3d/box3d.h>

class ExampleCamera: public FPSCamera
{
  public:
    using FPSCamera::FPSCamera;

    void start() override;
    void beforeUpdate() override;
    void afterUpdate() override;
    void fixedUpdate() override;

    void updatePostion() override {};

    MovementController* body = nullptr;
    glm::vec3 positionOffset = glm::vec3(0.0f, 1.0f, 0.0f);

    float minDistance = 1.0f;
  private:
    ExampleObject* lineRay = nullptr;

    bool isShooting = false;
};