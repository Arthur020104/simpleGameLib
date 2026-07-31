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

  if(lineRay != nullptr && glfwGetMouseButton(WINDOW.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    this->scene->destroy(lineRay);
    delete lineRay;
    this->lineRay = nullptr;
  }
  //if click fire mouse
  if(glfwGetMouseButton(WINDOW.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    cy::Vec2f mousePos = WINDOW.getMousePos(true);
    Ray r = generateRay(mousePos);

    bool hit = this->scene->intersectSceneObjects(r);
    if(hit)
    {
      for(Hit& hit: r.hits)
      {
        this->scene->destroy(hit.hitObject);
        delete hit.hitObject;
        //hit.hitObject->isSelected = true;

      }
      
      Vertex origin = {.pos = r.origin, .normal = cy::Vec3f(0.0f, 0.0f, 0.0f), .uv = cy::Vec2f(0.0f, 0.0f)};
      Vertex hitPoint = {.pos = r.origin + (r.direction * 1000.0f), .normal = cy::Vec3f(0.0f, 0.0f, 0.0f), .uv = cy::Vec2f(0.0f, 0.0f)};

      std::vector<Vertex> vertices{origin, hitPoint};

      Mesh* lineMesh = new Mesh(vertices);

      lineRay = new ExampleObject(lineMesh, DEFAULT_SHADER);
      lineRay->isSelected = true;
      this->scene->addObject(lineRay);
    }
  }

  return;
}