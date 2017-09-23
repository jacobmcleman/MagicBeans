#version 330 core

uniform mat4 object_to_world;
uniform mat4 world_to_camera;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec3 normal;
out vec2 uv;
out vec3 worldPos;

void main()
{
	worldPos = (vec4(aPos, 1) * object_to_world).xyz;
	uv = aUV;
	normal = mat3(transpose(inverse(object_to_world))) * aNormal;
	gl_Position = world_to_camera * object_to_world * vec4(aPos, 1);
}