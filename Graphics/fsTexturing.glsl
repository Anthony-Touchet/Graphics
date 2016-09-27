#version 410

in vec2 vTexCoord;
out vec4 FragColor;
uniform sampler2D diffuse;
uniform sampler2D white;

void main() {
	FragColor = texture(diffuse, vTexCoord) * texture(white, vTexCoord);
}