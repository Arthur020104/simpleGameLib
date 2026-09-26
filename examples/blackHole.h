#pragma once
#include <gameObject.h>

class BlackHole: public GameObject
{
  public:
    BlackHole(glm::vec3 position, glm::vec3 direction, float scale = 1.0f, float scaleSpeed = 0.1f, float rotationSpeed = 0.5f, float pullForce = 100.0f, float moveSpeed = 1.0f);

    void start() override {};
    void afterUpdate() override;
    void beforeUpdate() override {};

    void setScale(glm::vec3 scale) override;
    void setScale(float scale);

    virtual ~BlackHole() {};
  private:
    float scale = 5.0f;
    float scaleSpeed = 0.1f;
    float rotationSpeed = 0.5f;
    float pullForce = 10.0f;
    float moveSpeed = 1.0f;
    float radius = 1.0f;

    static float minDistanceToDestroy;
    static float linearFallOff;
    static float quadraticFallOff;
    static float constantFallOff;
    static float baseRadius;

    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);

    static std::shared_ptr<Mesh> blackHoleMesh;
    static std::shared_ptr<Program> blackHoleShader;

};