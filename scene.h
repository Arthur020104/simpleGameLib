#pragma once
#include <vector>
#include <gameObject.h>
#include <camera.h>

class GameObject;
class Camera;

class Scene
{
  public:
    Scene();

    ~Scene();

    void addObject(GameObject* obj);

    void addCamera(Camera* cam);

    void setActiveCam(uint16_t activeCam);

    void setActiveCam(Camera* cam);

    void draw();

    void handleObjectStart();

    virtual void beforeDrawing();

    virtual void aftherDrawing();
  private:
    std::vector<GameObject*> objects;
    std::vector<Camera*> cameras;

    std::vector<GameObject*> objectsWaitingToStart;

    uint16_t activeCamera = 0;
    //std::vector<Ligth*> lights;
};