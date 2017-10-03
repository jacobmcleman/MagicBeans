#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec3 emission;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform PointLight pointLights[16];
uniform int pointLightCount;

uniform vec3 viewPos;

in vec3 normal;
in vec2 uv;
in vec3 worldPos;

out vec4 FragColor;

vec3 CalculatePointLight(vec3 N, vec3 V, int lightIndex)
{
		//Vector to this light
		vec3 L = pointLights[lightIndex].position - worldPos;
		//Halfway vector between L and V
		vec3 H = normalize(L + V);
		//Distance to this light
		float distance = length(L);
		L = normalize(L);

		//Amount this light source will fade with distance
		float attenuation = min(1.0, 1.0 / (pointLights[lightIndex].constant + pointLights[lightIndex].linear * distance + pointLights[lightIndex].quadratic * distance * distance));

		//'Compute' Ambient
		float amb = 0.5;
		vec3 ambient = amb * pointLights[lightIndex].ambient * attenuation;
		
		//Compute Diffuse
		float diff = max(dot(N, L), 0.0);
		vec3 diffuse = diff * pointLights[lightIndex].diffuse * attenuation;
		
		//Compute Specular
		float spec = pow(max(dot(N, H), 0.0), material.shininess);
		vec3 specular = spec * pointLights[lightIndex].specular * attenuation;
		
		return (ambient * material.ambient) + (diffuse * material.diffuse) + (specular * material.specular);
}

void main()
{
	//Compute things that are based on the surface/viewer
	vec3 N = normalize(normal);
	vec3 V = normalize(viewPos - worldPos);

	vec3 output = vec3(0.0);

	for(int i = 0; i < pointLightCount; ++i)
	{
		output += CalculatePointLight(N, V, i);
	}

	output += material.emission;

	FragColor = vec4(output, 1);
}