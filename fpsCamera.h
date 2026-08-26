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
    glm::vec2 lastMousePos = glm::vec2(0.0f, 0.0f);
    bool firstMouse = true;
};