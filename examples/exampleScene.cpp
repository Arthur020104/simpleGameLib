#include <exampleScene.h>
#include <exampleObject.h>
#include <examplePointLight.h>
#include <exampleCamera.h>
#include <../mesh.h>
#include <../utils.h>
#include <../program.h>
#include <exampleCamera.h>
#include <utils.h>
#include <../material.h>
#include <uiItemExample.h>
#include <../instanceGroup.h>

ExampleScene::ExampleScene(): Scene()
{ 
  this->setGravity(glm::vec3(0.0f, 0.0f, 0.0f));
  std::shared_ptr<Material> planeMaterial = std::make_shared<Material>(glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(1.0f, 1.0f, 1.0f), 512.0f);

  ExampleCamera* cam = new ExampleCamera(glm::vec3(-0.0f, 0.0f, 0.0f));

  ExampleObject* plane = new ExampleObject("/home/arthur/Documents/simpleGame/obj/plane.obj", Program::getDefaultShader());
  plane->isIntersectable = true;
  this->addObject(plane);
  plane->useOnly(planeMaterial);
  plane->setScale(glm::vec3(100.0f, 1.0f, 100.0f));
  plane->createPhysicalBody(PhysicalShapeType::CUBE);

  glm::vec3 spacing = glm::vec3(1.0f, 1.0f, 0.0f);
  glm::vec3 startPosition = glm::vec3(-9.0f, 1.0f, -9.0f);

  std::shared_ptr<Material> boxMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/container.jpg", glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
  std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>("/home/arthur/Documents/simpleGame/obj/square.obj");
  ExampleObject* baseBox = new ExampleObject(boxMesh ,Program::getDefaultShader(), {boxMaterial});

  std::shared_ptr<Program> shader = std::make_shared<Program>("/home/arthur/Documents/simpleGame/shaders/instanced.vs", "/home/arthur/Documents/simpleGame/shaders/instanced.fs");
  InstanceGroup* group = new InstanceGroup(boxMesh,shader);

  for(uint8_t i = 0; i < 70; i++)
  {
    for(uint8_t j = 0; j < 70; j++)
    {
      // std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>("/home/arthur/Documents/simpleGame/obj/square.obj");
      // 

      ExampleObject* box = new ExampleObject(baseBox->getMesh(), Program::getDefaultShader(), baseBox->getMaterials(), GameObjectType::DYNAMIC);
      // box->useOnly(boxMaterial);
      this->addObject(box);
      box->setPosition(startPosition + spacing * glm::vec3(i, j, 0.0f));
      box->isIntersectable = true;

      box->createPhysicalBody(PhysicalShapeType::CUBE, 10000.0f, 1.0f);
      group->addObject(box);
    }
  }
  this->instances.push_back(group);
  delete baseBox;

  DirectionalLight* light = new DirectionalLight(glm::vec3(0.0f, 3.0f, 2.0f), glm::vec3(0.85, 0.85, 1.0), 1.0f);
  ExamplePointLight* pointLight = new ExamplePointLight(glm::vec3(-10.0f, 15.0f, -15.0f), glm::vec3(1.0, 0.5, 0.8), 5.0f);

  this->addLight(pointLight);
  this->addLight(light);
  this->addCamera(cam);
  this->setActiveCam(cam);

  this->addCubeMap({
    "/home/arthur/Documents/simpleGame/obj/kurt/space_rt.png",
    "/home/arthur/Documents/simpleGame/obj/kurt/space_lf.png",
    "/home/arthur/Documents/simpleGame/obj/kurt/space_up.png",
    "/home/arthur/Documents/simpleGame/obj/kurt/space_dn.png",
    "/home/arthur/Documents/simpleGame/obj/kurt/space_bk.png",
    "/home/arthur/Documents/simpleGame/obj/kurt/space_ft.png"
  });

  UIItem* crosshair = new UIItem("/home/arthur/Documents/simpleGame/obj/crosshair.png");
  crosshair->setScale(glm::vec3(0.005f, 0.005f, 0.1f));

  ExampleUIItem* testItem = new ExampleUIItem("/home/arthur/Documents/simpleGame/obj/container.jpg");
  testItem->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
  testItem->setPosition(glm::vec3(-0.9f, 0.9f, 0.0f));

  this->addUIItem(crosshair);
  this->addUIItem(testItem);
}