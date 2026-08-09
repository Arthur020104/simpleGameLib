#pragma once
#include <../light.h>
#include <../gameObject.h>
#include <../mesh.h>
#include <../utils.h>
#include <exampleObject.h>

class ExamplePointLight: public PointLight
{
  public: 
    using PointLight::PointLight;

    GameObject* body;

    virtual void start() override
    {
      std::shared_ptr<Mesh> mesh = loadMesh("/home/arthur/Documents/simpleGame/obj/square.obj");
      this->body = new ExampleObject(mesh, DEFAULT_SHADER);
      this->body->setScale(cy::Vec3f(1.0f, 1.0f, 1.0f));
      this->body->setPosition(this->getPosition());
      this->scene->addObject(this->body);
    };

    ~ExamplePointLight()
    {
      if(body != nullptr)
        delete body;
    }
};