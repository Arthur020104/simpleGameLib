#pragma once
#include <set>
#include <ui.h>
#include <ray.h>
#include <vector>
#include <light.h>
#include <cubeMap.h>
#include <component.h>
#include <timeQueue.h>
#include <gameObject.h>

class GameObject;
class Camera;
class Program;
class DirectionalLight;
struct Ray;
class UIItem;
class UI;

class Scene
{
  public:
    Scene();

    ~Scene();

    void addObject(GameObject* obj);

    void addCamera(Camera* cam);

    void setActiveCam(uint16_t activeCam);

    void setActiveCam(Camera* cam);

    void addUIItem(UIItem* uiItem);

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
    std::set<Component*> destroyQueue;

    TimeQueue timeQueue;

    CubeMap* cubeMap;

    UI* ui;

    bool hasCubeMap = false;

    uint16_t activeCamera = 0;

    void erase(Component* obj);
    //std::vector<Ligth*> lights;
};