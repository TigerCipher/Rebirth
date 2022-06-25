#version 330 core

layout(location=0) out vec4 color;

in vec2 vTexCoord;
in vec4 vColor;

uniform vec4 uColor;
uniform sampler2D uTexture;

void main()
{
	//color = texture(uTexture, vTexCoord) * uColor;
	color = vColor;
}