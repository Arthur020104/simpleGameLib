#include <material.h>

void Material::bind(Program* shaderProgram, char* arrayName, uint16_t index, uint16_t texUnit)
{
  glUseProgram(shaderProgram->getProgram());//bind is happening in here, GameObject and Light draw

  char location[32];
  std::snprintf(location, sizeof(location), "%s[%u]", arrayName, index);

  char strBuffer[32];

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.diffuse", location);
  shaderProgram->bindVec3(strBuffer, this->diffuse);

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.specularColor", location);
  shaderProgram->bindVec3(strBuffer, this->specularColor);

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.shininess", location);
  shaderProgram->bindFloat(strBuffer, this->shininess);

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.hasDiffuseTex", location);
  shaderProgram->bindBool(strBuffer, this->diffuseTex != nullptr);

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.hasSpecularTex", location);
  shaderProgram->bindBool(strBuffer, this->specularTex != nullptr);

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.hasEmissiveTex", location);
  shaderProgram->bindBool(strBuffer, this->emissiveTex != nullptr);

  if(this->diffuseTex != nullptr)
  {
    std::snprintf(strBuffer, sizeof(strBuffer), "%s.diffuseTexUnit", location);
    this->diffuseTex->bind(shaderProgram, strBuffer, texUnit);
  }

  if(this->specularTex != nullptr)
  {
    std::snprintf(strBuffer, sizeof(strBuffer), "%s.specularTexUnit", location);
    this->specularTex->bind(shaderProgram, strBuffer, ++texUnit);
  }

  if(this->emissiveTex != nullptr)
  {
    std::snprintf(strBuffer, sizeof(strBuffer), "%s.emissiveTexUnit", location);
    this->emissiveTex->bind(shaderProgram, strBuffer, ++texUnit);
  }
    
}

Material::Material(std::string diffuseTexturePath, glm::vec3 specularColor, float shininess): 
specularColor(specularColor), shininess(shininess) 
{
  this->addDiffuseTexture(diffuseTexturePath);
}

Material::Material(std::shared_ptr<Texture> diffuseTex, glm::vec3 specularColor, float shininess): 
specularColor(specularColor), shininess(shininess) 
{
  this->addDiffuseTexture(diffuseTex);
}

void Material::addDiffuseTexture(std::shared_ptr<Texture> diffuseTex)
{
  this->activeTextures++;
  this->diffuseTex = diffuseTex;
}

void Material::addDiffuseTexture(std::string diffuseTexturePath)
{
  std::shared_ptr<Texture> diffuseTex = std::make_shared<Texture>(diffuseTexturePath);

  this->addDiffuseTexture(diffuseTex);
}

void Material::addSpecularTexture(std::shared_ptr<Texture> specularTex)
{
  this->activeTextures++;
  this->specularTex = specularTex;
}

void Material::addSpecularTexture(std::string specularTexturePath)
{
  std::shared_ptr<Texture> specularTex = std::make_shared<Texture>(specularTexturePath);
  this->addSpecularTexture(specularTex);
}

void Material::addEmissiveTexture(std::shared_ptr<Texture> emissiveTex)
{
  this->activeTextures++;
  this->emissiveTex = emissiveTex;
}

void Material::addEmissiveTexture(std::string emissiveTexturePath)
{
  std::shared_ptr<Texture> emissiveTex = std::make_shared<Texture>(emissiveTexturePath);
  this->addEmissiveTexture(emissiveTex);
}