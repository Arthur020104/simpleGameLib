#pragma once
#include <gameObject.h>
#include <mesh.h>
#include <grassGroup.h>

class GrassGroup;

class GrassPlane : public GameObject
{
  public:
    GrassPlane(glm::vec3 position);

    void start() override;
    void afterUpdate() override {};
    void beforeUpdate() override {};

    void draw(glm::mat4 &viewProjection) override;
  private:
    std::shared_ptr<Mesh> grassMesh;
    std::shared_ptr<Material> grassMaterial;

    glm::vec3 basePosition = glm::vec3(0.0f, 0.0f, 0.0f);

    float minXDistance = 0.5f, minZDistance = 0.5f;
    float chunkSizeX = 100.0f, chunkSizeZ = 100.0f;

    static std::shared_ptr<Program> planeShader;

    bool onAction = false;
};