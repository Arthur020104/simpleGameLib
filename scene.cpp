#include <scene.h>
#include <iostream>
#include <window.h>
#include <algorithm>

void Scene::draw()
{
  if(cameras.size() <= 0) throw std::runtime_error("At least one camera is necessary for rendering");

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
}

void Scene::addUIItem(UIItem* uiItem)
{ 
  this->ui->addUIItem(uiItem); 
}

void Scene::handleStart()
{
  while(!this->destroyQueue.empty())
  {
    Component* obj = this->destroyQueue.front();
    this->destroyQueue.pop();
    erase(obj);
  }

  for(Component* obj: this->componentsWaitingToStart)
  {
    obj->start();
    this->components.push_back(obj);
  }

  this->componentsWaitingToStart.clear();
}

void Scene::beforeDrawing()
{
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
}

void Scene::afterDrawing()
{
  for(Component* obj: this->components)
  {
    obj->afterUpdate();
  }
}

void Scene::addCubeMap(std::vector<std::string> facePaths)
{
  this->cubeMap = new CubeMap(facePaths);
  this->hasCubeMap = true;
}

void Scene::addObject(GameObject* obj)
{
  obj->scene = this;
  this->objects.push_back(obj);
  this->componentsWaitingToStart.push_back(obj);
}

void Scene::addCamera(Camera* cam)
{
  cam->scene = this;
  this->cameras.push_back(cam);
  this->componentsWaitingToStart.push_back(cam);
}

void Scene::setActiveCam(uint16_t activeCam)
{
  if(cameras.size() <= activeCam) throw std::runtime_error("Cannot set the active camera: the specified camera index does not exist.");

  this->activeCamera = activeCam;
}

void Scene::setActiveCam(Camera* cam)
{
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
    if(obj->intersect(ray)) hitSomething = true;
  }

  return hitSomething;
}

void Scene::erase(Component* obj)
{
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
  if(shaderProgram == nullptr) return;

  for(uint16_t i = 0; i < this->lights.size(); i++)
  {
    this->lights[i]->bind(shaderProgram, "lights", i);
  }

  shaderProgram->bindUint("lightsSize", this->lights.size());
}

void Scene::addLight(Light* light)
{
  light->scene = this;
  this->lights.push_back(light);
  this->componentsWaitingToStart.push_back(light);
}

void Scene::destroy(Component* obj)
{
  this->destroyQueue.push(obj);
}