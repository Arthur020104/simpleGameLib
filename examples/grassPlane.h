#include <../gameObject.h>
#include <../mesh.h>
#include <grassGroup.h>

class GrassInstance: public GameObject
{
  public:
    using GameObject::GameObject;
    void start() override {};
    void beforeUpdate() override {};
    void afterUpdate() override {};
};

class GrassPlane : public GameObject
{
  public:
    GrassPlane(glm::vec3 position);

    void start() override;
    void afterUpdate() override {};
    void fixedUpdate() override;
    void beforeUpdate() override {};
  private:
    std::shared_ptr<Mesh> grassMesh;
    std::shared_ptr<Material> grassMaterial;

    glm::vec3 basePosition = glm::vec3(0.0f, 0.0f, 0.0f);

    float minXDistance = 0.5f, minZDistance = 0.5f;
    float chunkSizeX = 50.0f, chunkSizeZ = 50.0f;

    GrassGroup* generateGrassInstance(glm::vec3 basePosition, uint16_t& grassAmountX, uint16_t& grassAmountZ);

    bool onAction = false;
};