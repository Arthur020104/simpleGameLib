#pragma once
#include <mesh.h>
#include <program.h>
#include <transform.h>
#include <camera.h>
#include <ray.h>

class Mesh; 
class Program;
class Camera;

class GameObject: public Transform
{
  private:
    Mesh* mesh;
    Program* shaderProgram;
  public:
    GameObject(Mesh* meshData, Program* shader): mesh(meshData), shaderProgram(shader) {};

    ~GameObject();

    const Mesh* getMesh();

    const Program* getShaderProgram();

    void draw(cy::Matrix4f &projection, cy::Matrix4f &view);

    void draw(cy::Matrix4f &viewProjection);

    void draw(Camera camera);

    bool intersect(Ray& ray);

    bool isSelected = false;
};