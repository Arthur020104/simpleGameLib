#pragma once
#include <set>
#include <ui.h>
#include <ray.h>
#include <mutex>
#include <vector>
#include <light.h>
#include <cubeMap.h>
#include <component.h>
#include <timeQueue.h>
#include <gameObject.h>
#include <Libs/box3d/include/box3d/box3d.h>

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

    void handleStart();
    virtual void beforeUpdate();
    void draw();
    virtual void afterUpdate();

    void addObject(GameObject* obj);
    void addCamera(Camera* cam);
    void addLight(Light* light);
    void addUIItem(UIItem* uiItem);
    void addCubeMap(std::vector<std::string> facePaths);
    void destroy(Component* obj);

    void setActiveCam(uint16_t activeCam);
    void setActiveCam(Camera* cam);
    Camera* getActiveCamera() { return this->cameras[this->activeCamera]; }

    void bindSceneLights(Program* shaderProgram);
    virtual bool intersectSceneObjects(Ray& ray);
    TimeQueue* getTimeQueue() { return &this->timeQueue; }

    b3WorldId getWorldId() { return this->worldId; }

  private:
    void erase(Component* obj);

    std::vector<GameObject*> objects;
    mutable std::mutex objectsLock;

    std::vector<Camera*> cameras;
    mutable std::mutex camerasLock;
    uint16_t activeCamera = 0;

    std::vector<Light*> lights;
    mutable std::mutex lightsLock;

    std::vector<Component*> components;

    std::vector<Component*> componentsWaitingToStart;
    mutable std::mutex componentsWaitingToStartLock;

    std::set<Component*> destroyQueue;
    mutable std::mutex destroyQueueLock;

    CubeMap* cubeMap;
    bool hasCubeMap = false;

    UI* ui;
    mutable std::mutex uiLock;

    float timeStep = 1.0f / 60.0f;
    uint8_t subStepCount = 4;
    
    bool hasStartedPhysics = false;

    double accumulatedTime = 0.0f;

    TimeQueue timeQueue;
    b3WorldId worldId;

    glm::vec3 gravity = glm::vec3(0.0f, -10.0f, 0.0f);

    void fixedUpdate();
  protected:
    void setGravity(glm::vec3 gravity);
};