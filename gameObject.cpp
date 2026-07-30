#include <gameObject.h>
#include <mesh.h>
#include <GL/glew.h>
#include <ray.h>

GameObject::GameObject(Mesh* meshData, Program* shader): mesh(meshData), shaderProgram(shader) 
{
  this->mesh->registerObjectWithMesh(this);
  this->shaderProgram->registerObjectUsingProgram(this);
}

const Mesh* GameObject::getMesh()
{
  return this->mesh;
}

const Program* GameObject::getShaderProgram()
{
  return this->shaderProgram;
}

GameObject::~GameObject()
{
  if(this->mesh->removeUsingMesh(this) <= 0) delete this->mesh;

  if(this->shaderProgram->removeUsingProgram(this) <= 0) delete this->shaderProgram;
}

void GameObject::draw(cy::Matrix4f &viewProjection)
{
  this->mesh->bindVAO();

  const uint16_t shaderID = this->shaderProgram->getProgram();
  glUseProgram(shaderID);

  const cy::Matrix4f mvp = viewProjection * this->getModelMatrix();

  GLuint mvpLocation = glGetUniformLocation(shaderID, "mvp");
  glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp.cell);

  GLuint selectedLocation = glGetUniformLocation(shaderID, "isSelected");
  glUniform1i(selectedLocation, this->isSelected);
  if(this->mesh->getTriangleCount() >= 1)
    glDrawArrays(GL_TRIANGLES, 0, this->mesh->getTriangleCount() * 3);
  else{
    glDrawArrays(GL_LINES, 0, 2);
  }
    
}

void GameObject::draw(Camera* camera)
{
  draw(camera->getViewProjection());
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