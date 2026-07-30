#include <program.h>
#include <string.h>
#include <utils.h>

Program::Program(const char* vertexPath, const char* fragmentPath)
{
  this->vertexShaderPath = std::string(vertexPath);
  this->fragmentShaderPath = std::string(fragmentPath);
  this->compileShaders();
}

void Program::compileShaders()
{
  unsigned short shaders[2];

  shaders[0] = this->prepareShader(readFile(this->vertexShaderPath.c_str()).c_str(), "VERTEX");
  shaders[1] = this->prepareShader(readFile(this->fragmentShaderPath.c_str()).c_str(), "FRAGMENT");

  prepareProgram(shaders, 2);
}

Program::~Program()
{
  this->destroyProgram();
}

void Program::destroyProgram()
{
  glDeleteProgram(this->id);
}

void Program::prepareProgram(unsigned short* shaders, unsigned short size)
{
  this->id = glCreateProgram();

  for(unsigned short i = 0; i < size; i++)
  {
    glAttachShader(this->id, shaders[i]);
    glDeleteShader(shaders[i]);
  }
  glLinkProgram(this->id);

  checkCompileErrors(this->id, "PROGRAM");
}

unsigned short Program::prepareShader(const char* shaderCode, const char* type)
{
  unsigned short shaderId;
	shaderId = glCreateShader(this->SHADER_TYPES.at(type));

	glShaderSource(shaderId, 1, &shaderCode, NULL);
	glCompileShader(shaderId);
	checkCompileErrors(shaderId, type);

  return shaderId;
}

void Program::checkCompileErrors(unsigned short id, const char* type)
{
  GLint sucess;
	char infoLog[1024];
	
  if(type == "PROGRAM")
  {
    glGetProgramiv(id, GL_LINK_STATUS, &sucess);
    if(!sucess)
    {
      glGetProgramInfoLog(id, 512, NULL, infoLog);
      throw std::runtime_error(std::string("ERROR LINKIN THE PROGRAM: ") + infoLog);
    }
    return;
  }

  glGetShaderiv(id, GL_COMPILE_STATUS, &sucess);
  if(!sucess)
  {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    throw std::runtime_error(std::string("ERROR ON ") + type + std::string(" SHADER COMPILATION: ") + infoLog);
  }
}

unsigned short Program::getProgram()
{
  return this->id;
}

void Program::registerObjectUsingProgram(GameObject* obj)
{
  this->objectsUsingProgram++;
}

uint16_t Program::getUsingProgram()
{
  return this->objectsUsingProgram;
}

u_int16_t Program::removeUsingProgram(GameObject* obj)
{
  return --this->objectsUsingProgram;
}