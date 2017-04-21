#include "Sprite.h"

#include "GameObject.h"
#include "Transform.h"
#include "BasicShader.h"

#include "Logger.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLM/gtc/type_ptr.hpp>

namespace Beans
{
  INIT_REFLECTION(Sprite);

  const float Sprite::Vertices[] =
  {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,

    -0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
  };

  unsigned int Sprite::shaderProgram = 0;
  unsigned int Sprite::VBO = 0;
  unsigned int Sprite::VAO = 0;


  Sprite::Sprite(GameObject * owner) : Component(owner)
  {
  }

  void Sprite::Draw()
  {
    glUniform4f(glGetUniformLocation(shaderProgram, "tint"), Color.r, Color.g, Color.b, Color.a);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "world_to_camera"), 1, GL_FALSE, glm::value_ptr(Owner->GetComponent<Transform>()->GetMatrix()));

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  void Sprite::InitRendering()
  {
    //Compile the shaders
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);


    glShaderSource(vertexShader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      LOG(infoLog);
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      LOG(infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      LOG(infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    // 2. Copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    // 3. Then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //4. Unbind the VAO
    glBindVertexArray(0);
  }
  void Sprite::DrawSprites(const mat4& camTransform)
  {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "world_to_camera"), 1, GL_FALSE, glm::value_ptr(camTransform));

    for (const auto& sprite : GetList())
    {
      sprite->Draw();
    }

    glBindVertexArray(0);
  }
}