#pragma once
#include <ray.h>
#include <vertex.h>

class GameObject;
struct Ray;

class MeshBvhNode
{
  public:
    MeshBvhNode(std::vector<Vertex>& items, uint32_t depth);
    MeshBvhNode(std::vector<Vertex>& items, cy::Vec3f boundingVolume[2], uint32_t depth);

    ~MeshBvhNode();

    std::vector<Vertex> items;
    MeshBvhNode* children[2] = {nullptr, nullptr};

    cy::Vec3f boundingVolume[2];

    uint32_t depth;
    bool isLeaf;

    bool intersect(Ray& ray, GameObject* gameObject);
  
    private:  
    //for destructor delete two children and all items, items just will be filled for leaf nodes

    void init(std::vector<Vertex>& items, cy::Vec3f boundingVolume[2], uint32_t depth);
    
    cy::Vec3f getNewPoint(bool firstChild = true);


    GameObject* object = nullptr;

    static const uint8_t TARGET_ITEMS_PER_LEAF = 5;

    static const uint8_t MAX_DEPTH = 15;
};

void alterBoudingMin(cy::Vec3f& newVec, cy::Vec3f& baseVec);

void alterBoudingMax(cy::Vec3f& newVec, cy::Vec3f& baseVec);

bool insideBoundingVolume(cy::Vec3f& point, cy::Vec3f* boundingVolume);