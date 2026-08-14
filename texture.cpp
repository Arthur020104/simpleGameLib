#include <texture.h>
#include <Libs/stb/stb_image.h>

Texture::Texture(std::string path, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter):
path(path), wrapS(wrapS), wrapT(wrapT), minFilter(minFilter), magFilter(magFilter)
{
  this->id = loadDataToGPU();
}

Texture::~Texture()
{
  glDeleteTextures(1, &this->id);
}

uint32_t Texture::loadDataToGPU()
{
  stbi_set_flip_vertically_on_load(true);  

  int width, height, nrChannels;
  unsigned char *textureData = stbi_load(this->path.c_str(), &width, &height, &nrChannels, 0);

  if(!textureData)
    throw std::runtime_error(std::string("Failed to load texture: ") + this->path);

  GLenum textureFormat;

  switch (nrChannels)
  {
    case 1:
      textureFormat = GL_RED;
      break;
    case 2:
      textureFormat = GL_RG;
      break;
    case 3:
      textureFormat = GL_RGB;
      break;
    case 4:
      textureFormat = GL_RGBA;
      break;
    default:
      throw std::runtime_error("Unsupported number of channels in texture image");
      break;
  }

  uint32_t textureId;
  glGenTextures(1, &textureId);

  glBindTexture(GL_TEXTURE_2D, textureId); 

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter);
  
  glTexImage2D(
    GL_TEXTURE_2D, 
    0, 
    textureFormat, 
    width, 
    height, 
    0, 
    textureFormat, 
    GL_UNSIGNED_BYTE, 
    textureData
  );
  
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(textureData);

  return textureId;
}

void Texture::bind(Program* shader, std::string uniformName, uint8_t texUnit)
{
  this->bind(shader, uniformName.c_str(), texUnit);
}

void Texture::bind(Program* shader, const char* uniformName, uint8_t texUnit)
{
  glActiveTexture(GL_TEXTURE0 + texUnit); 
  glBindTexture(GL_TEXTURE_2D, this->id);

  shader->bindInt(("textures[" + std::to_string(texUnit) + "]").c_str(), texUnit);
  shader->bindInt(uniformName, texUnit);
}