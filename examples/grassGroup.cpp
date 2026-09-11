#include <grassGroup.h>
#include <malloc.h>

float GrassGroup::maxDistanceToUseHighQualityGrass = 175.0f;
float GrassGroup::stopRenderingDistance = 400.0f;
float GrassGroup::removeFromGPUDistance = 500.0f;
float GrassGroup::removeFromRAMDistance = 550.0f;
float GrassGroup::loadBackToGPUDistance = 450.0f;
float GrassGroup::minDistanceForOclusion = 100.0f;

uint32_t GrassGroup::maxMemoryUntilFreeRAM = 1024 * 1024 * 1024;
uint32_t GrassGroup::freeMemory = 0;


float GrassGroup::getRandomFloatR(float min, float max) 
{
  std::uniform_real_distribution<float> dis(min, max);
  return dis(this->gen);
}

GrassGroup::GrassGroup(const std::vector<GameObject*>& objs): InstanceGroup(GrassGroup::getLods()[0], false, getGrassShader(), objs, false)
{
  this->seed = this->rd();
  this->gen.seed(this->seed);

  this->baseY = 0.0f;
  this->windDirection = glm::vec3(1.0f, 0.0f, 0.0f);
  this->windStrength = 1.0f;
  this->grassStemSizeY = 0.5f;
}

GrassGroup::GrassGroup(std::shared_ptr<Program> shader, std::shared_ptr<Material> material, glm::vec3 basePosition, uint16_t grassAmountX, uint16_t grassAmountZ, float minXDistance, float minZDistance): InstanceGroup(GrassGroup::getLods()[0], false, shader)
{
  this->seed = this->rd();
  this->gen.seed(this->seed);

  this->material = material;
  this->program = shader;
  this->basePosition = basePosition;
  this->grassAmountX = grassAmountX; this->grassAmountZ = grassAmountZ;
  this->minXDistance = minXDistance; this->minZDistance = minZDistance;

  this->baseY = basePosition.y;
  this->grassStemSizeY = 0.5f;
  this->center = basePosition + glm::vec3(grassAmountX * minXDistance * 0.5f, 0.0f, grassAmountZ * minZDistance * 0.5f);
  this->windDirection = glm::vec3(1.0f, 0.0f, 0.0f);
  this->windStrength = 1.0f;
}

void GrassGroup::loadDataToRAM()
{
  this->groupLods[0] = std::make_shared<Mesh>(*GrassGroup::getLods()[0]);
  this->groupLods[1] = std::make_shared<Mesh>(*GrassGroup::getLods()[1]);
  this->mesh = this->groupLods[0];

  this->hasDataInRAM = true;

  this->gen.seed(this->seed);
  for(uint16_t i = 0; i < grassAmountX; i++)
  {
    for(uint16_t j = 0; j < grassAmountZ; j++)
    {
      GrassInstance* grass = new GrassInstance(this->groupLods[0], this->program, {material}, GameObjectType::STATIC);

      glm::vec3 grassPosition = glm::vec3(basePosition.x, basePosition.y, basePosition.z);

      float xNoise = i != 0 && i != grassAmountX - 1 ? getRandomFloatR(-minXDistance * 0.5f, minXDistance * 0.5f) : 0.0f;
      float zNoise = j != 0 && j != grassAmountZ - 1 ? getRandomFloatR(-minZDistance * 0.5f, minZDistance * 0.5f) : 0.0f;
      
      grassPosition.x += 0.0f + i * minXDistance;
      grassPosition.z += 0.0f + j * minZDistance;
      
      glm::vec3 rotation = glm::vec3(0.0f, getRandomFloatR(0.0f, 180.0f), 0.0f);
      
      grass->setRotation(rotation);
      
      float scale = getRandomFloatR(3.0f, 5.0f);
      glm::vec3 scaleVec = glm::vec3(scale, scale, scale);

      grass->setPosition(grassPosition);

      grass->setScale(scaleVec);

      this->objs.insert(grass);
    }
  }
}

void GrassGroup::cleanRAMData()
{
  if(!this->hasDataInRAM)
    return;

  if(this->hasVbo)
    this->removeDataFromGPU();

  GrassGroup::freeMemory += this->objs.size() * (sizeof(GameObject*) + sizeof(GameObject)) + materialToIndexMap.size() * (sizeof(Material*) + sizeof(uint8_t));

  if(this->lods[0].use_count() == 1)
    GrassGroup::freeMemory += this->lods[0]->getVertices().size() * sizeof(Vertex);
  if(this->lods[1].use_count() == 1)
    GrassGroup::freeMemory += this->lods[1]->getVertices().size() * sizeof(Vertex);
  
  this->hasDataInRAM = false;

  for(GameObject* obj : this->objs)
  {
    delete obj;
  }
  this->objs = {};
  this->materialToIndexMap = {};

  this->groupLods[0] = nullptr;
  this->groupLods[1] = nullptr;
  this->mesh = nullptr;
  this->hasVbo = false;
}

void GrassGroup::draw(Camera* cam)
{
  if(!this->canDraw)
    return;

  const uint16_t shaderID = this->program->getProgram();
  glUseProgram(shaderID);
  this->program->bindFloat("baseY", this->baseY);
  this->program->bindVec3("wind", this->windDirection * this->windStrength);
  this->program->bindFloat("grassStemSizeY", this->grassStemSizeY);
  this->program->bindFloat("time", (float)glfwGetTime());
  InstanceGroup::draw(cam);
}

void GrassGroup::reducedUpdate()
{
  if(GrassGroup::freeMemory > GrassGroup::maxMemoryUntilFreeRAM)
  {
    std::cout<<"Freeing RAM memory. Current free memory: " << GrassGroup::freeMemory << " bytes.\n";
    GrassGroup::freeMemory = 0;
    std::cout<<"Freeing RAM memory. Current free memory: " << GrassGroup::freeMemory << " bytes.\n";
    malloc_trim(0);
  }

  this->canDraw = true;

  glm::vec3 cameraViewDirection = this->scene->getActiveCamera()->getForwardVector();
  glm::vec3 cameraPosition = this->scene->getActiveCamera()->getPosition();
  const float distanceToCamera = glm::length(this->center - cameraPosition);

  if(distanceToCamera > GrassGroup::removeFromGPUDistance && distanceToCamera < GrassGroup::removeFromRAMDistance)
  {
    this->removeDataFromGPU();
    this->canDraw = false;
    return;
  }
  else if(distanceToCamera > GrassGroup::removeFromRAMDistance)
  {
    this->cleanRAMData();
    this->canDraw = false;
    return;
  }

  if(!this->hasVbo && distanceToCamera < GrassGroup::loadBackToGPUDistance)
  {
    this->passDataToGPU();
  }

  float fov = this->scene->getActiveCamera()->getFOV();

  Mesh* currentMesh = this->mesh.get();

  bool ocluded = distanceToCamera > GrassGroup::minDistanceForOclusion && glm::dot(cameraViewDirection, glm::normalize(this->center - cameraPosition)) < std::cos(glm::radians(fov + 10.0f));

  if(ocluded)
  {
    this->canDraw = false;
    return;
  }

  if(distanceToCamera < GrassGroup::maxDistanceToUseHighQualityGrass)
  {
    if(!this->hasDataInRAM)
      this->loadDataToRAM();

    this->mesh = this->groupLods[0];

    if(currentMesh != this->mesh.get() || !this->hasVbo)
      this->passDataToGPU();
    
    return;
  }
  else if(distanceToCamera < GrassGroup::stopRenderingDistance)
  {
    if(!this->hasDataInRAM)
      this->loadDataToRAM();

    this->mesh = this->groupLods[1];

    if(currentMesh != this->mesh.get() || !this->hasVbo)
      this->passDataToGPU();
    
    return;
  }
  else
    this->canDraw = false;
}

void GrassGroup::passDataToGPU()
{
  if(!this->hasDataInRAM)
    this->loadDataToRAM();

  InstanceGroup::passDataToGPU();
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