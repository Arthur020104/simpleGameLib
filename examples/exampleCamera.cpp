#include <exampleCamera.h>
#include <exampleObject.h>
#include <window.h>
#include <../ray.h>
#include <../vertex.h>
#include <../mesh.h>

void ExampleCamera::start()
{
  
  return;
}

void ExampleCamera::beforeUpdate()
{
  
  return;
}

void ExampleCamera::aftherUpdate()
{

  if(glfwGetKey(WINDOW.window, GLFW_KEY_E) == GLFW_PRESS)
  {
    this->position += cy::Vec3f(0.0f, velocity, 0.0f) * WINDOW.deltaTime;
    this->updateModelMatrix();
  }
  if(glfwGetKey(WINDOW.window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    this->position += cy::Vec3f(0.0f, -velocity, 0.0f) * WINDOW.deltaTime;
    this->updateModelMatrix();
  }
  if(glfwGetKey(WINDOW.window, GLFW_KEY_D) == GLFW_PRESS)
  {
    this->position += cy::Vec3f(velocity, 0.0f, 0.0f) * WINDOW.deltaTime;
    this->updateModelMatrix();
  }
  if(glfwGetKey(WINDOW.window, GLFW_KEY_A) == GLFW_PRESS)
  {
    this->position += cy::Vec3f(-velocity, 0.0f, 0.0f) * WINDOW.deltaTime;
    this->updateModelMatrix();
  }

  if(lineRay != nullptr)
  {
    this->scene->destroy(lineRay);
    delete lineRay;
    this->lineRay = nullptr;
  }
  cy::Vec2f mousePos = WINDOW.getMousePos(true);
  Ray r = generateRay(mousePos);

  bool hit = false;//this->scene->intersectSceneObjects(r);

  if(hit)
  {
    
    Vertex origin = {.pos = r.origin, .normal = cy::Vec3f(0.0f, 0.0f, 0.0f), .uv = cy::Vec2f(0.0f, 0.0f)};
    Vertex hitPoint = {.pos = r.hits[0].point, .normal = cy::Vec3f(0.0f, 0.0f, 0.0f), .uv = cy::Vec2f(0.0f, 0.0f)};
    Mesh* lineMesh = new Mesh(std::vector<Vertex>{origin, hitPoint});
    
    r.hits[0].hitObject->isSelected = true;

    lineRay = new ExampleObject(lineMesh, DEFAULT_SHADER);
    lineRay->scale = cy::Vec3f(1.2f, 1.2f, 1.2f);
    lineRay->isSelected = true;
    this->scene->addObject(lineRay);
  }
  
  return;
}