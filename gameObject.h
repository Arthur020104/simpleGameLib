#pragma once
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
    GameObject(std::shared_ptr<Mesh> meshData, std::shared_ptr<Program> shader);

    virtual ~GameObject();

    const std::shared_ptr<Mesh> getMesh();

    const std::shared_ptr<Program> getShaderProgram();

    virtual void draw(cy::Matrix4f &projection, cy::Matrix4f &view);

    void draw(cy::Matrix4f &viewProjection);

    void draw(Camera* camera);

    bool intersect(Ray& ray);

    bool isSelected = false;

    virtual void start() = 0;
    virtual void beforeUpdate() = 0;
    virtual void aftherUpdate() = 0;
  private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Program> shaderProgram;
};