#version 460 core
struct Light {
  vec3 position;
  vec3 color;

  float intensity;
  float ambientIntensity;
  uint type;

  float constantFallOff;
  float linearFallOff;
  float quadraticFallOff;
};
vec3 applyLight(Light light, vec3 normal, vec3 viewDirection);

const uint MAX_LIGHT_SIZE = 50;

out vec4 FragColor;
in vec3 worldFragPos;
in vec3 normalV;

uniform Light lights[MAX_LIGHT_SIZE];
uniform uint lightsSize;
uniform bool isSelected;
uniform vec3 viewPosition;

float specularStrength = 0.5;
float shininess = 32;

void main()
{  
  vec3 normal = normalize(normalV);
  vec4 baseColor = isSelected ? vec4(1.0, 1.0, 1.0, 1.0):  vec4(0.3, 0.3, 1.0, 1.0);

  vec3 viewDirection = normalize(viewPosition - worldFragPos);
  
  vec3 lightsEffect = vec3(0.0, 0.0, 0.0);
  for(uint i = 0; i < lightsSize; i++)
  {
    lightsEffect += applyLight(lights[i], normal, viewDirection);
  }
  FragColor = vec4(lightsEffect * baseColor.xyz, 1.0);
}

vec3 applyLight(Light light, vec3 normal, vec3 viewDirection)
{
  switch(light.type)
  {
    case 1:
    {
      vec3 reflectDirection = reflect(-light.position, normal);  
      float theta = max(dot(normal, light.position), 0.0);

      float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
      vec3 specular = specularStrength * spec * light.color;  

      vec3 diffuse = light.color * theta * light.intensity;
      vec3 ambient = light.color * light.ambientIntensity;

      return diffuse + ambient + specular;
      break;
    }
    case 2:
    {
      vec3 diff = light.position - worldFragPos;
      
      float distance = length(diff);
      float attenuation = 1.0 / (light.constantFallOff + light.linearFallOff * distance + light.quadraticFallOff * (distance * distance));

      vec3 lightDir = normalize(diff);
      vec3 reflectDirection = reflect(-lightDir, normal); 

      float theta = max(dot(normal, lightDir), 0.0);

      float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);

      vec3 specular = specularStrength * spec * light.color * light.intensity;  
      vec3 diffuse = light.color * theta * light.intensity;
      vec3 ambient = light.color * light.ambientIntensity;

      return (diffuse + ambient + specular) * attenuation;
      break;
    }
    default:
      return vec3(0.0, 0.0, 0.0);
  }
}