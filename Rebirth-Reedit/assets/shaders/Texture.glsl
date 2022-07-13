#type vertex
#version 450 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in vec2 aTexCoord;
layout(location=3) in float aTexIndex;
layout(location=4) in float aTilingFactor;
layout(location=5) in int aEntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 uViewProj;
};

struct VertexOutput
{
	vec4 color;
	vec2 texCoord;
	float texIndex;
	float padding;
};



layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int vEntityID;



	
void main()
{
	Output.texCoord = aTexCoord * aTilingFactor;
	Output.color = aColor;
	Output.texIndex = aTexIndex;
	vEntityID = aEntityID;
	gl_Position = uViewProj * vec4(aPos, 1.0);
}

#type fragment
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
	vec4 texColor = Input.color;

	switch(int(Input.texIndex))
	{
		case  0: texColor *= texture(uTextures[ 0], Input.texCoord); break;
		case  1: texColor *= texture(uTextures[ 1], Input.texCoord); break;
		case  2: texColor *= texture(uTextures[ 2], Input.texCoord); break;
		case  3: texColor *= texture(uTextures[ 3], Input.texCoord); break;
		case  4: texColor *= texture(uTextures[ 4], Input.texCoord); break;
		case  5: texColor *= texture(uTextures[ 5], Input.texCoord); break;
		case  6: texColor *= texture(uTextures[ 6], Input.texCoord); break;
		case  7: texColor *= texture(uTextures[ 7], Input.texCoord); break;
		case  8: texColor *= texture(uTextures[ 8], Input.texCoord); break;
		case  9: texColor *= texture(uTextures[ 9], Input.texCoord); break;
		case 10: texColor *= texture(uTextures[10], Input.texCoord); break;
		case 11: texColor *= texture(uTextures[11], Input.texCoord); break;
		case 12: texColor *= texture(uTextures[12], Input.texCoord); break;
		case 13: texColor *= texture(uTextures[13], Input.texCoord); break;
		case 14: texColor *= texture(uTextures[14], Input.texCoord); break;
		case 15: texColor *= texture(uTextures[15], Input.texCoord); break;
		case 16: texColor *= texture(uTextures[16], Input.texCoord); break;
		case 17: texColor *= texture(uTextures[17], Input.texCoord); break;
		case 18: texColor *= texture(uTextures[18], Input.texCoord); break;
		case 19: texColor *= texture(uTextures[19], Input.texCoord); break;
		case 20: texColor *= texture(uTextures[20], Input.texCoord); break;
		case 21: texColor *= texture(uTextures[21], Input.texCoord); break;
		case 22: texColor *= texture(uTextures[22], Input.texCoord); break;
		case 23: texColor *= texture(uTextures[23], Input.texCoord); break;
		case 24: texColor *= texture(uTextures[24], Input.texCoord); break;
		case 25: texColor *= texture(uTextures[25], Input.texCoord); break;
		case 26: texColor *= texture(uTextures[26], Input.texCoord); break;
		case 27: texColor *= texture(uTextures[27], Input.texCoord); break;
		case 28: texColor *= texture(uTextures[28], Input.texCoord); break;
		case 29: texColor *= texture(uTextures[29], Input.texCoord); break;
		case 30: texColor *= texture(uTextures[30], Input.texCoord); break;
		case 31: texColor *= texture(uTextures[31], Input.texCoord); break;
	}
	color = texColor;
	color2 = vEntityID;
}