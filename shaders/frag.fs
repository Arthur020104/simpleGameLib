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
vec3 applyLight(Light light, vec3 normal, vec3 viewDirection, float shininess, float specularStrength);

struct Material {
  vec3 diffuse;
  float shininess;
  float specularStrength;

  bool hasDiffuseTex;
  int diffuseTexUnit;
};

const uint MAX_LIGHT_SIZE = 50;
const uint MAX_MATERIAL_SIZE = 50;
const uint MAX_TEXTURES_SIZE = 50;

out vec4 FragColor;
in vec3 worldFragPos;
in vec3 normalV;
in vec2 texCoord;

flat in uint materialIdx;

uniform Light lights[MAX_LIGHT_SIZE];
uniform uint lightsSize;

uniform Material materials[MAX_MATERIAL_SIZE];
uniform bool isSelected;
uniform vec3 viewPosition;

uniform sampler2D textures[MAX_TEXTURES_SIZE];

void main()
{  
  vec3 normal = normalize(normalV);

  Material mat = materials[materialIdx];
  vec3 diffuseColor = mat.hasDiffuseTex ? texture(textures[mat.diffuseTexUnit], texCoord).rgb : mat.diffuse;

  vec3 viewDirection = normalize(viewPosition - worldFragPos);
  
  vec3 lightsEffect = vec3(0.0, 0.0, 0.0);
  for(uint i = 0; i < lightsSize; i++)
  {
    lightsEffect += applyLight(lights[i], normal, viewDirection, mat.shininess, mat.specularStrength);
  }

  if(lightsSize > MAX_LIGHT_SIZE)
    lightsEffect = vec3(0.0, 0.0, 0.0);

  FragColor = vec4(lightsEffect * diffuseColor, 1.0);
}

vec3 applyLight(Light light, vec3 normal, vec3 viewDirection, float shininess, float specularStrength)
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