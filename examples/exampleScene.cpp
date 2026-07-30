#include <exampleScene.h>
#include <exampleObject.h>
#include <../mesh.h>
#include <../utils.h>
#include <../program.h>
#include <exampleCamera.h>
#include <utils.h>

ExampleScene::ExampleScene(): Scene()
{
  Program* shaderProgram = new Program("/home/arthur/Documents/simpleGame/shaders/vertex.vs", "/home/arthur/Documents/simpleGame/shaders/frag.fs");

  Mesh* teapotMesh = loadMesh("/home/arthur/Documents/simpleGame/obj/teapot.obj");

  cy::Vec3f startPos(-50.0f, -50.0f, -100.0f);
  float moveAmount = 20;

  for(uint16_t i = 0; i < 10; i++)
  {
    for(uint16_t j = 0; j < 10; j++)
    {
      ExampleObject* teapotObj = new ExampleObject(teapotMesh, shaderProgram);
      teapotObj->position = startPos + cy::Vec3f(moveAmount * i, moveAmount*j, 0.0f);
      this->addObject(teapotObj);
    }
  }
 
  ExampleCamera* cam = new ExampleCamera(cy::Vec3f(-0.0f, 0.0f, 0.0f));

  this->addCamera(cam);
  this->setActiveCam(cam);
}