#include "CubeMesh.h"

#include "GameObject.h"
#include "Transform.h"
#include "BasicShader.h"

#include "Logger.h"
#include "Helpers.h"
#include "MagicBeansEngine.h"

#include "PointLight.h"


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

  CubeMesh::CubeMesh(GameObject* owner) : Component(owner), material(Material::Plastics::White)
  {

  }

  CubeMesh::~CubeMesh()
  {

  }

  void CubeMesh::Draw()
  {
    material.Use(shaderProgram);
    shaderProgram->SetUniformMat4f("object_to_world", Owner->GetComponent<Transform>()->GetMatrix());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glErrorCheck();
  }

  void CubeMesh::InitRendering(MagicBeansEngine* engine)
  {
    LOG("Initializing Cube Rendering");
    shaderProgram = new Shader("Resources/Shaders/basic.frag", "Resources/Shaders/basic.vert");

    glGenVertexArrays(1, &VAO); glErrorCheck();
    glGenBuffers(1, &VBO); glErrorCheck();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); glErrorCheck();

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); glErrorCheck();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); glErrorCheck();
    glEnableVertexAttribArray(0); glErrorCheck();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); glErrorCheck();
    glEnableVertexAttribArray(1); glErrorCheck();
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat))); glErrorCheck();
    glEnableVertexAttribArray(2); glErrorCheck();
    glBindVertexArray(0); glErrorCheck();

    engine_ = engine;
  }

  void CubeMesh::DrawSprites(const mat4& camTransform)
  {
    shaderProgram->Use();
    glBindVertexArray(VAO);
    glErrorCheck();

    shaderProgram->SetUniformMat4f("world_to_camera", camTransform);
    shaderProgram->SetUniformVec3f("viewPos", engine_->GetCamera()->GetComponent<Transform>()->position.Get());
    glErrorCheck();

    PointLight::SendLightsToShader(shaderProgram);
    glErrorCheck();

    for (const auto& cube : GetList())
    {
      cube->Draw();
    }

    glBindVertexArray(0);
    glErrorCheck();
  }
}