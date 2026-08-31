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

void ExampleCamera::fixedUpdate()
{
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
        b3BodyId bodyId = hit.hitObject->getBodyId();
        b3Vec3 velocity = b3Body_GetLinearVelocity(bodyId);

        r.direction = glm::normalize(r.direction) * 10.0f;
        b3Body_SetLinearVelocity(bodyId, (b3Vec3){r.direction.x + velocity.x, r.direction.y + velocity.y, r.direction.z + velocity.z});
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

void ExampleCamera::beforeUpdate()
{
  FPSCamera::beforeUpdate();
  return;
}