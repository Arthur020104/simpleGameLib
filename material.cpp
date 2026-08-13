#include <material.h>

void Material::bind(Program* shaderProgram, char* arrayName, uint16_t index, uint8_t texUnit)
{
  glUseProgram(shaderProgram->getProgram());//bind is happening in here, GameObject and Light draw

  std::string location = std::string(arrayName) + "[" + std::to_string(index) + "]";

  shaderProgram->bindVec3((location + ".diffuse").c_str(), this->diffuse);

  shaderProgram->bindFloat((location + ".specularStrength").c_str(), this->specularStrength);
  shaderProgram->bindFloat((location + ".shininess").c_str(), this->shininess);

  shaderProgram->bindBool((location + ".hasDiffuseTex").c_str(), this->diffuseTex != nullptr);
  shaderProgram->bindBool((location + ".hasSpecularTex").c_str(), this->specularTex != nullptr);

  if(this->diffuseTex != nullptr)
    this->diffuseTex->bind(shaderProgram, (location + ".diffuseTexUnit").c_str(), texUnit);

  if(this->specularTex != nullptr)
    this->specularTex->bind(shaderProgram, (location + ".specularTexUnit").c_str(), texUnit + 1);
}

Material::Material(std::string diffuseTexturePath, float specularStrength, float shininess): 
specularStrength(specularStrength), shininess(shininess) 
{
  this->addDiffuseTexture(diffuseTexturePath);
}

Material::Material(std::shared_ptr<Texture> diffuseTex, float specularStrength, float shininess): 
specularStrength(specularStrength), shininess(shininess) 
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