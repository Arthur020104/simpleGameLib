#include <gameObject.h>
#include <mesh.h>
#include <GL/glew.h>
#include <ray.h>

GameObject::GameObject(std::shared_ptr<Mesh> meshData, std::shared_ptr<Program> shader, std::shared_ptr<Material> material): mesh(meshData), shaderProgram(shader), Component()
{
  this->shaderProgram->registerObjectUsingProgram(this);
  this->useOnly(material);
}

const std::shared_ptr<Mesh> GameObject::getMesh()
{
  return this->mesh;
}

const std::shared_ptr<Program> GameObject::getShaderProgram()
{
  return this->shaderProgram;
}

GameObject::~GameObject()
{
  glDeleteBuffers(1, &materialIndicesVBO);
}

void GameObject::draw(cy::Matrix4f &viewProjection)
{
  this->mesh->bindVAO();

  const uint16_t shaderID = this->shaderProgram->getProgram();
  glUseProgram(shaderID);
  this->scene->bindSceneLights(this->shaderProgram.get());

  const cy::Matrix4f mvp = viewProjection * this->getModelMatrix();

  this->shaderProgram->bindMat4("mvp", mvp);
  this->shaderProgram->bindMat4("modelMatrix", this->getModelMatrix());
  
  Camera* activeCamera = this->scene->getActiveCamera();
  this->shaderProgram->bindVec3("viewPosition", activeCamera->getPosition());
  
  uint8_t texUnit = 0;
  for(uint8_t i = 0; i < this->materials.size(); i++)
  {
    this->materials[i]->bind(this->shaderProgram.get(), "materials", i, texUnit);
    texUnit += this->materials[i]->activeTextures;
  }

  this->mesh->renderMesh();
    
}

void GameObject::draw(Camera* camera)
{
  cy::Matrix4f viewProjection = camera->getViewProjection();
  draw(viewProjection);
}

void GameObject::draw(cy::Matrix4f &projection, cy::Matrix4f &view)
{
  cy::Matrix4f viewProjection = projection * view;
  draw(viewProjection);
}

bool GameObject::intersect(Ray& ray)
{
  //the ray vector will be transformed to the object space
  cy::Matrix4f invertedModelMatrix = this->getModelMatrix().GetInverse();

  cy::Vec4f modelSpaceOrigin = invertedModelMatrix * cy::Vec4f(ray.origin, 1.0f);
  cy::Vec4f modelSpaceDirection = invertedModelMatrix * cy::Vec4f(ray.direction, 0.0f);
  
  float scale = modelSpaceDirection.XYZ().Length();

  Ray modelSpaceRay;
  modelSpaceRay.origin = modelSpaceOrigin.XYZ();
  modelSpaceRay.direction = modelSpaceDirection.XYZ() / scale;

  if(!this->mesh->intersectMesh(modelSpaceRay, this)) return false;

  for(Hit& hit : modelSpaceRay.hits)
  {
    hit.point = (this->getModelMatrix() * cy::Vec4f(hit.point, 1.0f)).XYZ();
    ray.hits.push_back(hit);
  }
  
  return true;
}

void GameObject::useOnly(std::shared_ptr<Material> material)
{
  uint8_t i;
  for(i = 0; i < this->materials.size(); i++)
  {
    if(this->materials[i] == material) break;
  }

  if(i == this->materials.size())
    this->materials.push_back(material);

  this->materialIndices.assign(this->mesh->getVerticesAmount(), i);
  this->loadMaterialIndicesToGPU();
}

void GameObject::loadMaterialIndicesToGPU()
{
  uint32_t amountOfVertices = this->materialIndices.size();
  bool validMaterialIndices = amountOfVertices == this->mesh->getVerticesAmount();

  if(!validMaterialIndices)
    throw std::runtime_error("Error: The number of material indices does not match the number of vertices in the mesh.");

  this->mesh->bindVAO();

  if(!hasMaterialVBO)
    glGenBuffers(1, &this->materialIndicesVBO);

  glBindBuffer(GL_ARRAY_BUFFER, this->materialIndicesVBO);

  glBufferData(
    GL_ARRAY_BUFFER, 
    amountOfVertices * sizeof(uint8_t), 
    this->materialIndices.data(), 
    GL_STATIC_DRAW
  );

  glEnableVertexAttribArray(3);
  glVertexAttribIPointer(
    3,
    1,
    GL_UNSIGNED_BYTE,
    sizeof(uint8_t),
    (void*)0
  );
  glBindVertexArray(0);

  hasMaterialVBO = true;
}

void GameObject::addMaterial(std::shared_ptr<Material> material)
{
  this->materials.push_back(material);
}

void GameObject::useMaterial(std::shared_ptr<Material> material, uint32_t startIdx, uint32_t endIdx)
{
  if(startIdx > endIdx || endIdx >= this->materialIndices.size())
    throw std::runtime_error("Error: Invalid start or end index for material assignment.");

  uint8_t i;
  for(i = 0; i < this->materials.size(); i++)
  {
    if(this->materials[i] == material) break;
  }

  if(i == this->materials.size())
    throw std::runtime_error("Error: Material not found in the GameObject's material list.");

  for(uint32_t j = startIdx; j <= endIdx; j++)
  {
    this->materialIndices[j] = i;
  }

  this->loadMaterialIndicesToGPU();
}

void GameObject::addMaterial(std::shared_ptr<Material> material, uint32_t startIdx, uint32_t endIdx)
{
  this->addMaterial(material);
  this->useMaterial(material, startIdx, endIdx);
}