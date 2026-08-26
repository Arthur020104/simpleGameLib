#pragma once
#include <memory>
#include <string>
#include <texture.h>
#include <program.h>
#include <cy/cyVector.h>

class Material
{
  public:
    Material(glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess): 
      diffuse(diffuseColor), specularColor(specularColor), shininess(shininess), diffuseTex(nullptr) {};

    Material(std::shared_ptr<Texture> diffuseTex, glm::vec3 specularColor, float shininess);
    Material(std::string diffuseTexturePath, glm::vec3 specularColor, float shininess);

    void addDiffuseTexture(std::shared_ptr<Texture> diffuseTex);
    void addDiffuseTexture(std::string diffuseTexturePath);

    void addSpecularTexture(std::shared_ptr<Texture> specularTex);
    void addSpecularTexture(std::string specularTexturePath);

    void addEmissiveTexture(std::shared_ptr<Texture> emissiveTex);
    void addEmissiveTexture(std::string emissiveTexturePath);
    
    virtual void bind(Program* shaderProgram, char* arrayName, uint16_t index, uint16_t texUnit);

    static std::shared_ptr<Material> getDefaultMaterial();
    
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

    std::shared_ptr<Texture> diffuseTex = nullptr;
    std::shared_ptr<Texture> specularTex = nullptr;
    std::shared_ptr<Texture> emissiveTex = nullptr;
    uint16_t activeTextures = 0;
    float shininess = 1.0f;
  private:

    static std::shared_ptr<Material> createDefaultMaterial();

};