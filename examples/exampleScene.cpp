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

ExampleScene::ExampleScene(): Scene()
{ 
  std::shared_ptr<Material> goldMaterial = std::make_shared<Material>(glm::vec3(1.0f, 0.843f, 0.0f), glm::vec3(1.0f, 0.843f, 0.0f), 256.0f);

  ExampleCamera* cam = new ExampleCamera(glm::vec3(-0.0f, 0.0f, 0.0f));

  ExampleObject* plane = new ExampleObject("/home/arthur/Documents/simpleGame/obj/plane.obj", Program::getDefaultShader());
  this->addObject(plane);
  plane->useOnly(goldMaterial);
  plane->setScale(glm::vec3(100.0f, 1.0f, 100.0f));
  plane->createPhysicalBody(PhysicalShapeType::CUBE);

  glm::vec3 spacing = glm::vec3(1.0f, 1.0f, 0.0f);
  glm::vec3 startPosition = glm::vec3(-9.0f, 1.0f, -9.0f);

  for(uint8_t i = 0; i < 5; i++)
  {
    for(uint8_t j = 0; j < 100; j++)
    {
      std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>("/home/arthur/Documents/simpleGame/obj/square.obj");
      std::shared_ptr<Material> boxMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/container.jpg", glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);

      ExampleObject* box = new ExampleObject("/home/arthur/Documents/simpleGame/obj/square.obj", Program::getDefaultShader(), GameObjectType::DYNAMIC);
      box->useOnly(boxMaterial);
      this->addObject(box);
      box->setPosition(startPosition + spacing * glm::vec3(i, j, 0.0f));
      box->isIntersectable = true;

      box->createPhysicalBody(PhysicalShapeType::CUBE, 1.0f, 1.0f);
    }
  }


  DirectionalLight* light = new DirectionalLight(glm::vec3(0.0f, 3.0f, 2.0f), glm::vec3(1.0, 1.0, 1.0), 1.0f);
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
  crosshair->setScale(glm::vec3(0.05f, 0.07f, 0.1f));

  ExampleUIItem* testItem = new ExampleUIItem("/home/arthur/Documents/simpleGame/obj/container.jpg");
  testItem->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
  testItem->setPosition(glm::vec3(-0.9f, 0.9f, 0.0f));

  this->addUIItem(crosshair);
  this->addUIItem(testItem);
}