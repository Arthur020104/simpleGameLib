#pragma once
#include <vector>
#include <gameObject.h>
#include <ray.h>

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

    void handleObjectStart();

    virtual void beforeDrawing();

    virtual void aftherDrawing();

    void destroy(GameObject* obj);

    virtual bool intersectSceneObjects(Ray& ray);
  private:
    std::vector<GameObject*> objects;
    std::vector<Camera*> cameras;

    std::vector<GameObject*> objectsWaitingToStart;

    uint16_t activeCamera = 0;

    //std::vector<Ligth*> lights;
};