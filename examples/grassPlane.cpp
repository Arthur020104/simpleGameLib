#include <grassPlane.h> 
#include <../program.h>
#include <random>

float getRandomFloat(float min, float max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}

GrassPlane::GrassPlane(glm::vec3 position): GameObject(std::make_shared<Mesh>("../obj/plane.obj"), Program::getDefaultShader())
{
  this->grassMesh = std::make_shared<Mesh>("../obj/single_grass_blade/single_grass_blade.obj");
  this->grassMaterial = std::make_shared<Material>(glm::vec3(0.075f, 0.3f, 0.075f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
  // this->grassGroup = nullptr;
  this->setPosition(position);
}

GrassGroup* GrassPlane::generateGrassInstance(glm::vec3 basePosition, uint16_t& grassAmountX, uint16_t& grassAmountZ)
{
  std::vector<GameObject*> grassObjects;

  for(uint16_t i = 0; i < grassAmountX; i++)
  {
    for(uint16_t j = 0; j < grassAmountZ; j++)
    {
      GrassInstance* grass = new GrassInstance(this->grassMesh, Program::getDefaultShader(), {this->grassMaterial}, GameObjectType::STATIC);
      glm::vec3 grassPosition = glm::vec3(basePosition.x, basePosition.y, basePosition.z);

      float xNoise = i != 0 && i != grassAmountX - 1 ? getRandomFloat(-this->minXDistance * 0.5f, this->minXDistance * 0.5f) : 0.0f;
      float zNoise = j != 0 && j != grassAmountZ - 1 ? getRandomFloat(-this->minZDistance * 0.5f, this->minZDistance * 0.5f) : 0.0f;
      
      grassPosition.x += 0.0f + i * this->minXDistance;
      grassPosition.z += 0.0f + j * this->minZDistance;
      
      glm::vec3 rotation = glm::vec3(0.0f, getRandomFloat(0.0f, 180.0f), 0.0f);
      
      grass->setRotation(rotation);
      
      float scale = getRandomFloat(3.0f, 5.0f);
      glm::vec3 scaleVec = glm::vec3(scale, scale, scale);

      grass->setPosition(grassPosition);

      grass->setScale(scaleVec);

      grassObjects.push_back(grass);
    }
  }
  GrassGroup* grassGroup = new GrassGroup(grassObjects);
  grassGroup->baseY = this->getPosition().y;
  grassGroup->grassStemSizeY = 2.0f;
  grassGroup->center = basePosition + glm::vec3(grassAmountX * this->minXDistance * 0.5f, 0.0f, grassAmountZ * this->minZDistance * 0.5f);
  grassGroup->scene = this->scene;
  return grassGroup;
}

void GrassPlane::start()
{
  std::shared_ptr<Material> planeMaterial = std::make_shared<Material>("../obj/grass.png", glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
  planeMaterial->diffuseTex->setWrapping(GL_REPEAT, GL_REPEAT);
  this->useOnly(planeMaterial);

  glm::vec4 min = this->getModelMatrix() * glm::vec4(this->getMesh()->boundingVolume[0], 1.0f);
  glm::vec4 max = this->getModelMatrix() * glm::vec4(this->getMesh()->boundingVolume[1], 1.0f);

  this->basePosition = glm::vec3(min.x, this->getPosition().y, min.z) - glm::vec3(-0.6f, 0.0f, -0.6f);//offset

  uint16_t grassAmountX = std::floor((max.x - min.x) / this->minXDistance);
  uint16_t grassAmountZ = std::floor((max.z - min.z) / this->minZDistance);

  uint16_t xChunks = std::ceil((max.x - min.x) / this->chunkSizeX);
  uint16_t zChunks = std::ceil((max.z - min.z) / this->chunkSizeZ);

  float xChunkSize = (max.x - min.x) / (float)xChunks;
  float zChunkSize = (max.z - min.z) / (float)zChunks;

  uint16_t totalChunks = std::ceil(xChunks * zChunks);

  std::cout << "Grass chunks: " << xChunks << " x " << zChunks << std::endl;
  std::cout << "Grass total chunks: " << totalChunks << std::endl;
  std::cout<< "Grass chunk size: " << xChunkSize << " x " << zChunkSize << std::endl;
  std::cout << "Grass amount: " << grassAmountX << " x " << grassAmountZ << std::endl;
  std::cout << "Grass total amount: " << grassAmountX * grassAmountZ << std::endl;
  glm::vec3 chunkMaxBound = glm::vec3(max.x / this->chunkSizeX, 0.0f, max.z / this->chunkSizeZ);

  glm::vec3 addingToPosition = glm::vec3(0.0f, 0.0f, 0.0f);

  std::vector<GrassInstance*> grassInstances;
  grassInstances.assign(totalChunks, nullptr);

  uint16_t currentGroup = 0;

  uint16_t grassPerChunkX = std::ceil(grassAmountX / (float)xChunks);
  uint16_t grassPerChunkZ = std::ceil(grassAmountZ / (float)zChunks);
  for(uint32_t i = 0; i < xChunks; i++)
  {
    glm::vec3 startPosition = glm::vec3(xChunkSize * i, 0.0f, 0.0f);
    for(uint32_t j = 0; j < zChunks; j++)
    {
      startPosition.z = zChunkSize * j;

      this->scene->instances.push_back(this->generateGrassInstance(startPosition + basePosition, grassPerChunkX, grassPerChunkZ));
    }
  }

  // for(uint32_t i = 0; i < grassAmountX; i++)
  // {
  //   for(uint32_t j = 0; j < grassAmountZ; j++)
  //   {
  //     if(j % grassPerChunkZ == 0 && j != 0)
  //     {
  //       grassInstances[currentGroup] = new GrassGroup(grassObjects);
  //       grassObjects.clear();

  //       currentGroup++;
  //     }
  //     GrassInstance* grass = new GrassInstance(this->grassMesh, Program::getDefaultShader(), {grassMaterial}, GameObjectType::STATIC);

  //     float xNoise = i == 0 || i == grassAmountX - 1 ? getRandomFloat(-this->minXDistance * 0.5f, this->minXDistance * 0.5f) : 0.0f;
  //     float zNoise = j == 0 || j == grassAmountZ - 1 ? getRandomFloat(-this->minZDistance * 0.5f, this->minZDistance * 0.5f) : 0.0f;

  //     float xPos = xNoise + i * this->minXDistance;
  //     float zPos = zNoise + j * this->minZDistance;
      
  //     float rotationAngle = getRandomFloat(0.0f, 180.0f);
  //     grass->setRotation(glm::vec3(0.0f, rotationAngle, 0.0f));
      
  //     float yScale = getRandomFloat(3.0f, 5.0f);
  //     float yPos = 0.0f;//(this->grassMesh->boundingVolume[1].y - this->grassMesh->boundingVolume[0].y) * yScale * 0.5f;

  //     grass->setPosition(this->basePosition + glm::vec3(xPos, yPos, zPos));
  //     // std::cout << "Grass position: " << grass->getPosition().x << ", " << grass->getPosition().y << ", " << grass->getPosition().z << std::endl;
  //     baseScaling.x = yScale;
  //     baseScaling.z = yScale;
  //     baseScaling.y = yScale;
  //     grass->setScale(baseScaling);
  //     // this->scene->addObject(grass);
  //     // if(i==0 && j==0)
  //     grassObjects.push_back(grass);
  //   }
  // }
  // this->grassGroup = new GrassGroup(grassObjects);
  // this->grassGroup->baseY = this->getPosition().y;
  // this->grassGroup->grassStemSizeY = 2.0f;
  // this->scene->instances.push_back(grassGroup);
}

void GrassPlane::fixedUpdate()
{
  // if(glfwGetKey(WINDOW.window, GLFW_KEY_MINUS) == GLFW_PRESS && !this->onAction)
  // {
  //   this->grassGroup->windStrength -= 0.5f;
  //   this->onAction = true;
  // }

  // if(glfwGetKey(WINDOW.window, GLFW_KEY_EQUAL) == GLFW_PRESS && !this->onAction)
  // {
  //   this->grassGroup->windStrength += 0.5f;
  //   this->onAction = true;
  // }

  // if(glfwGetKey(WINDOW.window, GLFW_KEY_MINUS) == GLFW_RELEASE && glfwGetKey(WINDOW.window, GLFW_KEY_EQUAL) == GLFW_RELEASE)
  // {
  //   this->onAction = false;
  // }
}