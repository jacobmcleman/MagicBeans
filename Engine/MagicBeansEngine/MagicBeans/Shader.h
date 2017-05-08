#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h> // Include glew to get all the required OpenGL headers

#include <GLM\glm.hpp>

namespace Beans
{
  class Shader
  {
  public:
    Shader(const char* fragmentFile, const char* vertexFile, const char* geomFile = nullptr);

    void Use() const;

    void SetUniformMat4f(const GLchar* name, const glm::mat4& matrix) const;
    void SetUniformVec4f(const GLchar* name, const glm::vec4& vector) const;
    void SetUniformVec3f(const GLchar* name, const glm::vec3& vector) const;
    void SetUniformFloat(const GLchar* name, const float& value) const;
    void SetUniformInt (const GLchar* name, const int& value) const;

    GLuint getUniformLoc(const GLchar* name) const;

    GLuint GetShaderProgram() const;

  private:
    GLuint shaderProgram_;

   
  };
}