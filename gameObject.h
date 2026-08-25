#pragma once
#include <material.h>
#include <mesh.h>
#include <program.h>
#include <component.h>
#include <camera.h>
#include <ray.h>
#include <scene.h>
#include <memory>

class Mesh; 
class Program;
class Camera;
class Scene;

class GameObject: public Component
{
  public:
    GameObject(std::shared_ptr<Mesh> meshData, std::shared_ptr<Program> shader, std::vector<std::shared_ptr<Material>> material = {DEFAULT_MATERIAL});
    
    GameObject(std::string path, std::shared_ptr<Program> shader);

    GameObject(const char* path, std::shared_ptr<Program> shader);

    GameObject(cy::TriMesh& objTriMesh, std::shared_ptr<Program> shader);

    virtual ~GameObject();

    const std::shared_ptr<Mesh> getMesh();

    const std::shared_ptr<Program> getShaderProgram();

    virtual void draw(cy::Matrix4f &projection, cy::Matrix4f &view);

    virtual void draw(cy::Matrix4f &viewProjection);

    virtual void draw(Camera* camera);

    bool intersect(Ray& ray);

    void useOnly(std::shared_ptr<Material> material);

    void useOnly(uint8_t materialIndex);

    void addMaterial(std::shared_ptr<Material> material);

    void addMaterial(std::shared_ptr<Material> material, uint32_t startIdx, uint32_t endIdx);

    void useMaterial(std::shared_ptr<Material> material, uint32_t startIdx, uint32_t endIdx);

    uint32_t getMaterialIndicesSize() {return materialIndices.size();};

    uint8_t getMaterialsSize() {return materials.size();};

    void setMaterialIndices(std::vector<uint8_t> materialIndices);

    virtual void start() = 0;
    virtual void beforeUpdate() = 0;
    virtual void afterUpdate() = 0;
  private:

    GLuint materialIndicesVBO;
    bool hasMaterialVBO = false;

    std::vector<uint8_t> materialIndices;

    void loadGameObjectFromPath(cy::TriMesh& objTriMesh, std::shared_ptr<Program> shader);
    void loadMaterialIndicesToGPU();
  protected:
    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Program> shaderProgram;

    GameObject(): Component() {};
};