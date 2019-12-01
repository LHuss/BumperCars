#version 330 core

in vec3 v_color;
in vec2 vertexUV;
uniform sampler2D textureSampler;

out vec4 FragColor;
void main()
{
	
	vec4 textureColor = texture( textureSampler, vertexUV );
	FragColor = textureColor * vec4(v_color.r, v_color.g, v_color.b, 1.0f);
}