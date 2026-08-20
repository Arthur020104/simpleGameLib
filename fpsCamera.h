#pragma once
#include <camera.h>

class FPSCamera : public Camera
{
  public:
    using Camera::Camera;

    void start() override;
    void beforeUpdate() override;
    void afterUpdate() override;
  protected:
    float velocity = 100.0f;
    float sensitivity = 0.1f;
  private:
    cy::Vec2f lastMousePos = cy::Vec2f(0.0f, 0.0f);
    bool firstMouse = true;
};