#include <instanceGroup.h>
#include <algorithm>
#include <glm/gtc/matrix_access.hpp>
#include <unordered_map>

InstanceGroup::InstanceGroup(std::shared_ptr<Mesh> mesh, std::shared_ptr<Program> program, std::vector<GameObject*> objs)
{
  std::set<GameObject*> setObjs( objs.begin(), objs.end() );
  this->init(mesh, program, setObjs);
}

InstanceGroup::InstanceGroup(std::shared_ptr<Mesh> mesh, std::shared_ptr<Program> program, std::set<GameObject*> objs)
{
  this->init(mesh, program, objs);
}

InstanceGroup::~InstanceGroup()
{
  if(this->hasVbo)
  {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &MaterialVbo);
  }
}

void InstanceGroup::init(std::shared_ptr<Mesh> mesh, std::shared_ptr<Program> program, std::set<GameObject*> objs)
{
  this->mesh = mesh;
  this->program = program;
  this->objs = objs;

  for(GameObject* obj: this->objs)
  {
    assert(obj != nullptr);
    assert(obj->getShaderProgram().get() == this->program.get());

    obj->hide = true;

    if(mesh.get() != obj->getMesh().get() && mesh->meshHash == obj->getMesh()->meshHash)
      obj->setMesh(this->mesh);
    else if(mesh.get() != obj->getMesh().get() && mesh->meshHash != obj->getMesh()->meshHash)
      throw std::runtime_error("Error: InstanceGroup can only contain GameObjects with the same mesh.");

    if(obj->getGameObjectType() != this->gameObjectType)
      this->gameObjectType = GameObjectType::DYNAMIC;
  }

  if(this->objs.size() > 0)
    this->passDataToGPU();
}

void InstanceGroup::removeObject(GameObject* obj)
{
  obj->hide = false;
  this->objs.erase(obj);

  this->passDataToGPU();
}

void InstanceGroup::addObject(GameObject* obj)
{
  obj->hide = true;
  this->objs.insert(obj);

  if(mesh.get() != obj->getMesh().get() && mesh->meshHash == obj->getMesh()->meshHash)
    obj->setMesh(this->mesh);
  else if(mesh.get() != obj->getMesh().get() && mesh->meshHash != obj->getMesh()->meshHash)
    throw std::runtime_error("Error: InstanceGroup can only contain GameObjects with the same mesh.");
  
  if(obj->getGameObjectType() != this->gameObjectType)
    this->gameObjectType = GameObjectType::DYNAMIC;

  this->passDataToGPU();
}

void InstanceGroup::draw(Camera* cam, Scene* scene)
{
  const uint16_t shaderID = this->program->getProgram();
  glUseProgram(shaderID);

  scene->bindSceneLights(this->program.get());

  const glm::mat4 mv = cam->getViewProjection();

  this->program->bindMat4("mv", mv);
  this->program->bindVec3("viewPosition", cam->getPosition());
  this->program->bindBool("hasMaterialIndices", this->hasMutipleMaterialsPerObject);

  this->program->bindUint("verticesPerMesh", this->mesh->getVerticesAmount());
  
  this->bindForDrawing(); 
  this->mesh->bindVAO();

  glDrawArraysInstanced(GL_TRIANGLES, 0, this->mesh->getVerticesAmount(), this->objs.size()); 
}

void passModelMatrixData(std::vector<objData> &gpuData, GLuint VBO)
{
  std::size_t vec4Size = sizeof(glm::vec4);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(objData) * gpuData.size(), gpuData.data(), GL_STATIC_DRAW);

  for(uint8_t i = 0; i < 4; i++)
  {
    glEnableVertexAttribArray(4 + i);
    glVertexAttribPointer(
      4 + i,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(objData),
      (void*)(i * vec4Size)
    );
    glVertexAttribDivisor(4 + i, 1);
  }
}

void InstanceGroup::passDataToGPU()
{
  this->mesh->bindVAO();

  if(!this->hasVbo)
  {
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &MaterialVbo);
    this->hasVbo = true;  
  }

  uint8_t textureCounter = 0;//use it later to limit, keep the same max as in the shader,
  uint8_t materialCounter = 0;//use it later to limit, keep the same max as in the shader,

  std::vector<objData> gpuData;
  gpuData.reserve(this->objs.size());

  materialToIndexMap.clear();

  std::vector<uint8_t> allMatIdx;

  for (GameObject* item : this->objs)
  {
    if(!this->hasMutipleMaterialsPerObject && item->hasMultipleMaterials())
      this->hasMutipleMaterialsPerObject = true;
  }

  for (GameObject* item : this->objs)
  {
    objData data;
    data.modelMatrixRow0 = glm::row(item->getModelMatrix(), 0);
    data.modelMatrixRow1 = glm::row(item->getModelMatrix(), 1);
    data.modelMatrixRow2 = glm::row(item->getModelMatrix(), 2);
    data.modelMatrixRow3 = glm::row(item->getModelMatrix(), 3);

    gpuData.push_back(data);

    std::vector<uint8_t> materialsIdx;

    if(!this->hasMutipleMaterialsPerObject)
    {
      materialsIdx = {0};
    }
    else if(item->hasMultipleMaterials())
    {
      materialsIdx = item->getMaterialIndices();
      assert(materialsIdx.size() == item->getMesh()->getVerticesAmount() && "Material indices size does not match the number of vertices in the mesh.");
    }  
    else
      materialsIdx.resize(item->getMesh()->getVerticesAmount(), 0);
    
    std::vector<std::shared_ptr<Material>> materials = item->getMaterials();
    for(uint8_t i = 0; i < materials.size(); i++)
    {
      assert(materialCounter < 255 && "material slot counter overflow");

      bool alreadyLoaded = true;
      if(materialToIndexMap.find(materials[i].get()) == materialToIndexMap.end())
      {
        materialToIndexMap[materials[i].get()] = materialCounter;
        alreadyLoaded = false;
      }

      uint8_t idx = materialToIndexMap[materials[i].get()];
      
      if(i != idx)
        std::replace(materialsIdx.begin(), materialsIdx.end(), i, idx);
      
      if(alreadyLoaded)
        continue;

      textureCounter += materials[i]->activeTextures;      
      
      if(!alreadyLoaded) materialCounter++;
    }
    allMatIdx.insert(allMatIdx.end(), materialsIdx.begin(), materialsIdx.end());
  }

  passModelMatrixData(gpuData, VBO);

  while (allMatIdx.size() % 4 != 0 && this->hasMutipleMaterialsPerObject) 
    allMatIdx.push_back(0);

  if(this->hasMutipleMaterialsPerObject)
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, MaterialVbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint8_t) * this->mesh->getVerticesAmount() * gpuData.size(), allMatIdx.data(), GL_STATIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, MaterialVbo);
  } 
  else
  {
    glBindBuffer(GL_ARRAY_BUFFER, MaterialVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uint8_t) * allMatIdx.size(), allMatIdx.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
      3,
      1,
      GL_UNSIGNED_BYTE,
      GL_FALSE,
      sizeof(uint8_t),
      (void*)0
    );
    glVertexAttribDivisor(3, 1);
  }
}

void InstanceGroup::bindForDrawing()
{
  if(!this->hasVbo)
    throw std::runtime_error("Error: VBO not loaded. Call passDataToGPU() before drawing.");

  this->mesh->bindVAO();

  uint8_t textureCounter = 0;//use it later to limit, keep the same max as in the shader,

  for(auto material: this->materialToIndexMap)
  {
    material.first->bind(this->program.get(), "materials", this->materialToIndexMap[material.first], textureCounter);
    textureCounter += material.first->activeTextures;
  }

  std::vector<objData> gpuData;
  gpuData.reserve(this->objs.size());

  for (GameObject* item : this->objs)
  {
    objData data;
    data.modelMatrixRow0 = glm::row(item->getModelMatrix(), 0);
    data.modelMatrixRow1 = glm::row(item->getModelMatrix(), 1);
    data.modelMatrixRow2 = glm::row(item->getModelMatrix(), 2);
    data.modelMatrixRow3 = glm::row(item->getModelMatrix(), 3);

    gpuData.push_back(data);
  }

  passModelMatrixData(gpuData, VBO);
}
