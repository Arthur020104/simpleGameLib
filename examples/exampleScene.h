#pragma once
#include <../scene.h>

class ExampleScene: public Scene
{
  public:
    ExampleScene();

    void beforeUpdate() override;
  private:
    Camera* mainCam = nullptr;
    Camera* secondaryCam = nullptr;

    bool inToggle = false;

    void restartScene();
};
