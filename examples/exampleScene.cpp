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
#include <grassPlane.h>

ExampleScene::ExampleScene(): Scene()
{ 
  this->setGravity(glm::vec3(0.0f, -12.0f, 0.0f));
  std::shared_ptr<Material> planeMaterial = std::make_shared<Material>(glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(1.0f, 1.0f, 1.0f), 512.0f);

  ExampleCamera* cam = new ExampleCamera(glm::vec3(-0.0f, 0.0f, 0.0f));

  GrassPlane* grassPlane = new GrassPlane(glm::vec3(0.0f, 0.0f, 0.0f));
  this->addObject(grassPlane);
  grassPlane->setScale(glm::vec3(500.0f, 1.0f, 500.0f));
  grassPlane->createPhysicalBody(PhysicalShapeType::CUBE, 10000.0f, 1.0f);
  grassPlane->isIntersectable = true;

  glm::vec3 spacing = glm::vec3(1.0f, 1.0f, 0.0f);
  glm::vec3 startPosition = glm::vec3(-9.0f, 1.0f, -9.0f);
  
  std::shared_ptr<Material> boxMaterial2 = std::make_shared<Material>(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
  std::shared_ptr<Material> boxMaterial3 = std::make_shared<Material>(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
  std::shared_ptr<Material> boxMaterial4 = std::make_shared<Material>(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
  std::shared_ptr<Material> boxMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/container.jpg", glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
  std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>("/home/arthur/Documents/simpleGame/obj/square.obj");
  ExampleObject* baseBox = new ExampleObject(boxMesh ,Program::getDefaultShader(), {boxMaterial, boxMaterial2, boxMaterial3, boxMaterial4});

  std::shared_ptr<Program> shader = std::make_shared<Program>("/home/arthur/Documents/simpleGame/shaders/instanced.vs", "/home/arthur/Documents/simpleGame/shaders/instanced.fs");
  
  std::vector<GameObject*> boxes;

  for(uint8_t i = 0; i < 30; i++)
  {
    for(uint8_t j = 0; j < 30; j++)
    {
      ExampleObject* box = new ExampleObject(baseBox->getMesh(), Program::getDefaultShader(), baseBox->getMaterials(), GameObjectType::DYNAMIC);

      box->setPosition(startPosition + spacing * glm::vec3(i, j, 0.0f));
      box->isIntersectable = true;
      this->addObject(box);
      box->createPhysicalBody(PhysicalShapeType::CUBE, 10000.0f, 1.0f);
      boxes.push_back(box);
    }
  }
  
  InstanceGroup* group = new InstanceGroup(boxMesh, true, shader, boxes);
  this->instances.push_back(group);
  delete baseBox;

  DirectionalLight* light = new DirectionalLight(glm::vec3(0.0f, 3.0f, 2.0f), glm::vec3(0.85, 0.85, 1.0), 1.0f);
  ExamplePointLight* pointLight = new ExamplePointLight(glm::vec3(-10.0f, 15.0f, -15.0f), glm::vec3(1.0, 0.5, 0.8), 5.0f);

  this->addLight(pointLight);
  this->addLight(light);
  this->addCamera(cam);
  this->setActiveCam(cam);

  this->addCubeMap({
    "/home/arthur/Documents/simpleGame/obj/Cubemaps_2025-07-25/20250717_210302_0772_rt.png",
    "/home/arthur/Documents/simpleGame/obj/Cubemaps_2025-07-25/20250717_210302_0772_lf.png",
    "/home/arthur/Documents/simpleGame/obj/Cubemaps_2025-07-25/20250717_210302_0772_up.png",
    "/home/arthur/Documents/simpleGame/obj/Cubemaps_2025-07-25/20250717_210302_0772_dn.png",
    "/home/arthur/Documents/simpleGame/obj/Cubemaps_2025-07-25/20250717_210302_0772_bk.png",
    "/home/arthur/Documents/simpleGame/obj/Cubemaps_2025-07-25/20250717_210302_0772_ft.png"
  });

  UIItem* crosshair = new UIItem("/home/arthur/Documents/simpleGame/obj/crosshair.png");
  crosshair->setScale(glm::vec3(0.005f, 0.005f, 0.1f));

  ExampleUIItem* testItem = new ExampleUIItem("/home/arthur/Documents/simpleGame/obj/container.jpg");
  testItem->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
  testItem->setPosition(glm::vec3(-0.9f, 0.9f, 0.0f));

  this->addUIItem(crosshair);
  this->addUIItem(testItem);
}