#pragma once
#include <string>
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
    GLuint id;
    bool alreadyLoaded;
    GLenum wrapS, wrapT, minFilter, magFilter, textureFormat;
    uint16_t width, height;

    void loadDataToGPU(unsigned char* textureData);
};

namespace TEXTURE
{
  //To decrease usage of dynamic strings, this will be used to bind textures to shader.
  static const char* TEXTURE_UNIFORMS[] = {
    "textures[0]", "textures[1]", "textures[2]", "textures[3]",
    "textures[4]", "textures[5]", "textures[6]", "textures[7]",
    "textures[8]", "textures[9]", "textures[10]", "textures[11]",
    "textures[12]", "textures[13]", "textures[14]", "textures[15]",
    "textures[16]", "textures[17]", "textures[18]", "textures[19]",
    "textures[20]", "textures[21]", "textures[22]", "textures[23]",
    "textures[24]", "textures[25]", "textures[26]", "textures[27]",
    "textures[28]", "textures[29]", "textures[30]", "textures[31]",
    "textures[32]", "textures[33]", "textures[34]", "textures[35]",
    "textures[36]", "textures[37]", "textures[38]", "textures[39]",
    "textures[40]", "textures[41]", "textures[42]", "textures[43]",
    "textures[44]", "textures[45]", "textures[46]", "textures[47]",
    "textures[48]", "textures[49]", "textures[50]", "textures[51]",
    "textures[52]", "textures[53]", "textures[54]", "textures[55]",
    "textures[56]", "textures[57]", "textures[58]", "textures[59]",
    "textures[60]", "textures[61]", "textures[62]", "textures[63]"
  };
}