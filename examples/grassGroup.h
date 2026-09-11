#pragma once
#include <instanceGroup.h>
#include <random>

class GrassInstance: public GameObject
{
  public:
    using GameObject::GameObject;
    void start() override {};
    void beforeUpdate() override {};
    void afterUpdate() override {};

    ~GrassInstance() {};
};

class GrassGroup : public InstanceGroup
{
  public:
    GrassGroup(const std::vector<GameObject*>& objs);
    GrassGroup(std::shared_ptr<Program> shader, std::shared_ptr<Material> material, glm::vec3 basePosition, uint16_t grassAmountX, uint16_t grassAmountZ, float minXDistance, float minZDistance);

    void draw(Camera* cam) override;
    void reducedUpdate() override;

    float baseY = 0.0f;
    glm::vec3 windDirection = glm::vec3(1.0f, 0.0f, 0.0f);
    float windStrength = 1.0f;
    float grassStemSizeY = 0.5f;

    glm::vec3 center;

    bool canDraw = false;
  private:
    static std::shared_ptr<Program> grassShader;
    static std::shared_ptr<Mesh> lods[2];
    static float maxDistanceToUseHighQualityGrass;
    static float stopRenderingDistance;
    static float loadBackToGPUDistance;
    static float removeFromGPUDistance;
    static float removeFromRAMDistance;
    static float minDistanceForOclusion;

    static uint32_t maxMemoryUntilFreeRAM;
    static uint32_t freeMemory;

    void passDataToGPU() override;

    std::shared_ptr<Material> material;
    glm::vec3 basePosition = glm::vec3(0.0f, 0.0f, 0.0f);
    uint16_t grassAmountX, grassAmountZ;
    float minXDistance, minZDistance;

    std::random_device rd;
    std::mt19937 gen;
    uint32_t seed;

    std::shared_ptr<Mesh> groupLods[2] = {nullptr, nullptr};

    static std::shared_ptr<Program> getGrassShader();
    static std::shared_ptr<Mesh>* getLods();

    bool hasDataInRAM = false;
    void loadDataToRAM();
    void cleanRAMData();

    float getRandomFloatR(float min, float max);
};