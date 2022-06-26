#type vertex
#version 330 core

layout(location=0) in vec3 aPos;

uniform mat4 uViewProj;
uniform mat4 uModelTransform;

out vec3 vPos;

	
void main()
{
	vPos = aPos;
	gl_Position = uViewProj * uModelTransform * vec4(aPos, 1.0);
}


#type fragment
#version 330 core

layout(location=0) out vec4 color;

in vec3 vPos;

uniform vec4 uColor;

void main()
{
	color = uColor;
}