#vertex_shader
#version 330 core

layout(location = 0) in vec3 a_modelVertexPosition;

uniform mat4 u_ModelToWorldTransformation;
uniform mat4 u_lightProjection;
uniform mat4 u_lightView;

void main()
{
	vec3 worldVertexPosition = vec3(u_ModelToWorldTransformation * vec4(a_modelVertexPosition, 1.0));

	gl_Position =  u_lightProjection * u_lightView * vec4(worldVertexPosition, 1.0);

}



#fragment_shader
#version 330 core

void main()
{
	gl_FragDepth = gl_FragCoord.z;
}