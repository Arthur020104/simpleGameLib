#include <meshBvhNode.h>
#include <vector>
#include <algorithm>

void MeshBvhNode::init(std::vector<Vertex>& items, glm::vec3 boundingVolume[2], uint32_t depth)
{
  this->depth = depth;
  this->isLeaf = false;

  if(this->depth == 0)
  {
    this->boundingVolume[0] = glm::vec3(INFINITY, INFINITY, INFINITY);
    this->boundingVolume[1] = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
    for(Vertex& item: items)
    {
      alterBoudingMin(item.pos, this->boundingVolume[0]);
      alterBoudingMax(item.pos, this->boundingVolume[1]);
    }
  } 
  else
  {
    std::copy(boundingVolume, boundingVolume + 2, this->boundingVolume);
    
    if(items.size() <= TARGET_ITEMS_PER_LEAF || this->depth >= MAX_DEPTH) 
    {
      this->items = items;
      this->isLeaf = true;
      return;
    }
  }

  glm::vec3 firstChildBoundingVolume[2] = {this->boundingVolume[0], getNewPoint(true)};
  glm::vec3 secondChildboundingVolume[2] = {getNewPoint(false), this->boundingVolume[1]};
  
  std::vector<Vertex> firstChildItems;
  std::vector<Vertex> secondChildItems;

  //Spliting the items into two bvh children based on the bounding volume
  for(uint32_t i = 0; i < items.size(); i += 3)
  {
    glm::vec3 traingleCenterPoint = (items[i].pos + items[i+1].pos + items[i+2].pos) * 0.333333f;
    
    if(insideBoundingVolume(traingleCenterPoint, firstChildBoundingVolume))
    {
      firstChildItems.push_back(items[i]);
      firstChildItems.push_back(items[i+1]);
      firstChildItems.push_back(items[i+2]);
    }
    else
    {
      secondChildItems.push_back(items[i]);
      secondChildItems.push_back(items[i+1]);
      secondChildItems.push_back(items[i+2]);
    }
  }

  uint32_t childrenDepth = this->depth + 1;
  children[0] = firstChildItems.size() > 0 ? new MeshBvhNode(firstChildItems, firstChildBoundingVolume, childrenDepth) : nullptr;
  children[1] = secondChildItems.size() > 0 ? new MeshBvhNode(secondChildItems, secondChildboundingVolume, childrenDepth) : nullptr;

}

MeshBvhNode::MeshBvhNode(std::vector<Vertex>& items, uint32_t depth )
{
  glm::vec3 positiveInfinity = glm::vec3(INFINITY, INFINITY, INFINITY);
  glm::vec3 negativeInfinity = glm::vec3(-INFINITY, -INFINITY, -INFINITY);
  glm::vec3 boundingVolume[2] = { positiveInfinity, negativeInfinity };

  init(items, boundingVolume, depth);
}

MeshBvhNode::MeshBvhNode(std::vector<Vertex>& items, glm::vec3 boundingVolume[2], uint32_t depth)
{
  init(items, boundingVolume, depth);
}

MeshBvhNode::~MeshBvhNode()
{
  if(children[0] != nullptr) delete children[0];

  if(children[1] != nullptr) delete children[1];
}

bool MeshBvhNode::intersect(Ray& ray, GameObject* gameObject)
{
  bool hit = intersectBoundingVolume(ray, this->boundingVolume);
  if(!hit) return false;
  
  if(!this->isLeaf)
  {
    if(children[0] != nullptr && children[0]->intersect(ray, gameObject))
      return true;

    if(children[1] != nullptr && children[1]->intersect(ray, gameObject))
      return true;
    
    return false;
  }

  for(uint32_t i = 0; i < items.size(); i += 3)
  {
    if(intersectTriangle(ray, gameObject, items[i], items[i + 1], items[i + 2]))
      return true;
      
  }
  
  return false;
}

glm::vec3 MeshBvhNode::getNewPoint(bool firstChild)
{
  glm::vec3 baseVec = firstChild ? this->boundingVolume[1] : this->boundingVolume[0];
  switch (depth % 3)
  {
  case 0:
    baseVec.z = (this->boundingVolume[0].z + (this->boundingVolume[1].z - this->boundingVolume[0].z) * 0.5f);
    return baseVec;
    break;
  case 1:
    baseVec.y = (this->boundingVolume[0].y + (this->boundingVolume[1].y - this->boundingVolume[0].y) * 0.5f);
    return baseVec;
    break;
  case 2:
    baseVec.x = (this->boundingVolume[0].x + (this->boundingVolume[1].x - this->boundingVolume[0].x) * 0.5f);
    return baseVec;
    break;
  }

  return glm::vec3();
}

bool insideBoundingVolume(glm::vec3& point, glm::vec3* boundingVolume)
{
  if(point.x < boundingVolume[0].x) return false;
  if(point.y < boundingVolume[0].y) return false;
  if(point.z < boundingVolume[0].z) return false;

  if(point.x > boundingVolume[1].x) return false;
  if(point.y > boundingVolume[1].y) return false;
  if(point.z > boundingVolume[1].z) return false;

  return true;
}

void alterBoudingMin(glm::vec3& newVec, glm::vec3& baseVec)
{
  baseVec.x = newVec.x < baseVec.x ? newVec.x : baseVec.x;

  baseVec.y = newVec.y < baseVec.y ? newVec.y : baseVec.y;

  baseVec.z = newVec.z < baseVec.z ? newVec.z : baseVec.z;
}

void alterBoudingMax(glm::vec3& newVec, glm::vec3& baseVec)
{
 baseVec.x = newVec.x > baseVec.x ? newVec.x : baseVec.x;

  baseVec.y = newVec.y > baseVec.y ? newVec.y : baseVec.y;

  baseVec.z = newVec.z > baseVec.z ? newVec.z : baseVec.z;
}
