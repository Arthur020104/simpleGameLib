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
  std::shared_ptr<Material> roughMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/container.jpg", cy::Vec3f(0.1f, 0.1f, 0.1f), 1.0f);
  std::shared_ptr<Material> shinyMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/awesomeface.png", cy::Vec3f(1.0f, 1.0f, 1.0f), 32.0f);
  std::shared_ptr<Material> goldMaterial = std::make_shared<Material>(cy::Vec3f(1.0f, 0.843f, 0.0f), cy::Vec3f(1.0f, 0.843f, 0.0f), 256.0f);

  std::shared_ptr<Mesh> teapotMesh = std::make_shared<Mesh>("/home/arthur/Documents/simpleGame/obj/teapot.obj");

  cy::Vec3f startPos(0.0f, 2.0f, -50.0f);
  float moveAmount = 20;

  for(uint16_t i = 0; i < 10; i++)
  {
    for(uint16_t j = 0; j < 10; j++)
    {
      std::shared_ptr<Material> material;
      if((i + j) % 3 == 0)
        material = roughMaterial;
      else if((i + j) % 3 == 1)
        material = shinyMaterial;
      else
        material = goldMaterial;

      ExampleObject* teapotObj = new ExampleObject(teapotMesh, DEFAULT_SHADER);
      teapotObj->setPosition(startPos + cy::Vec3f(moveAmount * i, moveAmount*j, 0.0f));
      teapotObj->addMaterial(material, 0, teapotObj->getMaterialIndicesSize() / 2);
      teapotObj->setScale(cy::Vec3f(0.5f, 0.5f, 0.5f));
      teapotObj->setRotation(cy::Vec3f(-90.0f, 0.0f, 0.0f));
      this->addObject(teapotObj);
    }
  }
 
  ExampleCamera* cam = new ExampleCamera(cy::Vec3f(-0.0f, 0.0f, 0.0f));

  DirectionalLight* light = new DirectionalLight(cy::Vec3f(0.0f, 3.0f, 2.0f), cy::Vec3f(1.0, 1.0, 1.0), 1.0f);

  ExamplePointLight* pointLight = new ExamplePointLight(cy::Vec3f(-10.0f, 15.0f, -15.0f), cy::Vec3f(1.0, 0.5, 0.8), 5.0f);

  std::shared_ptr<Material> boxMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/OldChildrensToysObj/textures/2k/T_Toys_BaseColor.tga", cy::Vec3f(1.0f, 1.0f, 1.0f), 32.0f);
  //boxMaterial->addSpecularTexture("/home/arthur/Documents/simpleGame/obj/container2_specular.png");

  std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>("/home/arthur/Documents/simpleGame/obj/OldChildrensToysObj/meshes/ChildrensToys.obj");

  ExampleObject* specularTest = new ExampleObject(boxMesh, DEFAULT_SHADER, {boxMaterial});

  specularTest->setPosition(cy::Vec3f(0.0f, 0.0f, -10.0f));
  specularTest->setRotation(cy::Vec3f(-90.0f, 0.0f, 90.0f));
  specularTest->setScale(cy::Vec3f(20.0f, 20.0f, 20.0f));

  ExampleObject* test = new ExampleObject("/home/arthur/Documents/simpleGame/obj/source/Cartoon_City_Free.obj", DEFAULT_SHADER);
  test->setScale(cy::Vec3f(0.5f, 0.5f, 0.5f));

  ExampleObject* quad = new ExampleObject(Mesh::getQuadMesh(), DEFAULT_SHADER);
  quad->setPosition(cy::Vec3f(0.0f, 0.0f, -5.0f));
  quad->setScale(cy::Vec3f(50.0f, 50.0f, 50.0f));

  this->addObject(test);
  this->addObject(specularTest);
  this->addObject(quad);
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
  crosshair->setScale(cy::Vec3f(0.05f, 0.07f, 0.1f));

  ExampleUIItem* testItem = new ExampleUIItem("/home/arthur/Documents/simpleGame/obj/container.jpg");
  testItem->setScale(cy::Vec3f(0.1f, 0.1f, 0.1f));
  testItem->setPosition(cy::Vec3f(-0.9f, 0.9f, 0.0f));

  this->addUIItem(crosshair);
  this->addUIItem(testItem);
}