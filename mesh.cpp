#include <mesh.h>
#include <GL/glew.h>
#include <cstddef>
#include <cstdio>
#include <ray.h>
#include <vertex.h>
#include <ray.h>
#include <algorithm>

const std::vector<Vertex>& Mesh::getVertices() 
{ 
  return this->vertices; 
}

uint32_t Mesh::getId() 
{ 
  return this->id; 
}

Mesh::~Mesh()
{
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  
  if(this->bvh != nullptr)
  {
    delete this->bvh;
    this->bvh = nullptr;
  }
}

Mesh::Mesh(std::vector<Vertex> inputVertices, MeshType type)
{
  cy::Vec3f positiveInfinity = cy::Vec3f(INFINITY, INFINITY, INFINITY);
  cy::Vec3f negativeInfinity = cy::Vec3f(-INFINITY, -INFINITY, -INFINITY);

  cy::Vec3f boundingVolumeC[2] = {positiveInfinity, negativeInfinity};

  for(Vertex& vertex: inputVertices)
  {
    cy::Vec3f itemsBBox[2] = { positiveInfinity, negativeInfinity };

    alterBoudingMin(vertex.pos, itemsBBox[0]);
    alterBoudingMin(vertex.pos, itemsBBox[1]);

    alterBoudingMin(itemsBBox[0], boundingVolumeC[0]);
    alterBoudingMin(itemsBBox[1], boundingVolumeC[1]);
  }
  init(inputVertices, type, boundingVolumeC);
}

Mesh::Mesh(std::vector<Vertex> inputVertices, cy::Vec3f boundingVolume[2], MeshType type)
{
  init(inputVertices, type, boundingVolume);
}

void Mesh::init(std::vector<Vertex>& inputVertices, MeshType type, cy::Vec3f boundingVolume[2])
{
  this->id = nextMeshId++;
  this->vertices = inputVertices;
  this->type = type;
  this->bvh = nullptr;

  switch (type)
  {
    case MeshType::TRIANGLE_MESH:
      if(vertices.size() % 3 != 0)  throw std::invalid_argument("Error: The number of vertices is not valid for a triangle mesh.\n");
      
      bvh = new MeshBvhNode(this->vertices, 0);
      break;
    case MeshType::LINE_MESH:
      if(vertices.size() % 2 != 0)  throw std::invalid_argument("Error: The number of vertices is not valid for a line mesh.\n");
      break;
    default:
      throw std::invalid_argument("Error: The mesh type is not valid.\n");
      break;
  }

  std::copy(boundingVolume, boundingVolume + 2, this->boundingVolume);

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

bool Mesh::intersectMesh(Ray& ray, GameObject* gameObject)
{
  if(this->bvh == nullptr) return false;

  return this->bvh->intersect(ray, gameObject);
}

void Mesh::registerObjectWithMesh(GameObject* obj)
{
  this->objectsUsingMesh++;
}

uint16_t Mesh::getUsingMesh()
{
  return this->objectsUsingMesh;
}

uint16_t Mesh::removeUsingMesh(GameObject* obj)
{
  return --this->objectsUsingMesh;
}

void Mesh::renderMesh()
{
  switch (this->type)
  {
    case MeshType::TRIANGLE_MESH:
      glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
      break;
    case MeshType::LINE_MESH:
      glDrawArrays(GL_LINES, 0, this->vertices.size());
      break;

    default:
      break;
  }
}