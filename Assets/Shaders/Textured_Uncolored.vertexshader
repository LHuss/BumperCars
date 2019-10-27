#version 330 core

layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 3) in vec2 aUV;

uniform mat4 ViewProjectionTransform;
uniform mat4 WorldTransform;

out vec2 vertexUV;

void main()
{
   gl_Position = ViewProjectionTransform * WorldTransform * vec4(vertexPosition_modelspace,1);
   vertexUV = aUV;
}