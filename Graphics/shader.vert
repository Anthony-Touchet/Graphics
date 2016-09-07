#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
out vec4 vColour;
uniform mat4 projectionViewWorldMatrix;
uniform float Time;
void main() 
{ 
	vec4 a = position;
	//newP.x += cos(Time) + sin(Time);
	//newP.y += sin(Time) - cos(Time);
	//newP.z += sin(Time) / cos(Time);
	vColour = colour; 
	//a.x = a.x * cos(Time) - a.y * sin(Time); //* 2.5f;
	//a.y = a.y * cos(Time) + a.x * sin(Time) * 2.35f;
	gl_Position = projectionViewWorldMatrix * a; 

}