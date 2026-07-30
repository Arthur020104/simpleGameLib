#include <scene.h>
#include <iostream>

void Scene::draw()
{
  std::cout<<"drawing\n";
  for(GameObject* obj: this->objects)
  {
    obj->draw(this->cameras[activeCamera]);
  }
}

Scene::Scene()
{
  Camera* cam = new Camera(cy::Vec3f(0.0f, 0.0f, 0.0f));
  this->cameras.push_back(cam);
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
    obj->beforeDrawing();
  }
}

void Scene::aftherDrawing()
{
  for(GameObject* obj: this->objects)
  {
    obj->aftherDrawing();
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