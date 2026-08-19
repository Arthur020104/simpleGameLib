#include <material.h>

void Material::bind(Program* shaderProgram, char* arrayName, uint16_t index, uint8_t texUnit)
{
  glUseProgram(shaderProgram->getProgram());//bind is happening in here, GameObject and Light draw

  std::string location = std::string(arrayName) + "[" + std::to_string(index) + "]";

  shaderProgram->bindVec3((location + ".diffuse").c_str(), this->diffuse);
  shaderProgram->bindVec3((location + ".specularColor").c_str(), this->specularColor);

  shaderProgram->bindFloat((location + ".shininess").c_str(), this->shininess);

  shaderProgram->bindBool((location + ".hasDiffuseTex").c_str(), this->diffuseTex != nullptr);
  shaderProgram->bindBool((location + ".hasSpecularTex").c_str(), this->specularTex != nullptr);
  shaderProgram->bindBool((location + ".hasEmissiveTex").c_str(), this->emissiveTex != nullptr);

  if(this->diffuseTex != nullptr)
    this->diffuseTex->bind(shaderProgram, (location + ".diffuseTexUnit").c_str(), texUnit);

  if(this->specularTex != nullptr)
    this->specularTex->bind(shaderProgram, (location + ".specularTexUnit").c_str(), ++texUnit);

  if(this->emissiveTex != nullptr)
    this->emissiveTex->bind(shaderProgram, (location + ".emissiveTexUnit").c_str(), ++texUnit);
}

Material::Material(std::string diffuseTexturePath, cy::Vec3f specularColor, float shininess): 
specularColor(specularColor), shininess(shininess) 
{
  this->addDiffuseTexture(diffuseTexturePath);
}

Material::Material(std::shared_ptr<Texture> diffuseTex, cy::Vec3f specularColor, float shininess): 
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