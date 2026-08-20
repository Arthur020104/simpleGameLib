#include <exampleCamera.h>
#include <window.h>
#include <../ray.h>
#include <../vertex.h>
#include <../mesh.h>

void ExampleCamera::start()
{
  
  return;
}

void ExampleCamera::afterUpdate()
{
  
  return;
}

void ExampleCamera::beforeUpdate()
{
  FPSCamera::beforeUpdate();
  if(lineRay != nullptr && glfwGetMouseButton(WINDOW.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    this->scene->destroy(lineRay);
    //delete lineRay;
    this->lineRay = nullptr;
  }
  
  if(glfwGetMouseButton(WINDOW.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    cy::Vec2f mousePos = cy::Vec2f(0.0f, 0.0f);
    Ray r = generateRay(mousePos);

    bool hit = this->scene->intersectSceneObjects(r);
    if(hit)
    {
      for(Hit& hit: r.hits)
      {
        this->scene->getTimeQueue()->addToQueue([this, hit]() { this->scene->destroy(hit.hitObject); }, 5.0f);
      }
    }

    Vertex origin = {.pos = r.origin, .normal = cy::Vec3f(0.0f, 0.0f, 0.0f), .uv = cy::Vec2f(0.0f, 0.0f)};
    Vertex hitPoint = {.pos = r.origin + (r.direction * 1000.0f), .normal = cy::Vec3f(0.0f, 0.0f, 0.0f), .uv = cy::Vec2f(0.0f, 0.0f)};
          
    std::vector<Vertex> vertices{origin, hitPoint};

    std::shared_ptr<Mesh> lineMesh = std::make_shared<Mesh>(vertices, MeshType::LINE_MESH);

    lineRay = new ExampleObject(lineMesh, DEFAULT_SHADER);
    this->scene->addObject(lineRay);
  }

  return;
}