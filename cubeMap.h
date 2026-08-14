#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>

class Camera;
class Program;
class Mesh;

class CubeMap
{
  public:
    CubeMap(std::vector<std::string> facesPath);
    ~CubeMap();

    void draw(Camera* cam);

    uint32_t getId() { return this->id; };

  private:
    std::vector<std::string> facesPath;
    Mesh* mesh;
    Program* shaderProgram;
    uint32_t id;

    void loadTexture(std::string path, GLenum cubeMapTarget);
};
