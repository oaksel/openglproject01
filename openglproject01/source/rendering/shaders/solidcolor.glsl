#vertex_shader
#version 330

layout(location = 0) in vec3 a_modelVertexPosition;


uniform mat4 u_modelToWorldTransformation;
uniform mat4 u_cameraProjection;
uniform mat4 u_cameraView;

out vec3 p_modelWorldPosition;

void main()
{
	p_modelWorldPosition = vec3(u_modelToWorldTransformation * vec4(a_modelVertexPosition, 1.0));
	gl_Position = u_cameraProjection * u_cameraView * vec4(p_modelWorldPosition, 1.0);
}


#fragment_shader
#version 330


out vec4 o_Color;

void main()
{
	o_Color = vec4(1.0, 1.0, 0.0, 1.0);
}
