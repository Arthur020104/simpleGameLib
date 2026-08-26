#include <mesh.h>
#include <cstdio>
#include <cstddef>
#include <GL/glew.h>
#include <algorithm>

//PRE-DEFINED MESHES
std::shared_ptr<Mesh> Mesh::createQuadMesh()
{
  Vertex v1, v2, v3, v4;
  v1.pos = glm::vec3(-1.0f, -1.0f, 0.0f); v1.normal = glm::vec3(0.0f, 0.0f, 1.0f); v1.uv = glm::vec2(0.0f, 0.0f);
  v2.pos = glm::vec3(1.0f, -1.0f, 0.0f); v2.normal = glm::vec3(0.0f, 0.0f, 1.0f); v2.uv = glm::vec2(1.0f, 0.0f);
  v3.pos = glm::vec3(-1.0f, 1.0f, 0.0f); v3.normal = glm::vec3(0.0f, 0.0f, 1.0f); v3.uv = glm::vec2(0.0f, 1.0f);
  v4.pos = glm::vec3(1.0f, 1.0f, 0.0f); v4.normal = glm::vec3(0.0f, 0.0f, 1.0f); v4.uv = glm::vec2(1.0f, 1.0f);

  return std::make_shared<Mesh>(Mesh({v1, v2, v4, v1, v4, v3}, MeshType::TRIANGLE_MESH));
}

std::shared_ptr<Mesh> Mesh::getQuadMesh()
{
  static std::shared_ptr<Mesh> quadMesh = createQuadMesh();
  return quadMesh;
}

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
  glm::vec3 positiveInfinity = glm::vec3(INFINITY, INFINITY, INFINITY);
  glm::vec3 negativeInfinity = glm::vec3(-INFINITY, -INFINITY, -INFINITY);

  glm::vec3 boundingVolumeC[2] = {positiveInfinity, negativeInfinity};

  for(Vertex& vertex: inputVertices)
  {
    glm::vec3 itemsBBox[2] = { positiveInfinity, negativeInfinity };

    alterBoudingMin(vertex.pos, itemsBBox[0]);
    alterBoudingMin(vertex.pos, itemsBBox[1]);

    alterBoudingMin(itemsBBox[0], boundingVolumeC[0]);
    alterBoudingMin(itemsBBox[1], boundingVolumeC[1]);
  }
  init(inputVertices, type, boundingVolumeC);
}

Mesh::Mesh(const char* path)
{
  cy::TriMesh objTriMesh;
  if(!objTriMesh.LoadFromFileObj(path, true, &std::cout)) 
    throw std::invalid_argument("Failed to load Obj from file. Path: '" + std::string(path) + "'.\n");

  this->loadMesh(objTriMesh);
}

Mesh::Mesh(std::string path)
{
  cy::TriMesh objTriMesh;
  if(!objTriMesh.LoadFromFileObj(path.c_str(), true, &std::cout)) 
    throw std::invalid_argument("Failed to load Obj from file. Path: '" + path + "'.\n");

  this->loadMesh(objTriMesh);
}

Mesh::Mesh(cy::TriMesh& objTriMesh)
{
  this->loadMesh(objTriMesh);
}

Mesh::Mesh(std::vector<Vertex> inputVertices, glm::vec3 boundingVolume[2], MeshType type)
{
  init(inputVertices, type, boundingVolume);
}

void Mesh::loadMesh(cy::TriMesh& objTriMesh)
{
  if(!objTriMesh.HasNormals()) objTriMesh.ComputeNormals();

  if(!objTriMesh.IsBoundBoxReady()) objTriMesh.ComputeBoundingBox();

  std::vector<Vertex> meshData;

  for(uint32_t i = 0; i < objTriMesh.NF(); i++)
  {
    const cy::TriMesh::TriFace faceVerticesPos = objTriMesh.F(i);
    const cy::TriMesh::TriFace faceVerticesNormals = objTriMesh.FN(i);

    cy::TriMesh::TriFace faceVerticesUv;

    if(objTriMesh.HasTextureVertices()) faceVerticesUv = objTriMesh.FT(i);

    for(uint8_t j = 0; j < 3; j++)
    {
      Vertex vertex;

      cy::Vec3f auxVec = objTriMesh.V(faceVerticesPos.v[j]);
      vertex.pos = glm::vec3(auxVec.x, auxVec.y, auxVec.z);

      auxVec = objTriMesh.VN(faceVerticesNormals.v[j]);
      vertex.normal = glm::vec3(auxVec.x, auxVec.y, auxVec.z);

      if(objTriMesh.HasTextureVertices())
      {
        auxVec = objTriMesh.VT(faceVerticesUv.v[j]);
        vertex.uv = glm::vec2(auxVec.x, auxVec.y);
      }
      else
      {
        vertex.uv = glm::vec2(0.0f, 0.0f);
      }

      meshData.push_back(vertex);
    }
  }

  glm::vec3 boundMin = glm::vec3(objTriMesh.boundMin.x, objTriMesh.boundMin.y, objTriMesh.boundMin.z);
  glm::vec3 boundMax = glm::vec3(objTriMesh.boundMax.x, objTriMesh.boundMax.y, objTriMesh.boundMax.z);

  glm::vec3 bbox[2] = {boundMin, boundMax};
  init(meshData, MeshType::TRIANGLE_MESH, bbox);
}

void Mesh::init(std::vector<Vertex>& inputVertices, MeshType type, glm::vec3 boundingVolume[2])
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