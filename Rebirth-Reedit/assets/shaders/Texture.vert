#version 450 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in vec2 aTexCoord;
layout(location=3) in float aTexIndex;
layout(location=4) in float aTilingFactor;
layout(location=5) in int aEntityID;


struct VertexOutput
{
	vec4 color;
	vec2 texCoord;
	float texIndex;
	float padding;
};



layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int vEntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 uViewProj;
};

	
void main()
{
	Output.texCoord = aTexCoord * aTilingFactor;
	Output.color = aColor;
	Output.texIndex = aTexIndex;
	vEntityID = aEntityID;
	gl_Position = uViewProj * vec4(aPos, 1.0);
}