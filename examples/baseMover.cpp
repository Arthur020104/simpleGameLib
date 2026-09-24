#include <float.h>
#include <vector>
#include <iostream>
#include <baseMover.h>

void BaseMover::start()
{
  if(this->getGameObjectType() != GameObjectType::DYNAMIC)
    throw std::runtime_error("BaseMover::start() - BaseMover can only be used with DYNAMIC GameObjectType.");

  this->createPhysicalBody(PhysicalShapeType::CUBE, 1.0f, 0.3f);
}

void BaseMover::beforeUpdate()
{
  if(this->player == nullptr)
    return;



  // glm::vec3 playerDir = this->player->getPosition() - this->getPosition();
  // playerDir.y = std::clamp(playerDir.y, -0.8f, 0.8f);
  // playerDir = glm::normalize(playerDir);
  // glm::vec3 forceVec = playerDir * this->acceleration * (float)WINDOW.deltaTime;

  // // std::cout<<"BaseMover::beforeUpdate() - Force: "<<forceVec.x<<", "<<forceVec.y<<", "<<forceVec.z<<std::endl;

  // b3Vec3 force = (b3Vec3){ forceVec.x, forceVec.y, forceVec.z };
  // b3Body_ApplyForceToCenter(this->getBodyId(), force, true);

  // // this->setRotation(glm::vec3(0.0f, this->scene->getActiveCamera()->getRotation().y, 0.0f));
}