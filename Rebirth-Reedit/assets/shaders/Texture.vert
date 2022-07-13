#version 450 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in vec2 aTexCoord;
layout(location=3) in float aTexIndex;
layout(location=4) in float aTilingFactor;
layout(location=5) in int aEntityID;

out vec2 vTexCoord;
out vec4 vColor;
out flat float vTexIndex;
out flat int vEntityID;

uniform mat4 uViewProj;

	
void main()
{
	vTexCoord = aTexCoord * aTilingFactor;
	vColor = aColor;
	vTexIndex = aTexIndex;
	vEntityID = aEntityID;
	gl_Position = uViewProj * vec4(aPos, 1.0);
}