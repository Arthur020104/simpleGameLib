#pragma once
#include <material.h>
#include <gameObject.h>
#include <box3d/box3d.h>

class MovementController: public GameObject
{
  public:
    MovementController(glm::vec3 position = glm::vec3(0.0f, 15.0f, 0.0f));

    void start() override;
    void afterUpdate() override {};
    void beforeUpdate() override;
    void fixedUpdate() override {};

    void setScale(glm::vec3 scale) override;
  protected:
    bool isGrounded = false;

    float groundedRayDistance = 1.5f;
    float jumpForce = 15.0f;
    float maxSpeed = 0.0f;
    float acceleration = 80.0f;
    float inAirAcceleration = 20.0f;
    float currentAcceleration = 0.0f;
    float currentDrag = 0.0f;
    float inAirDrag = 0.1f;
    float drag = 4.0f;
    float unscaledHalfHeight = 0.25f;

    static float minValueToMove;
    
    b3Capsule mover;
    glm::vec3 velocity = glm::vec3(0.0f, -10.0f, 0.0f);
    
    void updateMoverPosition();
};