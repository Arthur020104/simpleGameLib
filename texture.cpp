#include <texture.h>
#include <stdexcept>
#include <Libs/stb/stb_image.h>

Texture::Texture(std::string path, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter):
path(path), wrapS(wrapS), wrapT(wrapT), minFilter(minFilter), magFilter(magFilter)
{
  loadDataFromPath();
}

Texture::Texture(uint16_t width, uint16_t height, GLenum textureFormat, unsigned char* data, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter):
width(width), height(height), textureFormat(textureFormat), wrapS(wrapS), wrapT(wrapT), minFilter(minFilter), magFilter(magFilter)
{
  loadDataToGPU(data);
}

Texture::~Texture()
{
  glDeleteTextures(1, &this->id);
}

void Texture::setFiltering(GLenum minFilter, GLenum magFilter)
{
  if(!this->alreadyLoaded)
    throw std::runtime_error("Error: Cannot set filtering on a texture that has not been loaded yet.\n");
  
  glBindTexture(GL_TEXTURE_2D, this->id);

  this->minFilter = minFilter;
  this->magFilter = magFilter;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter);
}
void Texture::setWrapping(GLenum wrapS, GLenum wrapT)
{
  if(!this->alreadyLoaded)
    throw std::runtime_error("Error: Cannot set wrapping on a texture that has not been loaded yet.\n");

  glBindTexture(GL_TEXTURE_2D, this->id);

  this->wrapS = wrapS;
  this->wrapT = wrapT;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
}

void Texture::setResolution(uint16_t width, uint16_t height, bool loadFromPath)
{
  if(!this->alreadyLoaded)
    throw std::runtime_error("Error: Cannot set resolution on a texture that has not been loaded yet.\n");

  this->width = width;
  this->height = height;

  if(!loadFromPath)
  {
    this->loadDataToGPU(nullptr);
    return;
  }

  if(this->path.empty())
    throw std::runtime_error("Error: Cannot load texture from path when no path is set.\n");

  this->loadDataFromPath();
}

void Texture::loadDataFromPath()
{
  stbi_set_flip_vertically_on_load(true);  

  int width, height, nrChannels;
  unsigned char *textureData = stbi_load(this->path.c_str(), &width, &height, &nrChannels, 0);

  this->width = (uint16_t)width;
  this->height = (uint16_t)height;

  if(!textureData)
    throw std::runtime_error(std::string("Failed to load texture: ") + this->path);

  switch (nrChannels)
  {
    case 1:
      this->textureFormat = GL_RED;
      break;
    case 2:
      this->textureFormat = GL_RG;
      break;
    case 3:
      this->textureFormat = GL_RGB;
      break;
    case 4:
      this->textureFormat = GL_RGBA;
      break;
    default:
      throw std::runtime_error("Unsupported number of channels in texture image");
      break;
  }

  this->loadDataToGPU(textureData);

  stbi_image_free(textureData);

}

void Texture::loadDataToGPU(unsigned char* textureData)
{
  glGenTextures(1, &this->id);

  glBindTexture(GL_TEXTURE_2D, this->id); 

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter);
  
  glTexImage2D(
    GL_TEXTURE_2D, 
    0, 
    this->textureFormat, 
    this->width, 
    this->height, 
    0, 
    textureFormat, 
    GL_UNSIGNED_BYTE, 
    textureData
  );
  
  glGenerateMipmap(GL_TEXTURE_2D);

  this->alreadyLoaded = true;
}

void Texture::bind(Program* shader, std::string uniformName, uint16_t texUnit)
{
  this->bind(shader, uniformName.c_str(), texUnit);
}

void Texture::bind(Program* shader, const char* uniformName, uint16_t texUnit)
{
  glActiveTexture(GL_TEXTURE0 + texUnit); 
  glBindTexture(GL_TEXTURE_2D, this->id);
  
  shader->bindInt(TEXTURE::TEXTURE_UNIFORMS[texUnit], texUnit);
  shader->bindInt(uniformName, texUnit);
}

void Texture::bind(Program* shader, uint16_t texUnit)
{
  glActiveTexture(GL_TEXTURE0 + texUnit); 
  glBindTexture(GL_TEXTURE_2D, this->id);

  shader->bindInt(TEXTURE::TEXTURE_UNIFORMS[texUnit], texUnit);
}