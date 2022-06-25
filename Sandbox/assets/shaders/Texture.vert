#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 uViewProj;
uniform mat4 uModelTransform;
uniform float uTilingFactor;

	
void main()
{
	vTexCoord = aTexCoord * uTilingFactor;
	gl_Position = uViewProj * uModelTransform * vec4(aPos, 1.0);
}