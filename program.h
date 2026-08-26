#pragma once
#include <string>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>

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
    void bindVec3(const char* uniformName, glm::vec3 value);
    void bindVec4(const char* uniformName, glm::vec4 value);
    void bindMat4(const char* uniformName, glm::mat4 value);
    void bindInt(const char* uniformName, int value);
    void bindBool(const char* uniformName, bool value);
    void bindUint(const char* uniformName, unsigned int value);

    static std::shared_ptr<Program> getDefaultShader();

  private:
    GLuint id;
    std::string vertexShaderPath, fragmentShaderPath;
    const std::unordered_map<const char*, GLint> SHADER_TYPES = {
      {"FRAGMENT", GL_FRAGMENT_SHADER},
      {"VERTEX", GL_VERTEX_SHADER}
    };

    unsigned short prepareShader(const char* shaderCode,const char* type);
    void checkCompileErrors(unsigned short id,const char* type);
    void prepareProgram(unsigned short* shaders, unsigned short size);
    void destroyProgram();
    static std::shared_ptr<Program> createDefaultShader();


    uint16_t objectsUsingProgram;
};
