#include <light.h>
#include <cstdio>
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

  char location[32];
  std::snprintf(location, sizeof(location), "%s[%u]", arrayName, index);

  char strBuffer[32];

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.color", location);
  shaderProgram->bindVec3(strBuffer, this->color);

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.position", location);
  shaderProgram->bindVec3(strBuffer, this->getPosition());

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.intensity", location);
  shaderProgram->bindFloat(strBuffer, this->intensity);

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.ambientIntensity", location);
  shaderProgram->bindFloat(strBuffer, this->ambientIntensity);

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.type", location);
  shaderProgram->bindUint(strBuffer, this->getLightType());
}

glm::vec3 DirectionalLight::getPosition()
{
  return this->getNormalizedPosition();
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

  char location[32];
  std::snprintf(location, sizeof(location), "%s[%u]", arrayName, index);

  char strBuffer[32];

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.constantFallOff", location);
  shaderProgram->bindFloat(strBuffer, this->constantFallOff);

  std::snprintf(strBuffer, sizeof(strBuffer), "%s.linearFallOff", location);
  shaderProgram->bindFloat(strBuffer, this->linearFallOff);
  
  std::snprintf(strBuffer, sizeof(strBuffer), "%s.quadraticFallOff", location);
  shaderProgram->bindFloat(strBuffer, this->quadraticFallOff);
}