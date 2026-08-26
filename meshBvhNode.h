#pragma once
#include <ray.h>
#include <vertex.h>

class GameObject;
struct Ray;

class MeshBvhNode
{
  public:
    MeshBvhNode(std::vector<Vertex>& items, uint32_t depth);
    MeshBvhNode(std::vector<Vertex>& items, glm::vec3 boundingVolume[2], uint32_t depth);

    ~MeshBvhNode();

    std::vector<Vertex> items;
    MeshBvhNode* children[2] = {nullptr, nullptr};

    glm::vec3 boundingVolume[2];

    uint32_t depth;
    bool isLeaf;

    bool intersect(Ray& ray, GameObject* gameObject);
  
    private:  

    void init(std::vector<Vertex>& items, glm::vec3 boundingVolume[2], uint32_t depth);
    
    glm::vec3 getNewPoint(bool firstChild = true);


    GameObject* object = nullptr;

    static const uint8_t TARGET_ITEMS_PER_LEAF = 5;

    static const uint8_t MAX_DEPTH = 15;
};

void alterBoudingMin(glm::vec3& newVec, glm::vec3& baseVec);

void alterBoudingMax(glm::vec3& newVec, glm::vec3& baseVec);

bool insideBoundingVolume(glm::vec3& point, glm::vec3* boundingVolume);