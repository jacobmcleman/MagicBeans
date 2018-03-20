#include "Shader.h"

#include <../Includes/GLM/gtc/matrix_transform.hpp>
#include <../Includes/GLM/gtc/type_ptr.hpp>

#include "Logger.h"
#include "Helpers.h"

namespace
{
  GLuint CompileShader(const GLchar* shaderCode, GLenum type)
  {
    static GLchar infoLog[512];
    GLint success;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      LOG(std::string("ERROR::SHADER::COMPILATION_FAILED: ") + infoLog);
    }

    return shader;
  }
}

Beans::Shader::Shader(const char * fragmentFileName, const char * vertexFileName)
{
  static GLchar infoLog[512];

  std::string vertCode, fragCode;
  std::ifstream fragFile, vertFile;

  vertFile.open(vertexFileName);
  fragFile.open(fragmentFileName);

  if (!vertFile.good())
  {
    LOG(std::string("Vertex shader failed to open (location given: ") + vertexFileName + ")");
  }
  if (!fragFile.good())
  {
    LOG(std::string("Fragment shader failed to open (location given: ") + fragmentFileName + ")");
  }

  std::stringstream vertStream, fragStream;

  vertStream << vertFile.rdbuf();
  fragStream << fragFile.rdbuf();

  vertFile.close();
  fragFile.close();

  vertCode = vertStream.str();
  fragCode = fragStream.str();

  const GLchar* vertShaderCode = vertCode.c_str();
  const GLchar* fragShaderCode = fragCode.c_str();

  //Compile the shaders
  GLuint vertShader = CompileShader(vertShaderCode, GL_VERTEX_SHADER);
  GLuint fragShader = CompileShader(fragShaderCode, GL_FRAGMENT_SHADER);
  
  //Link the shaders into a program
  shaderProgram_ = glCreateProgram();
  glAttachShader(shaderProgram_, vertShader);
  glAttachShader(shaderProgram_, fragShader);
  glLinkProgram(shaderProgram_);

  GLint success;

  glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
    LOG(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED: ") + infoLog);
  }

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
}

void Beans::Shader::Use() const
{
  glUseProgram(shaderProgram_);
}

void Beans::Shader::SetUniformMat4f(const GLchar * name, const glm::mat4 & matrix) const
{
  glUniformMatrix4fv(getUniformLoc(name), 1, GL_FALSE, glm::value_ptr(matrix));
  glErrorCheck();
}

void Beans::Shader::SetUniformVec4f(const GLchar * name, const glm::vec4 & vector) const
{
  glUniform4f(getUniformLoc(name), vector.x, vector.y, vector.z, vector.w);
  glErrorCheck();
}

void Beans::Shader::SetUniformVec3f(const GLchar * name, const glm::vec3 & vector) const
{
  glUniform3f(getUniformLoc(name), vector.x, vector.y, vector.z);
  glErrorCheck();
}

void Beans::Shader::SetUniformFloat(const GLchar * name, const float & value) const
{
  glUniform1f(getUniformLoc(name), value);
  glErrorCheck();
}

void Beans::Shader::SetUniformInt(const GLchar * name, const int & value) const
{
  glUniform1i(getUniformLoc(name), value);
  glErrorCheck();
}

GLuint Beans::Shader::GetShaderProgram() const
{
  return shaderProgram_;
}



GLuint Beans::Shader::getUniformLoc(const GLchar * name) const
{
  return glGetUniformLocation(shaderProgram_, name);
}
