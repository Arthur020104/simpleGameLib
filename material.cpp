#include <material.h>

void Material::bind(Program* shaderProgram, char* arrayName, uint16_t index)
{
  glUseProgram(shaderProgram->getProgram());//bind is happening in here, GameObject and Light draw

  std::string location = std::string(arrayName) + "[" + std::to_string(index) + "]";

  shaderProgram->bindVec3((location + ".diffuse").c_str(), this->diffuse);

  shaderProgram->bindFloat((location + ".specularStrength").c_str(), this->specularStrength);
  shaderProgram->bindFloat((location + ".shininess").c_str(), this->shininess);
}
