#pragma once
#include <material.h>
#include <gameObject.h>
#include <box3d/box3d.h>

class BaseMover: public GameObject
{
  public:
    using GameObject::GameObject;

    void start() override;
    void afterUpdate() override {};
    void beforeUpdate() override;
    // void fixedUpdate() override {};
    Component* player = nullptr;

  private:
    
    float groundedRayDistance = 1.0f;
    bool isGrounded = false;

    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    float maxSpeed = 5000.0f;
    float acceleration = 1000.0f;
    float drag = 0.9999f;
};