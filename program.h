#pragma once
#include <unordered_map>
#include <GL/glew.h>
#include <string.h>
#include <cy/cyTriMesh.h>
#include <cy/cyMatrix.h>

class Program
{
  public:
    Program(const char* vertexShaderPath, const char* fragmentShaderPath);
    ~Program();
    void compileShaders();
    unsigned short getProgram();

    void registerObjectUsingProgram(GameObject* obj);//for now passing the obj is useless, but later I may want to add a list of objects using this program
    uint16_t getUsingProgram();
    u_int16_t removeUsingProgram(GameObject* obj);

  private:
    unsigned short id;
    std::string vertexShaderPath, fragmentShaderPath;
    const std::unordered_map<const char*, GLint> SHADER_TYPES = {
      {"FRAGMENT", GL_FRAGMENT_SHADER},
      {"VERTEX", GL_VERTEX_SHADER}
    };

    unsigned short prepareShader(const char* shaderCode,const char* type);
    void checkCompileErrors(unsigned short id,const char* type);
    void prepareProgram(unsigned short* shaders, unsigned short size);
    void destroyProgram();


    uint16_t objectsUsingProgram;
};
