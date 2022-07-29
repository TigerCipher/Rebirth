#type vertex
#version 450 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in int aEntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 uViewProj;
};

struct VertexOutput
{
	vec4 color;
};



layout(location = 0) out VertexOutput Output;
layout(location = 1) out flat int vEntityID;

	
void main()
{
	Output.color = aColor;
	vEntityID = aEntityID;
	gl_Position = uViewProj * vec4(aPos, 1.0);
}

#type fragment
#version 450 core

layout(location=0) out vec4 outColor;
layout(location=1) out int outEntityID;

struct VertexOutput
{
	vec4 color;
};

layout(location = 0) in VertexOutput Input;
layout(location = 1) in flat int vEntityID;


void main()
{
	outColor = Input.color;
	outEntityID = vEntityID;
}