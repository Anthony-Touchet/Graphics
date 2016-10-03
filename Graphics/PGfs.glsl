#version 410

in vec2 frag_texcoord;

out vec4 out_color;
uniform sampler2D otherTexture;

void main()
{
	out_color = texture(otherTexture, frag_texcoord);
}