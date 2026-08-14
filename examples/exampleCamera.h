#pragma once
#include <../gameObject.h>
#include <exampleObject.h>

class ExampleCamera: public Camera
{
  public:
    using Camera::Camera;

    void start() override;
    void beforeUpdate() override;
    void afterUpdate() override;
  private:
    ExampleObject* lineRay = nullptr;

    float velocity = 100.0f;
};