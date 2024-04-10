#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;

uniform mat4 transformMatrix;

void main()
{
	gl_Position = transformMatrix * vec4(aPos, 1.f);
	//gl_Position = vec4(aPos, 1.f);
	vertexColor = vec4(aColor, 1.f);
}