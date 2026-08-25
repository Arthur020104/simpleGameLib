#pragma once
#include <cy/cyVector.h>
#include <program.h>
#include <memory>
#include <string>
#include <texture.h>

class Material
{
  public:
    Material(cy::Vec3f diffuseColor, cy::Vec3f specularColor, float shininess): 
      diffuse(diffuseColor), specularColor(specularColor), shininess(shininess), diffuseTex(nullptr) {};

    Material(std::shared_ptr<Texture> diffuseTex, cy::Vec3f specularColor, float shininess);
    Material(std::string diffuseTexturePath, cy::Vec3f specularColor, float shininess);

    void addDiffuseTexture(std::shared_ptr<Texture> diffuseTex);
    void addDiffuseTexture(std::string diffuseTexturePath);

    void addSpecularTexture(std::shared_ptr<Texture> specularTex);
    void addSpecularTexture(std::string specularTexturePath);

    void addEmissiveTexture(std::shared_ptr<Texture> emissiveTex);
    void addEmissiveTexture(std::string emissiveTexturePath);
    
    virtual void bind(Program* shaderProgram, char* arrayName, uint16_t index, uint16_t texUnit);
    
    cy::Vec3f diffuse = cy::Vec3f(1.0f, 1.0f, 1.0f);
    cy::Vec3f specularColor = cy::Vec3f(1.0f, 1.0f, 1.0f);

    std::shared_ptr<Texture> diffuseTex = nullptr;
    std::shared_ptr<Texture> specularTex = nullptr;
    std::shared_ptr<Texture> emissiveTex = nullptr;
    uint16_t activeTextures = 0;
    float shininess = 1.0f;
  private:
};

extern std::shared_ptr<Material> DEFAULT_MATERIAL;