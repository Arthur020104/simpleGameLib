#include <cubeMap.h>
#include <mesh.h>
#include <camera.h>
#include <program.h>
#define STB_IMAGE_IMPLEMENTATION
#include <Libs/stb/stb_image.h>

CubeMap::CubeMap(std::vector<std::string> facesPath): facesPath(facesPath)
{
  if(facesPath.size() != 6)
    throw std::runtime_error("CubeMap requires exactly 6 texture paths.");
    
  glGenTextures(1, &this->id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
  
  for(uint8_t i = 0; i < 6; i++)
  {
    loadTexture(facesPath[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
  }

  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  float defaultZ = 0.9999999f;
  Vertex v1, v2, v3;
  v1.pos = glm::vec3(-1.0f, -1.0f, defaultZ); v1.normal = glm::vec3(0.0f, 0.0f, 0.0f);
  v2.pos = glm::vec3(3.0f, -1.0f, defaultZ); v2.normal = glm::vec3(0.0f, 0.0f, 0.0f);
  v3.pos = glm::vec3(-1.0f, 3.0f, defaultZ); v3.normal = glm::vec3(0.0f, 0.0f, 0.0f);
  
  this->mesh = new Mesh({v1, v2, v3}, MeshType::TRIANGLE_MESH);
  this->shaderProgram = new Program("/home/arthur/Documents/simpleGame/shaders/cubeMap.vs", "/home/arthur/Documents/simpleGame/shaders/cubeMap.fs");
}

CubeMap::~CubeMap()
{
  delete this->mesh;
  delete this->shaderProgram;
  glDeleteTextures(1, &this->id);
}

void CubeMap::draw(Camera* cam)
{
  glDepthMask(GL_FALSE);

  const uint16_t shaderID = this->shaderProgram->getProgram();
  glUseProgram(shaderID);

  glActiveTexture(GL_TEXTURE0); 
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

  shaderProgram->bindMat4("invViewProjection", cam->getInvertedViewProjection());
  shaderProgram->bindInt("environmentTexture", 0);

  this->mesh->bindVAO();
  this->mesh->renderMesh();

  glDepthMask(GL_TRUE);
}

void CubeMap::loadTexture(std::string path, GLenum cubeMapTarget)
{
  stbi_set_flip_vertically_on_load(false);  

  int width, height, nrChannels;
  unsigned char *textureData = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  if(!textureData)
    throw std::runtime_error(std::string("Failed to load texture: ") + path);

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
  
  glTexImage2D(
    cubeMapTarget, 
    0, 
    textureFormat, 
    width, 
    height, 
    0, 
    textureFormat, 
    GL_UNSIGNED_BYTE, 
    textureData
  );

  stbi_image_free(textureData);
}