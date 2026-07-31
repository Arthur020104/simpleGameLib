#pragma once
#include <ray.h>
#include <vector>
#include <vertex.h>
#include <cy/cyTriMesh.h>
#include <gameObject.h>
#include <meshBvhNode.h>

struct Ray;
class GameObject;
class MeshBvhNode;

class Mesh
{
  public:
    bool intersectMesh(Ray& ray, GameObject* gameObject);

    Mesh(std::vector<Vertex> inputVertices, cy::Vec3f boundingVolume[2]);

    Mesh(std::vector<Vertex> inputVertices);


    ~Mesh();

    void registerObjectWithMesh(GameObject* obj);//for now passing the obj is useless, but later I may want to add a list of objects using this mesh

    uint16_t getUsingMesh();

    uint16_t removeUsingMesh(GameObject* obj);

    uint32_t getId();
    
    const std::vector<Vertex>& getVertices();

    const uint32_t getTriangleCount() { return this->triangleCount; }

    void bindVAO();
    

    cy::Vec3f boundingVolume[2];
  private:
    uint32_t id;
    std::vector<Vertex> vertices;
    //if later want to add material indice create the vbo on the gameObject as an separeted buffer(idk if is possible check later)
    //Add EBO SUPPORT LATER
    uint32_t VAO, VBO;
    uint32_t triangleCount;

    uint16_t objectsUsingMesh = 0;

    inline static uint32_t nextMeshId = 0;

    inline static float upperBound = 1.0f + ERROR_MARGIN;

    MeshBvhNode* bvh = nullptr;

    void init(std::vector<Vertex>& inputVertices, cy::Vec3f boundingVolume[2]);
};