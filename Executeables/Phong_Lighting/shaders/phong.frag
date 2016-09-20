// classic Phong equation
#version 410

in vec4 vPosition;
in vec4 vColour;
in vec4 vNormal;
out vec4 fragColor;

uniform mat4 ProjectionViewModel;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform vec3 lightDirection;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 Ia;
uniform vec3 cameraPosition;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

void main() {	
   
    vec3 red = vec3(250,0,0);
    vec3 green = vec3(0, 250, 0); 
    vec3 blue = vec3(0, 0, 250);

	vec3 Lm = normalize(lightDirection);
	vec3 N = normalize(vNormal.xyz);
	vec3 R = normalize(2 * (dot(N, Lm)) * N - Lm);
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	
	float a = dot(N,vec3(0,1.f,0));
    vec3 hemisphere = .5f * mix(red, blue, a) + .5f;
	
	float lambertTerm = max(0, dot(N, Lm));
	float specularTerm = pow(max( 0, dot( R, V ) ), specularPower);
   
    vec3 ambient = (Ia * .01f) * (Ka) * hemisphere;
    vec3 diffuse = Id  * Kd  * lambertTerm;
    vec3 specular = Is * Ks * specularTerm;

	fragColor = vec4( ambient + diffuse + specular, 1);
}