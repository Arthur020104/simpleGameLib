#pragma once
#include <ray.h>
#include <memory>
#include <mesh.h>
#include <scene.h>
#include <camera.h>
#include <program.h>
#include <material.h>
#include <component.h>
#include <cy/cyTriMesh.h>
#include <Libs/box3d/include/box3d/box3d.h>

class Mesh; 
class Program;
class Camera;
class Scene;

enum class GameObjectType {
  STATIC,
  DYNAMIC
};

enum class PhysicalShapeType {
  NONE,
  CUBE
};

class GameObject: public Component
{
  public:
    GameObject(std::shared_ptr<Mesh> meshData, std::shared_ptr<Program> shader, std::vector<std::shared_ptr<Material>> material = {Material::getDefaultMaterial()}, GameObjectType gameObjectType = GameObjectType::STATIC);
    
    GameObject(std::string path, std::shared_ptr<Program> shader, GameObjectType gameObjectType = GameObjectType::STATIC);

    GameObject(const char* path, std::shared_ptr<Program> shader, GameObjectType gameObjectType = GameObjectType::STATIC);

    GameObject(cy::TriMesh& objTriMesh, std::shared_ptr<Program> shader, GameObjectType gameObjectType = GameObjectType::STATIC);

    virtual ~GameObject();

    const std::shared_ptr<Mesh> getMesh();

    const std::shared_ptr<Program> getShaderProgram();

    virtual void draw(glm::mat4 &projection, glm::mat4 &view);

    virtual void draw(glm::mat4 &viewProjection);

    virtual void draw(Camera* camera);

    bool intersect(Ray& ray);

    void useOnly(std::shared_ptr<Material> material);

    void useOnly(uint8_t materialIndex);

    void addMaterial(std::shared_ptr<Material> material);

    void addMaterial(std::shared_ptr<Material> material, uint32_t startIdx, uint32_t endIdx);

    void useMaterial(std::shared_ptr<Material> material, uint32_t startIdx, uint32_t endIdx);

    uint32_t getMaterialIndicesSize() {return materialIndices.size();};

    uint8_t getMaterialsSize() {return materials.size();};

    std::vector<std::shared_ptr<Material>> getMaterials() {return this->materials;};

    void setMaterialIndices(std::vector<uint8_t> materialIndices);

    virtual void start() = 0;
    virtual void beforeUpdate() override;
    virtual void afterUpdate() = 0;

    virtual void setPosition(glm::vec3 pos) override;
    virtual void setRotation(glm::vec3 rot) override;
    virtual void setScale(glm::vec3 scale) override;

    bool isIntersectable = false;

    b3BodyId getBodyId() { return this->bodyId; }

    void createPhysicalBody(PhysicalShapeType physicalShapeType, float density = 1.0f, float friction = 0.3f);
  private:

    GLuint materialIndicesVBO;
    bool hasMaterialVBO = false;

    std::vector<uint8_t> materialIndices;
  
    GameObjectType gameObjectType = GameObjectType::STATIC;
    PhysicalShapeType physicalShapeType = PhysicalShapeType::NONE;

    void loadGameObjectFromPath(cy::TriMesh& objTriMesh, std::shared_ptr<Program> shader);
    void loadMaterialIndicesToGPU();
  protected:
    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Program> shaderProgram;

    bool hasPhysicalBody = false;

    b3BodyId bodyId;

    GameObject(): Component() {};
};