#define GLM_FORCE_SWIZZLE
#include <exampleCamera.h>
#include <window.h>
#include <ray.h>
#include <vertex.h>
#include <mesh.h>
#include <program.h>
#include <blackHole.h>


void ExampleCamera::start()
{
  this->velocity = 20.0f;

  this->body = new MovementController(glm::vec3(0.0f, 50.0f, 0.0f));
  glm::vec3 bounds[2] = {this->body->getMesh()->boundingVolume[0], this->body->getMesh()->boundingVolume[1]};
  bounds[0] = this->body->getModelMatrix() * glm::vec4(bounds[0], 1.0f);
  bounds[1] = this->body->getModelMatrix() * glm::vec4(bounds[1], 1.0f);
  
  this->positionOffset = glm::vec3( 0.0f, bounds[1].y - bounds[0].y, 0.0f);
  this->setPosition(this->body->getPosition() + this->positionOffset);

  this->scene->addObject(this->body);
  return;
}

void ExampleCamera::afterUpdate()
{
}

void ExampleCamera::fixedUpdate()
{
}

void ExampleCamera::beforeUpdate()
{
  if(this->scene->getActiveCamera() != this)
    return;

  FPSCamera::beforeUpdate();

  this->body->setRotation(glm::vec3(0.0f, this->getRotation().y, 0.0f));
  glm::vec3 offset = glm::vec3(this->body->getRotationMatrix() * glm::vec4(this->positionOffset, 1.0f));
  glm::vec3 newPos = this->body->getPosition() + offset;


  Ray r = generateRay(glm::vec2(0.0f, 0.0f));
  glm::vec3 dir = glm::normalize(r.direction);

  b3Pos origin = (b3Pos){newPos.x, newPos.y, newPos.z};
  b3Vec3 translation = (b3Vec3){dir.x * r.maxDistance, dir.y * r.maxDistance, dir.z * r.maxDistance};

  b3QueryFilter filter = b3DefaultQueryFilter();
  //set filter to ignore the body of the camera


  b3RayResult result = b3World_CastRayClosest(this->scene->getWorldId(), origin, translation, filter);


  if(result.hit)
  {
    glm::vec3 hitPoint(result.point.x, result.point.y, result.point.z);
    float dist = glm::distance(hitPoint, newPos);

    if(dist < this->minDistance)
    {
      newPos -= dir * (this->minDistance - dist);
    }
  }

  this->setPosition(newPos);

  if(glfwGetMouseButton(WINDOW.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetKey(WINDOW.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !this->isShooting)
  {
    this->isShooting = true;
    BlackHole* bh = new BlackHole(this->getPosition(), this->getForwardVector(), 5.0f, 0.1f, 500.0f, 50000.0f, 10.0f);
    this->scene->addObject(bh);
   
    // b3ExplosionDef explosion = b3DefaultExplosionDef();
    // explosion.position = result.point;
    // explosion.radius = 3.0f;
    // explosion.falloff = 2.0f;
    // explosion.impulsePerArea = 50.0f;

    // b3World_Explode(this->scene->getWorldId(), &explosion);
  } 
  else if(glfwGetMouseButton(WINDOW.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE || glfwGetKey(WINDOW.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
    this->isShooting = false;
  }

}