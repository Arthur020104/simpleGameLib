#pragma once
#include <../scene.h>

class MainScene : public Scene
{
  public:
    MainScene();
    ~MainScene();
    void beforeDrawing() override;
    void afterDrawing() override;
  private:
    
};

