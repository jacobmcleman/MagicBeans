#include "Sprite.h"

#include "GameObject.h"
#include "Transform.h"
#include "BasicShader.h"

#include "Logger.h"

#include <GL/glew.h>
#include <GLM/gtc/type_ptr.hpp>


namespace Beans
{
  INIT_REFLECTION(Sprite);

  const float Sprite::Vertices[] =
  {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
  };

  const unsigned int Sprite::Indices[] =
  {
    0, 1, 2,
    1, 2, 3
  };

  Shader* Sprite::shaderProgram = nullptr;
  unsigned int Sprite::VBO = 0;
  unsigned int Sprite::VAO = 0;
  unsigned int Sprite::EBO = 0;


  Sprite::Sprite(GameObject * owner) : Component(owner), Color(1, 1, 1, 1)
  {
    //Sprites depend on transform
    Owner->AddComponent<Transform>();
  }

  Sprite::~Sprite()
  {
  }

  void Sprite::Draw()
  {
    shaderProgram->SetUniformVec4f("tint", Color);
    shaderProgram->SetUniformMat4f("object_to_world", Owner->GetComponent<Transform>()->GetMatrix());
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  void Sprite::InitRendering()
  {
    LOG("Initializing Sprite rendering");
    shaderProgram = new Shader("Resources/Shaders/basic.frag", "Resources/Shaders/basic.vert");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    //TODO finish adding element buffer for indices

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }

  void Sprite::DrawSprites(const mat4& camTransform)
  {
    shaderProgram->Use();
    glBindVertexArray(VAO);
   
    shaderProgram->SetUniformMat4f("world_to_camera", camTransform);

    for (const auto& sprite : GetList())
    {
      sprite->Draw();
    }

    glBindVertexArray(0);
  }
}