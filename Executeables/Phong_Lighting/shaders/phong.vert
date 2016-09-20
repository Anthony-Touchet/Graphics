// // classic Phong equation
#version 410

in vec4 position;
in vec4 colour;
in vec4 normal;

out vec4 vPosition;
out vec4 vColour;
out vec4 vNormal;
uniform mat4 ProjectionViewModel;
void main() 
{ 
	vec4 a = position;
	vPosition = position;
	vColour = colour; 
	vNormal = normal;
	gl_Position = ProjectionViewModel * position; 
}