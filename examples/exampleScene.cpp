#include <exampleScene.h>
#include <exampleObject.h>
#include <examplePointLight.h>
#include <../mesh.h>
#include <../utils.h>
#include <../program.h>
#include <exampleCamera.h>
#include <utils.h>
#include <../material.h>

ExampleScene::ExampleScene(): Scene()
{
  std::shared_ptr<Material> roughMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/awesomeface.png", 0.1f, 2.0f);
  std::shared_ptr<Material> shinyMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/container.jpg", 1.0f, 32.0f);
  std::shared_ptr<Material> goldMaterial = std::make_shared<Material>(cy::Vec3f(1.0f, 0.843f, 0.0f), 1.0f, 256.0f);

  std::shared_ptr<Mesh> teapotMesh = loadMesh("/home/arthur/Documents/simpleGame/obj/teapot.obj");

  cy::Vec3f startPos(0.0f, 2.0f, -50.0f);
  float moveAmount = 20;

  for(uint16_t i = 0; i < 5; i++)
  {
    for(uint16_t j = 0; j < 5; j++)
    {
      std::shared_ptr<Material> material;
      if((i + j) % 3 == 0)
        material = roughMaterial;
      else if((i + j) % 3 == 1)
        material = shinyMaterial;
      else
        material = goldMaterial;

      ExampleObject* teapotObj = new ExampleObject(teapotMesh, DEFAULT_SHADER, material);
      teapotObj->setPosition(startPos + cy::Vec3f(moveAmount * i, moveAmount*j, 0.0f));
      teapotObj->addMaterial(DEFAULT_MATERIAL, 0, teapotObj->getMaterialIndicesSize() / 2);
      //teapotObj->setRotation(cy::Quatf(cy::Deg2Rad<float>() * -45.0f, cy::Vec3f(1.0f, 0.0f, 0.0f)));
      this->addObject(teapotObj);
    }
  }
 
  ExampleCamera* cam = new ExampleCamera(cy::Vec3f(-0.0f, 0.0f, 0.0f));

  DirectionalLight* light = new DirectionalLight(cy::Vec3f(0.0f, 10.0f, 10.0f), cy::Vec3f(1.0, 1.0, 1.0), 0.4f);

  ExamplePointLight* pointLight = new ExamplePointLight(cy::Vec3f(0.0f, 0.0f, -30.0f), cy::Vec3f(1.0, 1.0, 1.0), 5.0f);

  this->addLight(pointLight);
  this->addLight(light);
  this->addCamera(cam);
  this->setActiveCam(cam);
}