#pragma once
#include <cy/cyVector.h>
#include <program.h>
#include <memory>
#include <string>
#include <texture.h>

class Material
{
  public:
    Material(cy::Vec3f diffuseColor, float specularStrength, float shininess): 
      diffuse(diffuseColor), specularStrength(specularStrength), shininess(shininess), diffuseTex(nullptr) {};

    Material(std::shared_ptr<Texture> diffuseTex, float specularStrength, float shininess);
    Material(std::string diffuseTexturePath, float specularStrength, float shininess);

    void addDiffuseTexture(std::shared_ptr<Texture> diffuseTex);
    
    virtual void bind(Program* shaderProgram, char* arrayName, uint16_t index, uint8_t texUnit);
    
    cy::Vec3f diffuse = cy::Vec3f(1.0f, 1.0f, 1.0f);

    std::shared_ptr<Texture> diffuseTex = nullptr;
    uint8_t activeTextures = 0;
    float specularStrength = 1.0f, shininess = 1.0f;
  private:
};

extern std::shared_ptr<Material> DEFAULT_MATERIAL;