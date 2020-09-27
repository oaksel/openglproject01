#vertex_shader
#version 330

layout(location = 0) in vec3 a_modelVertexPosition;
layout(location = 1) in vec3 a_modelVertexNormal;
layout(location = 2) in vec2 a_modelVertexTextureCoordinate;
layout(location = 3) in vec3 a_modelVertexColor;

uniform mat4 u_modelToWorldTransformation;
uniform mat4 u_cameraProjection;
uniform mat4 u_cameraView;
uniform mat4 u_lightView; // for shadow

out vec3 p_worldVertexPosition;
out vec3 p_worldVertexNormal;
out vec2 p_modelVertexTextureCoordinate;
out vec3 p_modelVertexColor;

out vec4 p_lightVertexPosition; // for shadow

void main()
{
	p_modelVertexColor = a_modelVertexColor;
	p_worldVertexPosition = vec3(u_modelToWorldTransformation * vec4(a_modelVertexPosition, 1.0));
	p_worldVertexNormal = vec3(u_modelToWorldTransformation * vec4(a_modelVertexNormal, 0.0)); // 0.0 means do not include translations, only rotations

	p_modelVertexTextureCoordinate = a_modelVertexTextureCoordinate;

	p_lightVertexPosition = u_cameraProjection * u_lightView * vec4(p_worldVertexPosition, 1.0);

	gl_Position = u_cameraProjection * u_cameraView * vec4(p_worldVertexPosition, 1.0);
}


#fragment_shader
#version 330

in vec3 p_worldVertexPosition;
in vec3 p_worldVertexNormal;
in vec2 p_modelVertexTextureCoordinate;
in vec3 p_modelVertexColor;

in vec4 p_lightVertexPosition;

struct Light
{
	vec3 position;
	vec3 color;

	float linearAttenuation;
	float quadraticAttenuation;
};

struct Material
{
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

uniform Light u_light;
uniform Material u_material;

uniform sampler2D u_Texture;
uniform sampler2D u_depthMap;

uniform vec3 u_cameraPosition;

float shadowMaker(vec4 lightVertexPosition)
{
	vec3 projectedCoordinate = lightVertexPosition.xyz / lightVertexPosition.w;	// perspective divide?


	projectedCoordinate = projectedCoordinate * 0.5 + 0.5; // from NDC which is [-1,1] to [0,1]

	float closestDepth = texture(u_depthMap, projectedCoordinate.xy).z;
	float currentDepth = projectedCoordinate.z;

	float inLight = 0.0;
	if(currentDepth > closestDepth) inLight = 1.0;
	else inLight = 0.0;

	return inLight;
}

out vec4 o_Color;

void main()
{
	//ambient
	float ambientEffect = u_material.ambient;

	// diffuse
	vec3 lightVector = normalize(u_light.position - p_worldVertexPosition);
	float diffuseEffect = dot(lightVector, p_worldVertexNormal);
	diffuseEffect = clamp(diffuseEffect, 0, 1);
	diffuseEffect *= u_material.diffuse;

	//specular
	
	vec3 viewVector = vec3(normalize(u_cameraPosition - p_worldVertexPosition));
	vec3 halfwayVector = normalize(lightVector + viewVector);
	float specularEffect = dot(p_worldVertexNormal, halfwayVector);
	specularEffect = max(specularEffect, 0.0);
	specularEffect = pow(specularEffect, u_material.shininess);
	specularEffect *= u_material.specular;
	
	// attenuation

	float distance = length(u_light.position - p_worldVertexPosition);
	float attenuation = 1.0 / (1.0 * pow(distance, 0) + u_light.linearAttenuation * pow(distance, 1) + u_light.quadraticAttenuation * pow(distance, 2));

	vec3 lightEffect = u_light.color * (ambientEffect + (diffuseEffect + specularEffect));
	lightEffect *= attenuation;

	vec4 textureColor = texture(u_Texture, p_modelVertexTextureCoordinate);
	textureColor.rgb *= lightEffect;
	o_Color = textureColor;



	//o_Color = vec4(p_modelVertexColor * lightEffect, 1.0);
}
