#pragma once
#include <ray.h>
#include <vector>
#include <vertex.h>
#include <cy/cyTriMesh.h>
#include <gameObject.h>

struct Ray;
class GameObject;

class Mesh
{
  public:
    bool intersectMesh(Ray& ray, GameObject* gameObject);

    Mesh(const std::vector<Vertex>& inputVertices);

    ~Mesh();

    void registerObjectWithMesh(GameObject* obj);//for now passing the obj is useless, but later I may want to add a list of objects using this mesh

    uint16_t getUsingMesh();

    u_int16_t removeUsingMesh(GameObject* obj);

    uint32_t getId();
    
    const std::vector<Vertex>& getVertices();

    const uint32_t getTriangleCount() { return this->triangleCount; }

    void bindVAO();
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

    static bool intersectTriangle(Ray& ray, GameObject* gameObject, Vertex &v0, Vertex &v1, Vertex &v2);
};