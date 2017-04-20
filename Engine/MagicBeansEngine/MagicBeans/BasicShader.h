#pragma once
static const char *fragment_shader_text =
"\
#version 330 core\n\
\
out vec4 color;\
uniform vec4 tint;\
\
void main()\
{\
  color = tint;\
}\
";


static const char *vertex_shader_text =
"\
#version 330 core\n\
\
layout(location = 0) in vec3 position;\
\
void main()\
{\
  gl_Position = vec4(position.x, position.y, position.z, 1.0);\
}\
";