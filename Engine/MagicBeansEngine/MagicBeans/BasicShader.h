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
mat4 object_to_world; \
mat4 world_to_camera; \
\
layout(location = 0) in vec3 position;\
\
void main()\
{\
  gl_Position = world_to_camera * object_to_world * vec4(position.x, position.y, position.z, 1.0);\
}\
";