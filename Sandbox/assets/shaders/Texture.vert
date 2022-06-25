#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in vec2 aTexCoord;

out vec2 vTexCoord;
out vec4 vColor;

uniform mat4 uViewProj;
uniform float uTilingFactor;

	
void main()
{
	vTexCoord = aTexCoord * uTilingFactor;
	vColor = aColor;
	gl_Position = uViewProj * vec4(aPos, 1.0);
}