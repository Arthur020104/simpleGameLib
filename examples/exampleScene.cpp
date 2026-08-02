#include <exampleScene.h>
#include <exampleObject.h>
#include <../mesh.h>
#include <../utils.h>
#include <../program.h>
#include <exampleCamera.h>
#include <utils.h>

ExampleScene::ExampleScene(): Scene()
{
  std::shared_ptr<Mesh> teapotMesh = loadMesh("/home/arthur/Documents/simpleGame/obj/teapot.obj");

  cy::Vec3f startPos(-500.0f, -300.0f, -500.0f);
  float moveAmount = 20;

  for(uint16_t i = 0; i < 50; i++)
  {
    for(uint16_t j = 0; j < 50; j++)
    {
      ExampleObject* teapotObj = new ExampleObject(teapotMesh, DEFAULT_SHADER);
      teapotObj->position = startPos + cy::Vec3f(moveAmount * i, moveAmount*j, 0.0f);
      this->addObject(teapotObj);
    }
  }
 
  ExampleCamera* cam = new ExampleCamera(cy::Vec3f(-0.0f, 0.0f, 0.0f));

  this->addCamera(cam);
  this->setActiveCam(cam);
}