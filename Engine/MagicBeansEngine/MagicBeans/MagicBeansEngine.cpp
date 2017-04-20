// MagicBeansEngine.cpp : Defines the exported functions for the static library.
//

#include "MagicBeansEngine.h"
#include "Logger.h"

#include "GL\glew.h"
#include "BasicShader.h"

namespace Beans
{
  // This is the constructor of a class that has been exported.
  // see MagicBeansEngine.h for the class definition
  MagicBeansEngine::MagicBeansEngine(const std::string& gamename) :
    gameWindow_(gamename)
  {
    //Compile the shaders
    //TODO move somewhere else once its working
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

    GLfloat vertices[] =
    {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.5f, 0.5f, 0.0f,
     
      -0.5f, -0.5f, 0.0f,
      0.5f, 0.5f, 0.0f,
      -0.5f, 0.5f, 0.0f
    };
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    // 2. Copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. Then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //4. Unbind the VAO
    glBindVertexArray(0);

  }

  void MagicBeansEngine::RunGameLoop()
  {
    while (gameWindow_.UpdateWindow())
    {
      UpdateStep();

      DrawStep();

      gameWindow_.SwapBuffers();
    }
  }

  void MagicBeansEngine::UpdateStep()
  {
    //Do nothing (for now)
  }

  void MagicBeansEngine::DrawStep()
  {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glUniform4f(glGetUniformLocation(shaderProgram, "tint"), 1, 0, 0, 1);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    
  }

}