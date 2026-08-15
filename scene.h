#pragma once
#include <light.h>
#include <vector>
#include <gameObject.h>
#include <ray.h>
#include <cubeMap.h>
#include <component.h>
#include <queue>
#include <timeQueue.h>

class GameObject;
class Camera;
class Program;
class DirectionalLight;
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

    virtual void afterDrawing();

    void destroy(Component* obj);

    virtual bool intersectSceneObjects(Ray& ray);

    void addCubeMap(std::vector<std::string> facePaths);

    void addLight(Light* light);

    void bindSceneLights(Program* shaderProgram);

    TimeQueue* getTimeQueue() { return &this->timeQueue; }

    Camera* getActiveCamera() { return this->cameras[this->activeCamera]; }
  private:
    std::vector<GameObject*> objects;
    std::vector<Camera*> cameras;
    std::vector<Component*> components;
    std::vector<Light*> lights;

    std::vector<Component*> componentsWaitingToStart;
    std::queue<Component*> destroyQueue;

    TimeQueue timeQueue;

    CubeMap* cubeMap;
    bool hasCubeMap = false;

    uint16_t activeCamera = 0;

    void erase(Component* obj);
    //std::vector<Ligth*> lights;
};