#pragma once
#include <ray.h>
#include <string>
#include <vector>
#include <vertex.h>
#include <GL/glew.h>
#include <gameObject.h>
#include <cy/cyTriMesh.h>
#include <meshBvhNode.h>

struct Ray;
class GameObject;
class MeshBvhNode;

enum MeshType {
  TRIANGLE_MESH,
  LINE_MESH
};

class Mesh
{
  public:
    static std::shared_ptr<Mesh> getQuadMesh();
  
    bool intersectMesh(Ray& ray, GameObject* gameObject);

    Mesh(std::vector<Vertex> inputVertices, glm::vec3 boundingVolume[2], MeshType type = MeshType::TRIANGLE_MESH);

    Mesh(std::vector<Vertex> inputVertices, MeshType type = MeshType::TRIANGLE_MESH);

    Mesh(std::string path);

    Mesh(const char* path);

    Mesh(cy::TriMesh& objTriMesh);

    ~Mesh();

    void registerObjectWithMesh(GameObject* obj);//for now passing the obj is useless, but later I may want to add a list of objects using this mesh

    uint32_t getVerticesAmount() {return this->vertices.size();};

    uint16_t getUsingMesh();

    uint16_t removeUsingMesh(GameObject* obj);

    uint32_t getId();
    
    const std::vector<Vertex>& getVertices();

    const uint32_t getTriangleCount() { return this->triangleCount; }

    void bindVAO();

    void renderMesh();

    glm::vec3 boundingVolume[2];
    uint32_t meshHash = 0;
  private:
    uint32_t id;
    
    std::vector<Vertex> vertices;
    //if later want to add material indice create the vbo on the gameObject as an separeted buffer(idk if is possible check later)
    //Add EBO SUPPORT LATER
    GLuint VAO, VBO;
    uint32_t triangleCount;

    MeshType type;

    uint16_t objectsUsingMesh = 0;

    inline static uint32_t nextMeshId = 0;

    inline static float upperBound = 1.0f + ERROR_MARGIN;

    MeshBvhNode* bvh = nullptr;

    void init(std::vector<Vertex>& inputVertices, MeshType type, glm::vec3 boundingVolume[2]);

    void loadMesh(cy::TriMesh& objTriMesh);

    static std::shared_ptr<Mesh> createQuadMesh();
};
