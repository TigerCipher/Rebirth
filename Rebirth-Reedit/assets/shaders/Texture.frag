#version 450 core

layout(location=0) out vec4 color;
layout(location=1) out int color2;

struct VertexOutput
{
	vec4 color;
	vec2 texCoord;
	float texIndex;
	float padding;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int vEntityID;

layout(binding = 0) uniform sampler2D uTextures[32];

void main()
{
	color = texture(uTextures[int(Input.texIndex)], Input.texCoord) * Input.color;
	color2 = vEntityID;
}