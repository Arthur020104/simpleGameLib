#include <grassGroup.h>

float GrassGroup::maxDistanceToUseHighQualityGrass = 100.0f;
float GrassGroup::stopRenderingDistance = 200.0f;
float GrassGroup::removeFromGPUDistance = 260.0f;

GrassGroup::GrassGroup(const std::vector<GameObject*>& objs): InstanceGroup(GrassGroup::getLods()[0], false, getGrassShader(), objs, false)
{
  this->baseY = 0.0f;
  this->windDirection = glm::vec3(1.0f, 0.0f, 0.0f);
  this->windStrength = 1.0f;
  this->grassStemSizeY = 0.5f;
  
  this->groupLods[0] = std::make_shared<Mesh>(*GrassGroup::getLods()[0]);
  this->groupLods[1] = std::make_shared<Mesh>(*GrassGroup::getLods()[1]);
}

void GrassGroup::draw(Camera* cam, Scene* scene)
{
  this->update();

  if(!this->canDraw)
    return;

  const uint16_t shaderID = this->program->getProgram();
  glUseProgram(shaderID);
  this->program->bindFloat("baseY", this->baseY);
  this->program->bindVec3("wind", this->windDirection * this->windStrength);
  this->program->bindFloat("grassStemSizeY", this->grassStemSizeY);
  this->program->bindFloat("time", (float)glfwGetTime());
  InstanceGroup::draw(cam, scene);
}

void GrassGroup::update()
{
  this->canDraw = true;

  glm::vec3 cameraViewDirection = this->scene->getActiveCamera()->getForwardVector();
  glm::vec3 cameraPosition = this->scene->getActiveCamera()->getPosition();
  float fov = this->scene->getActiveCamera()->getFOV();

  const float distanceToCamera = glm::length(this->center - cameraPosition);
  Mesh* currentMesh = this->mesh.get();

  bool ocluded = distanceToCamera > 50.0f && glm::dot(cameraViewDirection, glm::normalize(this->center - cameraPosition)) < std::cos(glm::radians(fov + 10.0f));

  if(!ocluded && distanceToCamera < GrassGroup::maxDistanceToUseHighQualityGrass)
  {
    this->mesh = this->groupLods[0];

    if(currentMesh != this->mesh.get() || !this->hasVbo)
      this->passDataToGPU();
  }
  else if(!ocluded && distanceToCamera < GrassGroup::stopRenderingDistance)
  {
    this->mesh = this->groupLods[1];

    if(currentMesh != this->mesh.get() || !this->hasVbo)
      this->passDataToGPU();
  }
  else if(distanceToCamera > GrassGroup::removeFromGPUDistance)
  {
    this->canDraw = false;
    this->removeDataFromGPU();
  }
  else
    this->canDraw = false;
}

std::shared_ptr<Mesh> GrassGroup::lods[2] = {nullptr, nullptr};

std::shared_ptr<Mesh>* GrassGroup::getLods()
{
  if(GrassGroup::lods[0] != nullptr)
    return GrassGroup::lods;
  
  GrassGroup::lods[0] = std::make_shared<Mesh>("../obj/single_grass_blade/single_grass_blade.obj");
  GrassGroup::lods[1] = std::make_shared<Mesh>("../obj/single_grass_blade/simpleGrass.obj");

  return GrassGroup::lods;
}

std::shared_ptr<Program> GrassGroup::grassShader = nullptr;

std::shared_ptr<Program> GrassGroup::getGrassShader()
{
  if(GrassGroup::grassShader != nullptr)
    return GrassGroup::grassShader;
  
  GrassGroup::grassShader = std::make_shared<Program>("../shaders/grassInstanced.vs", "../shaders/grassInstanced.fs");
  return GrassGroup::grassShader;
}