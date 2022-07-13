#version 450 core

layout(location=0) out vec4 color;
layout(location=1) out int color2;

in vec2 vTexCoord;
in vec4 vColor;
in flat float vTexIndex;

uniform sampler2D uTextures[32];

void main()
{
	color = texture(uTextures[int(vTexIndex)], vTexCoord) * vColor;
	color2 = 50;
}