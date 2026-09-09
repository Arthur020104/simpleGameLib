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

struct Material {
  vec3 diffuse;
  vec3 specularColor;
  
  float shininess;

  bool hasDiffuseTex;
  int diffuseTexUnit;

  bool hasSpecularTex;
  int specularTexUnit;

  int emissiveTexUnit;
  bool hasEmissiveTex;
};

vec3 applyLight(Light light, vec3 normal, vec3 viewDirection, Material mat);

const uint MAX_LIGHT_SIZE = 50;
const uint MAX_MATERIAL_SIZE = 50;
const uint MAX_TEXTURES_SIZE = 50;

out vec4 FragColor;
in vec3 worldFragPos;
in vec3 normalV;
in vec2 texCoord;
in float distanceFromBaseY;

flat in uint materialIdx;

uniform Light lights[MAX_LIGHT_SIZE];
uniform uint lightsSize;

uniform Material materials[MAX_MATERIAL_SIZE];
uniform vec3 viewPosition;
uniform sampler2D textures[MAX_TEXTURES_SIZE];

void main()
{  
  vec3 normal = normalize(normalV);

  Material mat = materials[materialIdx];

  vec3 viewDirection = normalize(viewPosition - worldFragPos);
  
  vec3 lightsEffect = vec3(0.0, 0.0, 0.0);
  for(uint i = 0; i < lightsSize; i++)
  {
    lightsEffect += applyLight(lights[i], normal, viewDirection, mat);
  }
  lightsEffect += mat.hasEmissiveTex ? texture(textures[mat.emissiveTexUnit], texCoord).rgb : vec3(0.0, 0.0, 0.0);

  if(lightsSize > MAX_LIGHT_SIZE)
    lightsEffect = vec3(0.0, 0.0, 0.0);

  FragColor = vec4(lightsEffect, 1.0) + vec4(0.12, 0.15, 0.0, 1.0) * distanceFromBaseY;
}

vec3 applyLight(Light light, vec3 normal, vec3 viewDirection, Material mat)
{
  vec3 diffuseColor = mat.hasDiffuseTex ? texture(textures[mat.diffuseTexUnit], texCoord).rgb : mat.diffuse;

  switch(light.type)
  {
    case 1:
    {
      vec3 reflectDirection = reflect(-light.position, normal);  
      float theta = max(dot(normal, light.position), 0.0);

      float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), mat.shininess);
      vec3 specular = mat.hasSpecularTex ? 
        texture(textures[mat.specularTexUnit], texCoord).rgb * mat.specularColor * spec * light.color * light.intensity: 
        mat.specularColor * spec * light.color * diffuseColor * light.intensity;  

      vec3 diffuse = light.color * theta * light.intensity;
      vec3 ambient = light.color * light.ambientIntensity;

      return (diffuse + ambient) * diffuseColor + specular;
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

      float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), mat.shininess);

      vec3 specular = mat.hasSpecularTex ? 
        texture(textures[mat.specularTexUnit], texCoord).rgb * mat.specularColor * spec * light.color * light.intensity: 
        mat.specularColor * spec * light.color * diffuseColor * light.intensity;
       
      vec3 diffuse = light.color * theta * light.intensity;
      vec3 ambient = light.color * light.ambientIntensity;

      return ((diffuse + ambient ) * diffuseColor + specular) * attenuation;
      break;
    }
    default:
      return vec3(0.0, 0.0, 0.0);
  }
}