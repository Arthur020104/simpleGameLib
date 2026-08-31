#pragma once
#include <exampleObject.h>
#include <../fpsCamera.h>
#include <Libs/box3d/include/box3d/box3d.h>

class ExampleCamera: public FPSCamera
{
  public:
    using FPSCamera::FPSCamera;

    void start() override;
    void beforeUpdate() override;
    void afterUpdate() override;
    void fixedUpdate() override;
  private:
    ExampleObject* lineRay = nullptr;
};