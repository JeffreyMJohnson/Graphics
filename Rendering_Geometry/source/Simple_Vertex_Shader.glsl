#version 330

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;

uniform mat4 ProjectionView;

out vec4 vColor;

void main()
{
	vColor = Color;
	gl_Position = ProjectionView * Position;
}