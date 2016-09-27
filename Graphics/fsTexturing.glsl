#version 410

in vec2 vTexCoord;
out vec4 FragColor;
uniform sampler2D diffuse;
uniform sampler2D white;

void main() {

	vec4 a = texture(diffuse, vTexCoord);
	vec4 b = texture(white, vTexCoord);

	vec3 newA = b.a * b.rgb;
	vec3 newB = a.rgb * -(b.a);

	FragColor = vec4(mix(newA, newB, .5f), 1);
}