#pragma once
#include <vector>
#include <gameObject.h>
#include <ray.h>
#include <component.h>
#include <queue>

class GameObject;
class Camera;
struct Ray;

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

    void handleStart();

    virtual void beforeDrawing();

    virtual void aftherDrawing();

    void destroy(Component* obj);

    virtual bool intersectSceneObjects(Ray& ray);
  private:
    std::vector<GameObject*> objects;
    std::vector<Camera*> cameras;
    std::vector<Component*> components;

    std::vector<Component*> componentsWaitingToStart;
    std::queue<Component*> destroyQueue;

    uint16_t activeCamera = 0;

    void erase(Component* obj);
    //std::vector<Ligth*> lights;
};