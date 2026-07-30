#include <exampleScene.h>
#include <exampleObject.h>
#include <../mesh.h>
#include <../utils.h>
#include <../program.h>
#include <utils.h>

ExampleScene::ExampleScene(): Scene()
{
  Program* shaderProgram = new Program("/home/arthur/Documents/simpleGame/shaders/vertex.vs", "/home/arthur/Documents/simpleGame/shaders/frag.fs");

  Mesh* teapotMesh = loadMesh("/home/arthur/Documents/simpleGame/obj/teapot.obj");

  ExampleObject* teapotObj = new ExampleObject(teapotMesh, shaderProgram);
  teapotObj->position = cy::Vec3f(0.0f, 20.0f, -20.0f);
 
   Camera* cam = new Camera(cy::Vec3f(0.0f, 5.0f, 30.0f));

   this->addCamera(cam);
  this->setActiveCam(1);
  this->addObject(teapotObj);
}