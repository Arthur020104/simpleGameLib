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

    b3Vec3 origin = (b3Vec3){r.origin.x, r.origin.y, r.origin.z};
    b3Vec3 direction = (b3Vec3){r.direction.x * r.maxDistance, r.direction.y * r.maxDistance, r.direction.z * r.maxDistance};

    b3RayResult result = b3World_CastRayClosest(this->scene->getWorldId(), origin, direction, b3DefaultQueryFilter());

    if(result.hit)
    {
      b3ExplosionDef explosion = b3DefaultExplosionDef();
      explosion.position = result.point;
      explosion.radius = 3.0f;
      explosion.falloff = 2.0f;
      explosion.impulsePerArea = 50.0f;

      b3World_Explode(this->scene->getWorldId(), &explosion);
    }

    // Vertex origin = {.pos = r.origin, .normal = glm::vec3(0.0f, 0.0f, 0.0f), .uv = glm::vec2(0.0f, 0.0f)};
    // Vertex hitPoint = {.pos = r.origin + (r.direction * 1000.0f), .normal = glm::vec3(0.0f, 0.0f, 0.0f), .uv = glm::vec2(0.0f, 0.0f)};
          
    // std::vector<Vertex> vertices{origin, hitPoint};

    // std::shared_ptr<Mesh> lineMesh = std::make_shared<Mesh>(vertices, MeshType::LINE_MESH);

    // lineRay = new ExampleObject(lineMesh, Program::getDefaultShader());
    // this->scene->addObject(lineRay);
  }

  return;
}

void ExampleCamera::beforeUpdate()
{
  FPSCamera::beforeUpdate();
  return;
}