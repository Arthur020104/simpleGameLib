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
  std::shared_ptr<Material> roughMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/container.jpg", 0.1f, 1.0f);
  std::shared_ptr<Material> shinyMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/awesomeface.png", 1.0f, 32.0f);
  std::shared_ptr<Material> goldMaterial = std::make_shared<Material>(cy::Vec3f(1.0f, 0.843f, 0.0f), 9.0f, 256.0f);

  std::shared_ptr<Mesh> teapotMesh = loadMesh("/home/arthur/Documents/simpleGame/obj/teapot.obj");

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

  DirectionalLight* light = new DirectionalLight(cy::Vec3f(0.0f, 3.0f, 1.0f), cy::Vec3f(1.0, 0.8588, 0.7333), 0.6f);

  ExamplePointLight* pointLight = new ExamplePointLight(cy::Vec3f(-5.0f, 6.0f, -7.0f), cy::Vec3f(1.0, 1.0, 1.0), 100.0f);

  std::shared_ptr<Material> boxMaterial = std::make_shared<Material>("/home/arthur/Documents/simpleGame/obj/container2.png", 4.0f, 32.0f);
  boxMaterial->addSpecularTexture("/home/arthur/Documents/simpleGame/obj/container2_specular.png");

  std::shared_ptr<Mesh> boxMesh = loadMesh("/home/arthur/Documents/simpleGame/obj/square.obj");

  ExampleObject* specularTest = new ExampleObject(boxMesh, DEFAULT_SHADER, boxMaterial);
  specularTest->setPosition(cy::Vec3f(0.0f, 0.0f, -10.0f));
  specularTest->setScale(cy::Vec3f(3.0f, 3.0f, 3.0f));

  this->addObject(specularTest);
  this->addLight(pointLight);
  this->addLight(light);
  this->addCamera(cam);
  this->setActiveCam(cam);

  this->addCubeMap({
    "/home/arthur/Documents/simpleGame/obj/cubemap/cubemap_posx.png",
    "/home/arthur/Documents/simpleGame/obj/cubemap/cubemap_negx.png",
    "/home/arthur/Documents/simpleGame/obj/cubemap/cubemap_posy.png",
    "/home/arthur/Documents/simpleGame/obj/cubemap/cubemap_negy.png",
    "/home/arthur/Documents/simpleGame/obj/cubemap/cubemap_posz.png",
    "/home/arthur/Documents/simpleGame/obj/cubemap/cubemap_negz.png"
  });
}