#include <exampleCamera.h>
#include <window.h>
#include <../ray.h>
#include <../vertex.h>
#include <../mesh.h>
#include <../program.h>

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
    glm::vec2 mousePos = glm::vec2(0.0f, 0.0f);
    Ray r = generateRay(mousePos);

    bool hit = this->scene->intersectSceneObjects(r);
    if(hit)
    {
      for(Hit& hit: r.hits)
      {
        this->scene->getTimeQueue()->addToQueue([this, hit]() { this->scene->destroy(hit.hitObject); }, 5.0f);
      }
    }

    Vertex origin = {.pos = r.origin, .normal = glm::vec3(0.0f, 0.0f, 0.0f), .uv = glm::vec2(0.0f, 0.0f)};
    Vertex hitPoint = {.pos = r.origin + (r.direction * 1000.0f), .normal = glm::vec3(0.0f, 0.0f, 0.0f), .uv = glm::vec2(0.0f, 0.0f)};
          
    std::vector<Vertex> vertices{origin, hitPoint};

    std::shared_ptr<Mesh> lineMesh = std::make_shared<Mesh>(vertices, MeshType::LINE_MESH);

    lineRay = new ExampleObject(lineMesh, Program::getDefaultShader());
    this->scene->addObject(lineRay);
  }

  return;
}