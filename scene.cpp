#include <scene.h>
#include <iostream>
#include <algorithm>

void Scene::draw()
{
  if(cameras.size() <= 0) throw std::runtime_error("At least one camera is necessary for rendering");
  for(GameObject* obj: this->objects)
  {
    obj->draw(this->cameras[activeCamera]);
  }
}

Scene::Scene()
{ 
}

Scene::~Scene()
{
  for(GameObject* objP: objectsWaitingToStart)
  {
    delete objP;
  }

  for(GameObject* objP: objects)
  {
    delete objP;
  }

  for(Camera* cam: cameras)
  {
    delete cam;
  }
}

void Scene::handleObjectStart()
{
  for(GameObject* obj: this->objectsWaitingToStart)
  {
    obj->start();
    this->objects.push_back(obj);
  }

  this->objectsWaitingToStart.clear();
}

void Scene::beforeDrawing()
{
  for(GameObject* obj: this->objects)
  {
    obj->beforeUpdate();
  }

  for(Camera* obj: this->cameras)
  {
    obj->beforeUpdate();
  }
}

void Scene::aftherDrawing()
{
  for(GameObject* obj: this->objects)
  {
    obj->aftherUpdate();
  }

  for(Camera* obj: this->cameras)
  {
    obj->aftherUpdate();
  }
}

void Scene::addObject(GameObject* obj)
{
  obj->scene = this;
  this->objectsWaitingToStart.push_back(obj);
}

void Scene::addCamera(Camera* cam)
{
  cam->scene = this;
  this->cameras.push_back(cam);
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
    if(obj->intersect(ray) && !hitSomething) hitSomething = true;
  }

  return hitSomething;
}

void Scene::destroy(GameObject* obj)
{
  auto itemOnObjects = std::find(this->objects.begin(), this->objects.end(), obj);

  if(itemOnObjects != this->objects.end())
  {
    this->objects.erase(itemOnObjects);
    return;
  }

  auto itemOnStartObjects = std::find(this->objectsWaitingToStart.begin(), this->objectsWaitingToStart.end(), obj);

  if(itemOnStartObjects != this->objectsWaitingToStart.end()) this->objectsWaitingToStart.erase(itemOnStartObjects);

}