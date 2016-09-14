#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
out vec4 vColour;
uniform mat4 projectionViewWorldMatrix;
uniform float Time;
void main() 
{ 
	vec4 a = position * 5 +  cos(Time);
	vColour = colour;
	gl_Position = projectionViewWorldMatrix * a; 

}