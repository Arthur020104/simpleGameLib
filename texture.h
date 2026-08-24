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

    Texture(
      uint16_t width, uint16_t height, GLenum textureFormat, unsigned char* data,
      GLenum wrapS = GL_MIRRORED_REPEAT, GLenum wrapT = GL_MIRRORED_REPEAT, 
      GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR, GLenum magFilter = GL_LINEAR
    );
    
    ~Texture();

    void loadDataFromPath();

    void bind(Program* shader, std::string uniformName, uint16_t texUnit);
    void bind(Program* shader, const char* uniformName, uint16_t texUnit);

    void bind(Program* shader, uint16_t texUnit);

    uint32_t getId() {return id;};

    void setFiltering(GLenum minFilter, GLenum magFilter);
    void setWrapping(GLenum wrapS, GLenum wrapT);
    void setResolution(uint16_t width, uint16_t height, bool loadFromPath = false);
    
    std::string path;
  private:
    uint32_t id;
    bool alreadyLoaded;
    GLenum wrapS, wrapT, minFilter, magFilter, textureFormat;
    uint16_t width, height;

    void loadDataToGPU(unsigned char* textureData);
};