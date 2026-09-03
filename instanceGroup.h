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
    InstanceGroup(std::shared_ptr<Mesh> mesh, std::shared_ptr<Program> program, std::vector<GameObject*> objs);
    InstanceGroup(std::shared_ptr<Mesh> mesh, std::shared_ptr<Program> program, std::set<GameObject*> objs = {});

    ~InstanceGroup();

    void init(std::shared_ptr<Mesh> mesh, std::shared_ptr<Program> program, std::set<GameObject*> objs);

    void draw(Camera* cam, Scene* scene);

    void removeObject(GameObject* obj);
    void addObject(GameObject* obj);

    void loadVBO();
    void bindForDrawing();
  private:
    std::shared_ptr<Mesh> mesh;
    std::set<GameObject*> objs;
    std::shared_ptr<Program> program;
    GameObjectType gameObjectType = GameObjectType::STATIC;

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