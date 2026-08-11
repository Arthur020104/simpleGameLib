#pragma once
#include <cy/cyVector.h>
#include <program.h>
#include <memory>

class Material
{
  public:
    Material(cy::Vec3f diffuseColor, float specularStrength, float shininess): 
      diffuse(diffuseColor), specularStrength(specularStrength), shininess(shininess) {};
    
    virtual void bind(Program* shaderProgram, char* arrayName, uint16_t index);
    
    cy::Vec3f diffuse = cy::Vec3f(1.0f, 1.0f, 1.0f);
    float specularStrength = 1.0f, shininess = 1.0f;
  private:
};

extern std::shared_ptr<Material> DEFAULT_MATERIAL;