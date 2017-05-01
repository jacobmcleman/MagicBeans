#include "CubeMesh.h"

#include "GameObject.h"
#include "Transform.h"
#include "BasicShader.h"

#include "Logger.h"
#include "MagicBeansEngine.h"


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLM/gtc/type_ptr.hpp>

namespace Beans
{
  INIT_REFLECTION(CubeMesh);

  const float CubeMesh::Vertices[] =
  { 
    /*
      POSITION                       TEXTURE                  NORMAL
    */                                                        
    -0.5f, -0.5f, -0.5f,             0.0f, 0.0f,              0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,             1.0f, 0.0f,              0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,             1.0f, 1.0f,              0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,             1.0f, 1.0f,              0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,             0.0f, 1.0f,              0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,             0.0f, 0.0f,              0.0f, 0.0f, -1.0f,
                                                              
    -0.5f, -0.5f,  0.5f,             0.0f, 0.0f,              0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,             1.0f, 0.0f,              0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,             1.0f, 1.0f,              0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,             1.0f, 1.0f,              0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,             0.0f, 1.0f,              0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,             0.0f, 0.0f,              0.0f, 0.0f, 1.0f,
                                                              
    -0.5f,  0.5f,  0.5f,             1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,             1.0f, 1.0f,              -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,             0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,             0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,             0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,             1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,
                                                              
    0.5f,  0.5f,  0.5f,              1.0f, 0.0f,              1.0f, 0.0f, 0.0f,
    0.5f,  0.5f, -0.5f,              1.0f, 1.0f,              1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,              0.0f, 1.0f,              1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,              0.0f, 1.0f,              1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,              0.0f, 0.0f,              1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,              1.0f, 0.0f,              1.0f, 0.0f, 0.0f,
                                                              
    -0.5f, -0.5f, -0.5f,             0.0f, 1.0f,              0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,             1.0f, 1.0f,              0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,             1.0f, 0.0f,              0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,             1.0f, 0.0f,              0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,             0.0f, 0.0f,              0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,             0.0f, 1.0f,              0.0f, -1.0f, 0.0f, 
                                                              
    -0.5f,  0.5f, -0.5f,             0.0f, 1.0f,              0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,             1.0f, 1.0f,              0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,             1.0f, 0.0f,              0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,             1.0f, 0.0f,              0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,             0.0f, 0.0f,              0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,             0.0f, 1.0f,              0.0f, 1.0f, 0.0f
  };

  Shader* CubeMesh::shaderProgram = nullptr;
  unsigned int CubeMesh::VBO = 0;
  unsigned int CubeMesh::VAO = 0;
  MagicBeansEngine* CubeMesh::engine_ = nullptr;

  CubeMesh::CubeMesh(GameObject* owner) : Component(owner), Color(1, 1, 1)
  {

  }

  CubeMesh::~CubeMesh()
  {

  }

  void CubeMesh::Draw()
  {
    shaderProgram->SetUniformVec4f("tint", vec4(Color, 1.0f));
    shaderProgram->SetUniformMat4f("object_to_world", Owner->GetComponent<Transform>()->GetMatrix());
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  void CubeMesh::InitRendering(MagicBeansEngine* engine)
  {
    LOG("Initializing Cube Rendering");
    shaderProgram = new Shader("Resources/Shaders/basic.frag", "Resources/Shaders/basic.vert");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    engine_ = engine;
  }

  void CubeMesh::DrawSprites(const mat4& camTransform)
  {
    shaderProgram->Use();
    glBindVertexArray(VAO);
    shaderProgram->SetUniformMat4f("world_to_camera", camTransform);
    shaderProgram->SetUniformVec3f("lightPos", vec3(20.0f * sinf((float)(engine_->GetTimeSinceStartup())), 0, 14.5f));
    shaderProgram->SetUniformVec3f("viewPos", engine_->GetCamera()->GetComponent<Transform>()->position.Data());

    for (const auto& cube : GetList())
    {
      cube->Draw();
    }

    glBindVertexArray(0);
  }
}