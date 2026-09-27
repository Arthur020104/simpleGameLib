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
#include <grassPlane.h>
#include <movementController.h>

ExampleScene::ExampleScene(): Scene()
{
  this->setGravity(glm::vec3(0.0f, -12.0f, 0.0f));
  std::shared_ptr<Material> planeMaterial = std::make_shared<Material>(glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(1.0f, 1.0f, 1.0f), 512.0f);

  ExampleCamera* cam = new ExampleCamera(glm::vec3(-0.0f, 0.0f, 0.0f));

  GrassPlane* grassPlane = new GrassPlane(glm::vec3(0.0f, 0.0f, 0.0f));
  this->addObject(grassPlane);
  grassPlane->setScale(glm::vec3(500.0f, 500.0f, 500.0f));
  grassPlane->createPhysicalBody(PhysicalShapeType::CUBE, 1.0f, 1.0f);
  grassPlane->isIntersectable = true;

  glm::vec3 spacing = glm::vec3(1.0f, 1.0f, 0.0f);
  glm::vec3 startPosition = glm::vec3(-9.0f, 1.0f, -9.0f);

  std::shared_ptr<Material> boxMaterial = std::make_shared<Material>("../obj/container2.png", glm::vec3(1.0f, 1.0f, 1.0f), 512.0f);
  boxMaterial->addSpecularTexture("../obj/container2_specular.png");
  std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>("../obj/square.obj");
  ExampleObject* baseBox = new ExampleObject(boxMesh, Program::getDefaultShader(), {boxMaterial});

  std::shared_ptr<Program> shader = std::make_shared<Program>("../shaders/instanced.vs", "../shaders/instanced.fs");

  std::vector<GameObject*> boxes;

  for (uint8_t i = 0; i < 50; i++)
  {
    for (uint8_t j = 0; j < 50; j++)
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

  ExampleObject* whiteBox = new ExampleObject(baseBox->getMesh(), Program::getDefaultShader(), {boxMaterial}, GameObjectType::DYNAMIC);
  whiteBox->setPosition(glm::vec3(0.0f, 50.0f, 0.0f));
  this->addObject(whiteBox);
  whiteBox->createPhysicalBody(PhysicalShapeType::CUBE, 1.0f, 0.3f);

  //marble texture from https://www.vecteezy.com/photo/13425660-white-gray-black-marble-pattern-square-background
  std::shared_ptr<Material> marbleMaterial = std::make_shared<Material>("../obj/marbleTexture.jpg", glm::vec3(0.7f, 0.7f, 0.7f), 512.0f);
  ExampleObject* obstacle = new ExampleObject(boxMesh, Program::getDefaultShader(), {marbleMaterial});
  obstacle->setPosition(glm::vec3(0.0f, 5.0f, 10.0f));
  obstacle->setScale(glm::vec3(20.0f, 10.0f, 1.0f));
  this->addObject(obstacle);
  obstacle->createPhysicalBody(PhysicalShapeType::CUBE, 1.0f, 1.0f);

  delete baseBox;

  DirectionalLight* light = new DirectionalLight(glm::vec3(3.0f, 3.0f, 2.0f), glm::vec3(1.0, 1.0, 1.0), 1.0f);
  ExamplePointLight* pointLight = new ExamplePointLight(glm::vec3(-10.0f, 15.0f, 5.0f), glm::vec3(0.7, 0.7, 1.0), 15.0f);

  this->addLight(pointLight);
  this->addLight(light);
  this->addCamera(cam);
  this->setActiveCam(cam);

  this->addCubeMap({
    "../obj/Cubemaps_2025-07-25/20250717_210302_0772_rt.png",
    "../obj/Cubemaps_2025-07-25/20250717_210302_0772_lf.png",
    "../obj/Cubemaps_2025-07-25/20250717_210302_0772_up.png",
    "../obj/Cubemaps_2025-07-25/20250717_210302_0772_dn.png",
    "../obj/Cubemaps_2025-07-25/20250717_210302_0772_bk.png",
    "../obj/Cubemaps_2025-07-25/20250717_210302_0772_ft.png"
  });

  UIItem* crosshair = new UIItem("../obj/crosshair.png");
  crosshair->setScale(glm::vec3(0.005f, 0.005f, 0.1f));

  ExampleUIItem* testItem = new ExampleUIItem("../obj/container.jpg");
  testItem->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
  testItem->setPosition(glm::vec3(-0.9f, 0.9f, 0.0f));

  this->addUIItem(crosshair);
  this->addUIItem(testItem);

  FPSCamera* secondaryCam = new FPSCamera(glm::vec3(0.0f, 0.0f, 0.0f));
  this->addCamera(secondaryCam);

  this->mainCam = cam;
  this->secondaryCam = secondaryCam;

  ExampleObject* house = new ExampleObject("../obj/casa.obj", Program::getDefaultShader(), GameObjectType::STATIC);
  this->addObject(house);
  house->setPosition(glm::vec3(0.0f, 2.5f, 35.0f));
  house->setScale(glm::vec3(3.0f, 3.0f, 3.0f));
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