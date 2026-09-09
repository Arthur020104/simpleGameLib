#pragma once
#include <instanceGroup.h>
class GrassGroup : public InstanceGroup
{
  public:
    GrassGroup(const std::vector<GameObject*>& objs);

    using InstanceGroup::InstanceGroup;
    void draw(Camera* cam, Scene* scene) override;
    void update();

    float baseY = 0.0f;
    glm::vec3 windDirection = glm::vec3(1.0f, 0.0f, 0.0f);
    float windStrength = 1.0f;
    float grassStemSizeY = 0.5f;

    glm::vec3 center;

    bool canDraw = true;
  private:
    static std::shared_ptr<Program> grassShader;
    static std::shared_ptr<Mesh> lods[2];
    static float maxDistanceToUseHighQualityGrass;
    static float stopRenderingDistance;
    static float removeFromGPUDistance;

    std::shared_ptr<Mesh> groupLods[2] = {nullptr, nullptr};

    static std::shared_ptr<Program> getGrassShader();
    static std::shared_ptr<Mesh>* getLods();
};