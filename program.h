#pragma once
#include <unordered_map>
#include <GL/glew.h>
#include <string.h>
#include <cy/cyTriMesh.h>
#include <cy/cyMatrix.h>
#include <memory>

class GameObject;

class Program
{
  public:
    Program(const char* vertexShaderPath, const char* fragmentShaderPath);
    ~Program();
    void compileShaders();
    unsigned short getProgram();

    void registerObjectUsingProgram(GameObject* obj);//for now passing the obj is useless, but later I may want to add a list of objects using this program
    uint16_t getUsingProgram();
    uint16_t removeUsingProgram(GameObject* obj);

    void bindFloat(const char* uniformName, float value);
    void bindVec3(const char* uniformName, cy::Vec3f value);
    void bindVec4(const char* uniformName, cy::Vec4f value);
    void bindMat4(const char* uniformName, cy::Matrix4f value);
    void bindInt(const char* uniformName, int value);
    void bindBool(const char* uniformName, bool value);
    void bindUint(const char* uniformName, unsigned int value);

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

extern std::shared_ptr<Program> DEFAULT_SHADER;