#pragma once
#include <mesh.h>
#include <program.h>
#include <transform.h>
#include <camera.h>
#include <ray.h>
#include <scene.h>

class Mesh; 
class Program;
class Camera;
class Scene;

class GameObject: public Transform
{
  public:
    GameObject(Mesh* meshData, Program* shader);

    virtual ~GameObject();

    const Mesh* getMesh();

    const Program* getShaderProgram();

    void draw(cy::Matrix4f &projection, cy::Matrix4f &view);

    void draw(cy::Matrix4f &viewProjection);

    void draw(Camera* camera);

    bool intersect(Ray& ray);

    bool isSelected = false;

    Scene* scene;

    virtual void start() = 0;
    virtual void beforeDrawing() = 0;
    virtual void aftherDrawing() = 0;
  private:
    Mesh* mesh;
    Program* shaderProgram;
};