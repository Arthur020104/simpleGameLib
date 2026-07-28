#include <mesh.h>
#include <GL/glew.h>
#include <cstddef>
#include <cstdio>
#include <ray.h>
#include <vertex.h>
#include <ray.h>

const std::vector<Vertex>& Mesh::getVertices() 
{ 
  return this->vertices; 
}

uint32_t Mesh::getId() 
{ 
  return this->id; 
}

Mesh::Mesh(const std::vector<Vertex>& inputVertices): id(Mesh::nextMeshId++), vertices(inputVertices) 
{
  this->triangleCount = this->vertices.size() / 3;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
  // Upload vertex data to GPU memory
  glBufferData(
    GL_ARRAY_BUFFER,
    this->vertices.size() * sizeof(Vertex),
    this->vertices.data(),
    GL_STATIC_DRAW //IF DATA WILL CHANGE, MAKING ALTERIONS TO THE BUFFER, USE GL_DYNAMIC_DRAW OR GL_STREAM_DRAW
  );

  //VERTEX 
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 
    3, 
    GL_FLOAT, 
    GL_FALSE, 
    sizeof(Vertex), 
    (void*)offsetof(Vertex, pos)
  );
  
  //NORMAL
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    (void*)offsetof(Vertex, normal)
  );

  //UV
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
    2,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    (void*)offsetof(Vertex, uv)
  );
  glBindVertexArray(0);
}

void Mesh::bindVAO() 
{
  glBindVertexArray(this->VAO);
}

bool Mesh::intersectTriangle(Ray& ray, GameObject* gameObject, Vertex &v0, Vertex &v1, Vertex &v2)
{
  cy::Vec3f planeNormal = (v1.pos - v0.pos) ^ (v2.pos - v0.pos);

  float overArea = 1.0f / planeNormal.Length();

  planeNormal.Normalize();

  cy::Vec3f centerPoint = (v0.pos + v1.pos + v2.pos) *  0.33333333333f;

  float v = planeNormal % ray.direction;

  const float t = (planeNormal % (centerPoint - ray.origin)) / v;

  if(t < TMIN) return false;

  cy::Vec3f pointOnPlane = ray.origin + ray.direction * t;

  float total = 0;

  float alpha = (((v1.pos - pointOnPlane) ^ (v2.pos - pointOnPlane)) % planeNormal) * overArea;
  if (alpha < 0.0f || alpha > 1.0f) return false;

  float beta = (((v2.pos - pointOnPlane) ^ (v0.pos - pointOnPlane)) % planeNormal) * overArea;
  if (beta < 0.0f || beta > 1.0f) return false;

  float gamma = 1.0f - alpha - beta;
  if (gamma < 0.0f || gamma > 1.0f) return false;
  
  total = alpha + beta + gamma;

  if(abs(total - 1.0) > ERROR_MARGIN) return false;

  Hit hit;
  hit.point = pointOnPlane;
  hit.hitObject = gameObject;

  ray.hits.push_back(hit);

  return true;
}

bool Mesh::intersectMesh(Ray& ray, GameObject* gameObject)
{
  for(uint32_t i = 0; i < this->vertices.size(); i += 3)
  {
    if(Mesh::intersectTriangle(ray, gameObject, vertices[i], vertices[i + 1], vertices[i + 2]))
      return true;
  }

  return false;
}