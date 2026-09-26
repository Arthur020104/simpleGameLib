#include <blackHole.h>

std::shared_ptr<Program> BlackHole::blackHoleShader = std::make_shared<Program>("../shaders/vertex.vs", "../shaders/blackHole.fs");
std::shared_ptr<Mesh> BlackHole::blackHoleMesh = std::make_shared<Mesh>("/home/arthur/Documents/simpleGame/obj/source/sphere.obj");

float BlackHole::baseRadius = (BlackHole::blackHoleMesh->boundingVolume[1].x - BlackHole::blackHoleMesh->boundingVolume[0].x) * 0.5f;
float BlackHole::minDistanceToDestroy = 4.0f;
float BlackHole::linearFallOff = 0.5f;
float BlackHole::quadraticFallOff = 0.005f;
float BlackHole::constantFallOff = 0.8f;

BlackHole::BlackHole(glm::vec3 position, glm::vec3 direction, float scale, float scaleSpeed, float rotationSpeed, float pullForce, float moveSpeed): 
GameObject(BlackHole::blackHoleMesh, BlackHole::blackHoleShader, {Material::getDefaultMaterial()}, GameObjectType::STATIC), scale(scale), scaleSpeed(scaleSpeed), rotationSpeed(rotationSpeed), direction(direction), pullForce(pullForce), moveSpeed(moveSpeed)
{
  this->setPosition(position);
  this->setScale(glm::vec3(scale, scale, scale));
  this->isIntersectable = false;
  this->pullForce = pullForce;
  
  this->radius = BlackHole::baseRadius * scale;
}

void BlackHole::afterUpdate()
{
  const float deltaTime = WINDOW.deltaTime;

  this->setPosition(this->getPosition() + this->direction * this->moveSpeed * deltaTime);
  this->setRotation(glm::vec3(0.0f, this->rotationSpeed * deltaTime, 0.0f) + this->getRotation());
  
  for(GameObject* obj: this->scene->getGameObjects())
  {
    if(obj == this || !obj->hasRigidBody() || obj->getGameObjectType() != GameObjectType::DYNAMIC)
      continue;
    
    glm::vec3 distanceVec = this->getPosition() - obj->getPosition();
    float distance = glm::length(distanceVec);

    if(distance < this->radius)
    {
      this->scene->destroy(obj);
      this->setScale(this->getScale() + glm::vec3(this->scaleSpeed, this->scaleSpeed, this->scaleSpeed) * 0.03f);
      this->pullForce += this->scaleSpeed * 1500.0f;
      continue;
    }

    float attenuation = 1.0f / (BlackHole::constantFallOff + BlackHole::linearFallOff * distance + BlackHole::quadraticFallOff * (distance * distance));

    glm::vec3 holeDirection = glm::normalize(distanceVec);

    glm::vec3 forceVec = holeDirection * this->pullForce * (float)WINDOW.deltaTime * attenuation;

    b3Vec3 force = (b3Vec3){ forceVec.x, forceVec.y, forceVec.z };
    b3Body_ApplyForceToCenter(obj->getBodyId(), force, true);
  }
}

void BlackHole::setScale(glm::vec3 scale)
{
  if(scale.x != scale.y || scale.x != scale.z)
    throw std::invalid_argument("BlackHole: Scale must be uniform in all axes.");

  this->radius = BlackHole::baseRadius * scale.x;
  this->scale = scale.x;

  GameObject::setScale(scale);
}

void BlackHole::setScale(float scale)
{
  this->scale = scale;
  this->radius = BlackHole::baseRadius * scale;

  GameObject::setScale(glm::vec3(scale, scale, scale));
}