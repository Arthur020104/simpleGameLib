#pragma once
#include <string>
#include <cstdint>
#include <program.h>

class Texture
{
  public:
    Texture(
      std::string path, 
      GLenum wrapS = GL_MIRRORED_REPEAT, GLenum wrapT = GL_MIRRORED_REPEAT, 
      GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR, GLenum magFilter = GL_LINEAR
    );

    uint32_t loadDataToGPU();

    void bind(Program* shader, std::string uniformName, uint8_t texUnit);
    void bind(Program* shader, const char* uniformName, uint8_t texUnit);

    uint32_t getId() {return id;};

    GLenum wrapS, wrapT, minFilter, magFilter;
    std::string path;
  private:
    uint32_t id;
};