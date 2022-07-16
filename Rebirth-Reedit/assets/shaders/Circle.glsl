#type vertex
#version 450 core

layout(location=0) in vec3 aWorldPos;
layout(location=1) in vec3 aLocalPos;
layout(location=2) in vec4 aColor;
layout(location=3) in float aThickness;
layout(location=4) in float aFade;
layout(location=5) in int aEntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 uViewProj;
};

struct VertexOutput
{
	vec3 localPos;
	vec4 color;
	float thickness;
	float fade;
};



layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int vEntityID;

	
void main()
{
	Output.localPos = aLocalPos;
	Output.color = aColor;
	Output.thickness = aThickness;
	Output.fade = aFade;
	vEntityID = aEntityID;
	gl_Position = uViewProj * vec4(aWorldPos, 1.0);
}

#type fragment
#version 450 core

layout(location=0) out vec4 outColor;
layout(location=1) out int outEntityID;

struct VertexOutput
{
	vec3 localPos;
	vec4 color;
	float thickness;
	float fade;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int vEntityID;


void main()
{
	float distance = 1.0 - length(Input.localPos);
	float alpha = smoothstep(0.0, Input.fade, distance);
	alpha *= smoothstep(Input.thickness + Input.fade, Input.thickness, distance);

	if(alpha == 0.0)
		discard;

	outColor = Input.color;
	outColor.a *= alpha;

	outEntityID = vEntityID;
}