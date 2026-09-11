#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 

class Transform
{
  public:
    Transform(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

    void updateModelMatrix();
    const glm::mat4& getModelMatrix();
    const glm::mat4& getRotationMatrix();
    const glm::mat4& getInvertedTransposedModelMatrix();

    virtual void setPosition(glm::vec3 pos);
    virtual void setRotation(glm::vec3 rot);
    virtual void setRotation(glm::quat rot);
    virtual void setScale(glm::vec3 scale);

    virtual glm::vec3 getPosition() { return this->position; }
    virtual glm::vec3 getRotation();
    virtual glm::quat getRotationQuat() { return this->rotation; }
    virtual glm::vec3 getScale() { return this->scale; }
    virtual glm::vec3 getNormalizedPosition() { return glm::normalize(this->position); }

    virtual glm::vec3 getForwardVector();
    virtual glm::vec3 getRightVector();
    virtual glm::vec3 getUpVector();

  private:
    glm::mat4 modelMatrix, rotationMatrix;
    glm::mat4 invertedTransposedModelMatrix;

    glm::vec3 position, scale, eulerRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat rotation;
};