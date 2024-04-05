#version 330 core
out vec4 FragColor;

uniform vec4 polyColor;

in vec4 vertexColor;

void main()
{
	// FragColor = polyColor;
	FragColor = vertexColor;
}