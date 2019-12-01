#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;  // You will need this when you do lighting
layout(location = 2) in vec3 vertexColor; 

// Values that stay constant for the whole mesh.
uniform mat4 ViewProjectionTransform;
uniform mat4 WorldTransform;

// Outputs to fragment shader
out vec3 v_normal;
out vec3 v_color;
out vec3 v_pos;

void main()
{
	gl_Position =  ViewProjectionTransform * WorldTransform * vec4(vertexPosition_modelspace, 1.0f);
	v_pos = vec3(WorldTransform * vec4(vertexPosition_modelspace, 1.0f));
	v_normal = mat3(transpose(inverse(WorldTransform))) * vertexNormal_modelspace;
	v_color = vertexColor;
}

