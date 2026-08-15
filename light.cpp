#include <light.h>
#include <string.h>

void Light::bind(Program* shaderProgram)
{
  glUseProgram(shaderProgram->getProgram());//bind is happening in here and in GameObject draw

  shaderProgram->bindVec3("lightColor", this->color);
  shaderProgram->bindVec3("lightPosition", this->getPosition());
  
  shaderProgram->bindFloat("lightIntensity", this->intensity);
  shaderProgram->bindFloat("ambientIntensity", this->ambientIntensity);
  shaderProgram->bindUint("lightType", this->getLightType());
}

void Light::bind(Program* shaderProgram, char* arrayName, uint16_t index)
{
  glUseProgram(shaderProgram->getProgram());//bind is happening in here and in GameObject draw

  std::string location = std::string(arrayName) + "[" + std::to_string(index) + "]";

  shaderProgram->bindVec3((location + ".color").c_str(), this->color);
  shaderProgram->bindVec3((location + ".position").c_str(), this->getPosition());

  shaderProgram->bindFloat((location + ".intensity").c_str(), this->intensity);
  shaderProgram->bindFloat((location + ".ambientIntensity").c_str(), this->ambientIntensity);

  shaderProgram->bindUint((location + ".type").c_str(), this->getLightType());
}

cy::Vec3f DirectionalLight::getPosition()
{
  return -this->getNormalizedPosition();
}

void PointLight::bind(Program* shaderProgram) 
{
  Light::bind(shaderProgram);

  shaderProgram->bindFloat("constantFallOff", this->constantFallOff);
  shaderProgram->bindFloat("linearFallOff", this->linearFallOff);
  shaderProgram->bindFloat("quadraticFallOff", this->quadraticFallOff);
}

void PointLight::bind(Program* shaderProgram, char* arrayName, uint16_t index)
{
  Light::bind(shaderProgram, arrayName, index);

  std::string location = std::string(arrayName) + "[" + std::to_string(index) + "]";

  shaderProgram->bindFloat((location + ".constantFallOff").c_str(), this->constantFallOff);
  shaderProgram->bindFloat((location + ".linearFallOff").c_str(), this->linearFallOff);
  shaderProgram->bindFloat((location + ".quadraticFallOff").c_str(), this->quadraticFallOff);
}