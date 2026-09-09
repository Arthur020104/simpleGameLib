#pragma once
#include <set>
#include <mesh.h>
#include <vector>
#include <camera.h>
#include <program.h>
#include <gameObject.h>
#include <material.h>
#include <gameObjectType.h>
#include <Libs/box3d/include/box3d/box3d.h>


class InstanceGroup
{
  public:
    InstanceGroup(std::shared_ptr<Mesh> mesh, bool objectsOnScene, std::shared_ptr<Program> program, std::vector<GameObject*> objs, bool passDataToGPU = true);
    InstanceGroup(std::shared_ptr<Mesh> mesh, bool objectsOnScene, std::shared_ptr<Program> program = std::make_shared<Program>("../shaders/instanced.vs", "../shaders/instanced.fs"), std::set<GameObject*> objs = {}, bool passDataToGPU = true);

    ~InstanceGroup();

    void init(std::shared_ptr<Mesh> mesh, bool objectsOnScene, std::shared_ptr<Program> program, std::set<GameObject*> objs, bool passDataToGPU);

    virtual void draw(Camera* cam, Scene* scene);

    virtual void removeObject(GameObject* obj);
    virtual void addObject(GameObject* obj);

    virtual void passDataToGPU();
    virtual void bindForDrawing();

    virtual void removeDataFromGPU();

    bool objectsOnScene = false;

    Scene* scene = nullptr;
  protected:
    std::shared_ptr<Mesh> mesh;
    std::set<GameObject*> objs;
    std::shared_ptr<Program> program;
    GameObjectType gameObjectType = GameObjectType::STATIC;

    bool hasMutipleMaterialsPerObject = false;

    std::unordered_map<Material*, uint8_t> materialToIndexMap;

    GLuint VBO, MaterialVbo;
    bool hasVbo = false;

};

struct objData
{
  glm::vec4 modelMatrixRow0;
  glm::vec4 modelMatrixRow1;
  glm::vec4 modelMatrixRow2;
  glm::vec4 modelMatrixRow3;
};