#version 460 core
out vec4 FragColor;
in vec3 normalV;

uniform bool isSelected;

void main()
{
  if(isSelected)
  {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  }
  else
  {
    FragColor = vec4(normalV.xyz, 1.0);
  }
  
}