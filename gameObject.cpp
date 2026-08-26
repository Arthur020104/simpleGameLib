#include <gameObject.h>
#include <mesh.h>
#include <GL/glew.h>
#include <ray.h>

GameObject::GameObject(std::shared_ptr<Mesh> meshData, std::shared_ptr<Program> shader, std::vector<std::shared_ptr<Material>> material): mesh(meshData), shaderProgram(shader), materials(material), Component()
{
  if(material.size() == 0)
    throw std::runtime_error("Error: GameObject must have at least one material.");

  this->shaderProgram->registerObjectUsingProgram(this);
  this->useOnly(material[0]);
}
/* cy::TriMesh objTriMesh;
  if(!objTriMesh.LoadFromFileObj(path.c_str(), true, &std::cout)) std::cerr << "Failed to load Obj from file. Path: '"<< path << "'.\n";*/

GameObject::GameObject(std::string path, std::shared_ptr<Program> shader): Component()
{
  cy::TriMesh objTriMesh;
  if(!objTriMesh.LoadFromFileObj(path.c_str(), true, &std::cout)) 
    throw std::invalid_argument("Failed to load Obj from file. Path: '" + path + "'.\n");

  this->loadGameObjectFromPath(objTriMesh, shader);
}

GameObject::GameObject(cy::TriMesh& objTriMesh, std::shared_ptr<Program> shader): Component()
{
  this->loadGameObjectFromPath(objTriMesh, shader);
}

GameObject::GameObject(const char* path, std::shared_ptr<Program> shader): Component()
{
  cy::TriMesh objTriMesh;
  if(!objTriMesh.LoadFromFileObj(path, true, &std::cout)) 
    throw std::invalid_argument("Failed to load Obj from file. Path: '" + std::string(path) + "'.\n");

  this->loadGameObjectFromPath(objTriMesh, shader);
}
void GameObject::loadGameObjectFromPath(cy::TriMesh& objTriMesh, std::shared_ptr<Program> shader)
{
  this->mesh = std::make_shared<Mesh>(objTriMesh);
  
  this->materialIndices.reserve(mesh->getVerticesAmount());

  for(uint32_t i = 0; i < objTriMesh.NF(); i++)
  {
    int materialIdx = objTriMesh.GetMaterialIndex(i);

    if (materialIdx < 0) materialIdx = 0;

    this->materialIndices.insert(this->materialIndices.end(), 3, (uint8_t)materialIdx);
  }

  if(objTriMesh.NM() == 0)
    this->materials.push_back(DEFAULT_MATERIAL);

  for(uint8_t i = 0; i < objTriMesh.NM(); i++)
  { 
    cy::TriMesh::Mtl material = objTriMesh.M(i);

    glm::vec3 diffuseColor = glm::vec3(material.Kd[0], material.Kd[1], material.Kd[2]);
    if(glm::length(diffuseColor) == 0.0f) diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 specularColor = glm::vec3(material.Ks[0], material.Ks[1], material.Ks[2]);
    if(glm::length(specularColor) == 0.0f) specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    
    // glm::vec3 ka = glm::vec3(material.Ka);
      
    float shininess = material.Ns > 0.0f ? material.Ns : 1.0f;
    
    this->materials.push_back(std::make_shared<Material>(diffuseColor, specularColor, shininess));

    if(material.map_Kd != nullptr)
      this->materials[i]->addDiffuseTexture(material.map_Kd.data);

    if(material.map_Ks != nullptr)
      this->materials[i]->addSpecularTexture(material.map_Ks.data);

    if(material.map_Ke != nullptr)
      this->materials[i]->addEmissiveTexture(material.map_Ke.data);
  }
  
  /*
    Str   name;		//!< Material name
		float Ka[3];	//!< Ambient color
		float Kd[3];	//!< Diffuse color
		float Ks[3];	//!< Specular color
		float Tf[3];	//!< Transmission color
		float Ns;		//!< Specular exponent
		float Ni;		//!< Index of refraction
		int   illum;	//!< Illumination model
		Str   map_Ka;	//!< Ambient color texture map
		Str   map_Kd;	//!< Diffuse color texture map
		Str   map_Ks;	//!< Specular color texture map
		Str   map_Ns;	//!< Specular exponent texture map
		Str   map_d;	//!< Alpha texture map
		Str   map_bump;	//!< Bump texture map
		Str   map_disp;	//!< Displacement texture map*/

  this->shaderProgram = shader;
  this->shaderProgram->registerObjectUsingProgram(this);
  this->loadMaterialIndicesToGPU();
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

void GameObject::draw(glm::mat4 &viewProjection)
{
  this->mesh->bindVAO();

  const uint16_t shaderID = this->shaderProgram->getProgram();
  glUseProgram(shaderID);
  this->scene->bindSceneLights(this->shaderProgram.get());

  const glm::mat4 mvp = viewProjection * this->getModelMatrix();

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
  glm::mat4 viewProjection = camera->getViewProjection();
  draw(viewProjection);
}

void GameObject::draw(glm::mat4 &projection, glm::mat4 &view)
{
  glm::mat4 viewProjection = projection * view;
  draw(viewProjection);
}

bool GameObject::intersect(Ray& ray)
{
  //the ray vector will be transformed to the object space
  glm::mat4 invertedModelMatrix = glm::inverse(this->getModelMatrix());

  glm::vec4 modelSpaceOrigin = invertedModelMatrix * glm::vec4(ray.origin, 1.0f);
  glm::vec4 modelSpaceDirection = invertedModelMatrix * glm::vec4(ray.direction, 0.0f);
  
  Ray modelSpaceRay;
  modelSpaceRay.origin = glm::vec3(modelSpaceOrigin);
  modelSpaceRay.direction = glm::normalize(glm::vec3(modelSpaceDirection));

  if(!this->mesh->intersectMesh(modelSpaceRay, this)) return false;

  for(Hit& hit : modelSpaceRay.hits)
  {
    hit.point = glm::vec3(this->getModelMatrix() * glm::vec4(hit.point, 1.0f));
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

  this->useOnly(i);
}

void GameObject::GameObject::useOnly(uint8_t materialIndex)
{
  if(materialIndex >= this->materials.size())
    throw std::runtime_error("Error: Material index out of bounds.");

  this->materialIndices.assign(this->mesh->getVerticesAmount(), materialIndex);
  this->loadMaterialIndicesToGPU();
}

void GameObject::loadMaterialIndicesToGPU()
{
  uint32_t amountOfVertices = this->materialIndices.size();
  bool validMaterialIndices = amountOfVertices == this->mesh->getVerticesAmount();

  if(!validMaterialIndices)
    throw std::runtime_error("Error: The number of material indices does not match the number of vertices in the mesh. Number of vertices: " + std::to_string(this->mesh->getVerticesAmount()) + ", Number of material indices: " + std::to_string(amountOfVertices));

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

void GameObject::setMaterialIndices(std::vector<uint8_t> materialIndices)
{
  this->materialIndices = materialIndices;
  this->loadMaterialIndicesToGPU();
}