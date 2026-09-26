#include <exampleScene.h>
#include <new>
#include <exampleObject.h>
#include <examplePointLight.h>
#include <exampleCamera.h>
#include <mesh.h>
#include <utils.h>
#include <program.h>
#include <exampleCamera.h>
#include <utils.h>
#include <material.h>
#include <uiItemExample.h>
#include <instanceGroup.h>
#include <baseMover.h>
#include <grassPlane.h>
#include <movementController.h>

ExampleScene::ExampleScene(): Scene()
{
  this->setGravity(glm::vec3(0.0f, -12.0f, 0.0f));
  std::shared_ptr<Material> planeMaterial = std::make_shared<Material>(glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(1.0f, 1.0f, 1.0f), 512.0f);

  ExampleCamera* cam = new ExampleCamera(glm::vec3(-0.0f, 0.0f, 0.0f));

  GrassPlane* grassPlane = new GrassPlane(glm::vec3(0.0f, 0.0f, 0.0f));
  this->addObject(grassPlane);
  grassPlane->setScale(glm::vec3(10000.0f, 10000.0f, 10000.0f));
  grassPlane->createPhysicalBody(PhysicalShapeType::CUBE, 1.0f, 1.0f);
  grassPlane->isIntersectable = true;
  // throw std::runtime_error("ExampleScene: GrassPlane is not compatible with the current physics engine. Please use a different ground object.");

  glm::vec3 spacing = glm::vec3(1.0f, 1.0f, 0.0f);
  glm::vec3 startPosition = glm::vec3(-9.0f, 1.0f, -9.0f);

  std::shared_ptr<Material> boxMaterial2 = std::make_shared<Material>(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
  std::shared_ptr<Material> boxMaterial3 = std::make_shared<Material>(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
  std::shared_ptr<Material> boxMaterial4 = std::make_shared<Material>(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
  std::shared_ptr<Material> boxMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/container.jpg", glm::vec3(0.2f, 0.2f, 0.2f), 5.0f);
  std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>("/home/arthur/Documents/simpleGame/obj/square.obj");
  ExampleObject* baseBox = new ExampleObject(boxMesh, Program::getDefaultShader(), {boxMaterial});

  std::shared_ptr<Program> shader = std::make_shared<Program>("/home/arthur/Documents/simpleGame/shaders/instanced.vs", "/home/arthur/Documents/simpleGame/shaders/instanced.fs");

  std::vector<GameObject*> boxes;

  for (uint8_t i = 0; i < 60; i++)
  {
    for (uint8_t j = 0; j < 60; j++)
    {
      ExampleObject* box = new ExampleObject(baseBox->getMesh(), shader, baseBox->getMaterials(), GameObjectType::DYNAMIC);
      box->setPosition(startPosition + spacing * glm::vec3(i, j, 0.0f));
      box->isIntersectable = true;
      this->addObject(box);
      box->createPhysicalBody(PhysicalShapeType::CUBE, 0.1f, 0.0f);
      boxes.push_back(box);
    }
  }

  InstanceGroup* group = new InstanceGroup(boxMesh, true, shader, boxes);
  this->addInstanceGroup(group);

  BaseMover* mover = new BaseMover(baseBox->getMesh(), Program::getDefaultShader(), {Material::getDefaultMaterial()}, GameObjectType::DYNAMIC);
  mover->setPosition(glm::vec3(0.0f, 50.0f, 0.0f));
  mover->player = cam;
  this->addObject(mover);

  ExampleObject* obstacle = new ExampleObject(boxMesh, Program::getDefaultShader(), {boxMaterial2});
  obstacle->setPosition(glm::vec3(0.0f, 5.0f, 10.0f));
  obstacle->setScale(glm::vec3(20.0f, 10.0f, 1.0f));
  this->addObject(obstacle);
  obstacle->createPhysicalBody(PhysicalShapeType::CUBE, 1.0f, 1.0f);

  delete baseBox;

  DirectionalLight* light = new DirectionalLight(glm::vec3(3.0f, 3.0f, 2.0f), glm::vec3(1.0, 1.0, 1.0), 1.5f);
  ExamplePointLight* pointLight = new ExamplePointLight(glm::vec3(-10.0f, 15.0f, -15.0f), glm::vec3(0.8, 1.0, 1.0), 20.0f);

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

  FPSCamera* secondaryCam = new FPSCamera(glm::vec3(0.0f, 0.0f, 0.0f));
  this->addCamera(secondaryCam);

  this->mainCam = cam;
  this->secondaryCam = secondaryCam;

  ExampleObject* house = new ExampleObject("/home/arthur/Documents/simpleGame/obj/casa.obj", Program::getDefaultShader(), GameObjectType::STATIC);
  this->addObject(house);
  house->setPosition(glm::vec3(0.0f, 2.5f, 25.0f));
  house->createPhysicalBody(PhysicalShapeType::CUBE, 1.0f, 1.0f);
}

void ExampleScene::beforeUpdate()
{
  Scene::beforeUpdate();

  if (glfwGetKey(WINDOW.window, GLFW_KEY_C) == GLFW_PRESS && !this->inToggle)
  {
    this->inToggle = true;

    this->setActiveCam(this->getActiveCamera() == this->mainCam ? this->secondaryCam : this->mainCam);
  }
  else if (glfwGetKey(WINDOW.window, GLFW_KEY_C) == GLFW_RELEASE)
  {
    this->inToggle = false;
  }

  if (glfwGetKey(WINDOW.window, GLFW_KEY_R) == GLFW_PRESS)
  {
    this->restartScene();
  }
}

void ExampleScene::restartScene()
{
  this->~ExampleScene();
  ::new (static_cast<void*>(this)) ExampleScene();
}