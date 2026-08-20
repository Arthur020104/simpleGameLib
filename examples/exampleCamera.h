#pragma once
#include <exampleObject.h>
#include <../fpsCamera.h>

class ExampleCamera: public FPSCamera
{
  public:
    using FPSCamera::FPSCamera;

    void start() override;
    void beforeUpdate() override;
    void afterUpdate() override;
  private:
    ExampleObject* lineRay = nullptr;
};