#include <gameObject.h>
#include <mesh.h>
#include <GL/glew.h>
#include <ray.h>

GameObject::GameObject(std::shared_ptr<Mesh> meshData, std::shared_ptr<Program> shader): mesh(meshData), shaderProgram(shader), Component()
{
  this->shaderProgram->registerObjectUsingProgram(this);
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
  
}

void GameObject::draw(cy::Matrix4f &viewProjection)
{
  this->mesh->bindVAO();

  const uint16_t shaderID = this->shaderProgram->getProgram();
  glUseProgram(shaderID);
  this->scene->bindSceneLights(this->shaderProgram.get());

  const cy::Matrix4f mvp = viewProjection * this->getModelMatrix();

  this->shaderProgram->bindMat4("mvp", mvp);
  this->shaderProgram->bindBool("isSelected", this->isSelected);
  this->shaderProgram->bindMat4("modelMatrix", this->getModelMatrix());
  
  Camera* activeCamera = this->scene->getActiveCamera();
  this->shaderProgram->bindVec3("viewPosition", activeCamera->getPosition());
  
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