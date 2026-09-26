#include <grassPlane.h> 
#include <program.h>
#include <random>

std::shared_ptr<Program> GrassPlane::planeShader = std::make_shared<Program>("../shaders/resizedTexture.vs", "../shaders/frag.fs");

GrassPlane::GrassPlane(glm::vec3 position): GameObject(Mesh::getQuadMesh(), GrassPlane::planeShader)
{
  this->grassMesh = std::make_shared<Mesh>("../obj/single_grass_blade/single_grass_blade.obj");
  this->grassMaterial = std::make_shared<Material>(glm::vec3(0.075f, 0.3f, 0.075f), glm::vec3(0.1f, 0.1f, 0.1f), 1.0f);
  this->setPosition(position);
}

void GrassPlane::start()
{
  std::shared_ptr<Material> planeMaterial = std::make_shared<Material>("../obj/grass.png", glm::vec3(0.1f, 0.1f, 0.1f), 1.0f);
  planeMaterial->diffuseTex->setWrapping(GL_REPEAT, GL_REPEAT);
  this->useOnly(planeMaterial);
  this->setRotation(glm::vec3(270.0f, 0.0f, 0.0f));

  

  std::cout << "GrassPlane: Bounding Volume: min: " << this->getMesh()->boundingVolume[0].x << ", " << this->getMesh()->boundingVolume[0].y << ", " << this->getMesh()->boundingVolume[0].z << std::endl;
  std::cout << "GrassPlane: Bounding Volume: max: " << this->getMesh()->boundingVolume[1].x << ", " << this->getMesh()->boundingVolume[1].y << ", " << this->getMesh()->boundingVolume[1].z << std::endl;

  glm::vec4 min = this->getModelMatrix() * glm::vec4(this->getMesh()->boundingVolume[0], 1.0f);
  glm::vec4 max = this->getModelMatrix() * glm::vec4(this->getMesh()->boundingVolume[1], 1.0f);
  glm::vec4 tempMin = glm::min(min, max);
  max = glm::max(max, min);
  min = tempMin;

  std::cout << "GrassPlane: min: " << min.x << ", " << min.y << ", " << min.z << std::endl;
  std::cout << "GrassPlane: max: " << max.x << ", " << max.y << ", " << max.z << std::endl;

  this->basePosition = glm::vec3(min.x, this->getPosition().y, min.z) - glm::vec3(-0.6f, 0.0f, -0.6f);//offset
  std::cout << "GrassPlane: basePosition: " << this->basePosition.x << ", " << this->basePosition.y << ", " << this->basePosition.z << std::endl;
  
  uint16_t grassAmountX = std::floor((max.x - min.x) / this->minXDistance);
  uint16_t grassAmountZ = std::floor((max.z - min.z) / this->minZDistance);
  std::cout << "GrassPlane: grassAmountX: " << grassAmountX << std::endl;
  std::cout << "GrassPlane: grassAmountZ: " << grassAmountZ << std::endl;

  // throw std::runtime_error("GrassPlane: GrassPlane is not compatible with the current physics engine. Please use a different ground object.");
  uint16_t xChunks = std::ceil((max.x - min.x) / this->chunkSizeX);
  uint16_t zChunks = std::ceil((max.z - min.z) / this->chunkSizeZ);

  float xChunkSize = (max.x - min.x) / (float)xChunks;
  float zChunkSize = (max.z - min.z) / (float)zChunks;

  uint16_t totalChunks = std::ceil(xChunks * zChunks);

  glm::vec3 chunkMaxBound = glm::vec3(max.x / this->chunkSizeX, 0.0f, max.z / this->chunkSizeZ);

  glm::vec3 addingToPosition = glm::vec3(0.0f, 0.0f, 0.0f);

  std::vector<GrassInstance*> grassInstances;
  grassInstances.assign(totalChunks, nullptr);

  uint16_t currentGroup = 0;

  uint16_t grassPerChunkX = std::ceil(grassAmountX / (float)xChunks);
  uint16_t grassPerChunkZ = std::ceil(grassAmountZ / (float)zChunks);

  std::shared_ptr<Program> grassShader = std::make_shared<Program>("../shaders/grassInstanced.vs", "../shaders/grassInstanced.fs");
  for(uint32_t i = 0; i < xChunks; i++)
  {
    glm::vec3 startPosition = glm::vec3(xChunkSize * i, 0.0f, 0.0f);
    for(uint32_t j = 0; j < zChunks; j++)
    {
      startPosition.z = zChunkSize * j;

      this->scene->addInstanceGroup(
        new GrassGroup(
          grassShader, 
          this->grassMaterial, 
          startPosition + basePosition, grassPerChunkX, 
          grassPerChunkZ, 
          this->minXDistance, 
          this->minZDistance
        )
      );
    }
  }
}

void GrassPlane::draw(glm::mat4 &viewProjection)
{
  if(this->hide) return;

  this->mesh->bindVAO();

  const uint16_t shaderID = this->shaderProgram->getProgram();
  glUseProgram(shaderID);

  glm::vec2 textureScale = glm::vec2(this->getScale().x, this->getScale().y) * 0.05f;

  this->shaderProgram->bindVec2("textureScale", textureScale);

  GameObject::draw(viewProjection);
}