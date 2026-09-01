#include <scene.h>
#include <iostream>
#include <window.h>
#include <algorithm>
#include <utils.h>

void Scene::draw()
{
  if(cameras.size() <= 0) throw std::runtime_error("At least one camera is necessary for rendering");

  std::scoped_lock lock(this->objectsLock, this->camerasLock, this->uiLock);

  for(GameObject* obj: this->objects)
  {
    obj->draw(this->cameras[activeCamera]); 
  }

  if(hasCubeMap)
    cubeMap->draw(this->cameras[activeCamera]);

  this->ui->draw();
}

Scene::Scene()
{ 
  this->ui = new UI();
  this->ui->scene = this;
  this->componentsWaitingToStart.push_back(this->ui);

  b3WorldDef worldDef = b3DefaultWorldDef();
  worldDef.gravity = (b3Vec3){ this->gravity.x, this->gravity.y, this->gravity.z };

  this->worldId = b3CreateWorld(&worldDef);
}

Scene::~Scene()
{
  for(Component* obj: this->components)
  {
    delete obj;
  }

  for(Component* obj: this->componentsWaitingToStart)
  {
    delete obj;
  }

  if(hasCubeMap)
    delete cubeMap;
  
  b3DestroyWorld(worldId);
}

void Scene::setGravity(glm::vec3 gravity)
{
  this->gravity = gravity;
  b3World_SetGravity(this->worldId, (b3Vec3){ gravity.x, gravity.y, gravity.z });
}

void Scene::addUIItem(UIItem* uiItem)
{ 
  std::scoped_lock lock(this->uiLock);
  this->ui->addUIItem(uiItem); 
}

void Scene::handleStart()
{
  
  while(true)
  {
    Component* objToDelete = nullptr;
    {
      std::scoped_lock lock(this->destroyQueueLock);
      if(this->destroyQueue.empty()) break;

      objToDelete = *this->destroyQueue.begin();
      this->destroyQueue.erase(this->destroyQueue.begin());
    }
    
    if(objToDelete)
    {
      this->erase(objToDelete);
    }
  }

  std::vector<Component*> toStart;
  {
    std::scoped_lock componentsLock(this->componentsWaitingToStartLock);
    toStart = std::move(this->componentsWaitingToStart);
    this->componentsWaitingToStart.clear();
  }

  for(Component* obj: toStart)
  {
    obj->start();
    this->components.push_back(obj);
  }
}

void Scene::beforeUpdate()
{
  //Placeholder
  if(glfwGetKey(WINDOW.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(WINDOW.window, true);
  }

  std::scoped_lock lock(this->camerasLock);

  if(WINDOW.updateCameras)
  {
    for(Camera* cam: this->cameras)
    {
      cam->updateMatrices();
    }
    WINDOW.updateCameras = false;
  }
  
  for(Component* obj: this->components)
  {
    obj->beforeUpdate();
  }

  this->accumulatedTime += WINDOW.deltaTime;

  while(this->accumulatedTime >= this->timeStep)
  {
    this->fixedUpdate();
    b3World_Step(this->worldId, this->timeStep, this->subStepCount);
    this->accumulatedTime -= this->timeStep;
  }
}

void Scene::afterUpdate()
{
  for(Component* obj: this->components)
  {
    obj->afterUpdate();
  }
}

void Scene::fixedUpdate()
{
  for(Component* obj: this->components)
  {
    obj->fixedUpdate();
  }
}

void Scene::addCubeMap(std::vector<std::string> facePaths)
{
  this->cubeMap = new CubeMap(facePaths);
  this->hasCubeMap = true;
}

void Scene::addObject(GameObject* obj)
{
  std::scoped_lock lock(this->objectsLock, this->componentsWaitingToStartLock);

  obj->scene = this;
  this->objects.push_back(obj);
  this->componentsWaitingToStart.push_back(obj);
}

void Scene::addCamera(Camera* cam)
{
  std::scoped_lock lock(this->camerasLock, this->componentsWaitingToStartLock);

  cam->scene = this;
  this->cameras.push_back(cam);
  this->componentsWaitingToStart.push_back(cam);
}

void Scene::setActiveCam(uint16_t activeCam)
{
  std::scoped_lock lock(this->camerasLock);

  if(cameras.size() <= activeCam) throw std::runtime_error("Cannot set the active camera: the specified camera index does not exist.");

  this->activeCamera = activeCam;
}

void Scene::setActiveCam(Camera* cam)
{ 
  std::scoped_lock lock(this->camerasLock);

  for(uint16_t i = 0; i < this->cameras.size(); i++)
  {
    if(cam == this->cameras[i])
    {
      this->activeCamera = i;
      break;
    }
  }
}

bool Scene::intersectSceneObjects(Ray& ray)
{
  bool hitSomething = false;

  for(GameObject* obj: this->objects)
  {
    if(obj->isIntersectable && obj->intersect(ray)) hitSomething = true;
  }

  return hitSomething;
}

void Scene::erase(Component* obj)
{
  std::scoped_lock lock(this->objectsLock, this->componentsWaitingToStartLock, this->camerasLock, this->lightsLock);

  bool validForDeletion = false;
  
  auto itemOnObjects = std::find(this->objects.begin(), this->objects.end(), obj);
  if(itemOnObjects != this->objects.end())
  {
    this->objects.erase(itemOnObjects);
    validForDeletion = true;
  }

  auto itemOnComponents = std::find(this->components.begin(), this->components.end(), obj);
  if(itemOnComponents != this->components.end())
  {
    this->components.erase(itemOnComponents);
    validForDeletion = true;
  }

  auto itemOnStartComponents = std::find(this->componentsWaitingToStart.begin(), this->componentsWaitingToStart.end(), obj);
  if(itemOnStartComponents != this->componentsWaitingToStart.end())
  {
    this->componentsWaitingToStart.erase(itemOnStartComponents);
    validForDeletion = true;
  }

  auto itemOnCameras = std::find(this->cameras.begin(), this->cameras.end(), obj);
  if(itemOnCameras != this->cameras.end())
  {
    this->cameras.erase(itemOnCameras);
    validForDeletion = true;
  }

  auto itemOnLights = std::find(this->lights.begin(), this->lights.end(), obj);
  if(itemOnLights != this->lights.end())
  {
    this->lights.erase(itemOnLights);
    validForDeletion = true;
  }

  if(!validForDeletion) return;
  
  delete obj;
}

void Scene::bindSceneLights(Program* shaderProgram)
{
  std::scoped_lock lock(this->lightsLock);

  if(shaderProgram == nullptr) return;

  for(uint16_t i = 0; i < this->lights.size(); i++)
  {
    this->lights[i]->bind(shaderProgram, "lights", i);
  }

  shaderProgram->bindUint("lightsSize", this->lights.size());
}

void Scene::addLight(Light* light)
{
  std::scoped_lock lock(this->lightsLock, this->componentsWaitingToStartLock);

  light->scene = this;
  this->lights.push_back(light);
  this->componentsWaitingToStart.push_back(light);
}

void Scene::destroy(Component* obj)
{
  if(obj == nullptr) return;

  std::scoped_lock lock(this->destroyQueueLock);

  this->destroyQueue.insert(obj);
}