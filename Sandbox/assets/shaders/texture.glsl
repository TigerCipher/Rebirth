#type vertex
#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCoord;

uniform mat4 uViewProj;
uniform mat4 uModelTransform;

out vec2 vTexCoord;
	
void main()
{
	vTexCoord = aTexCoord;
	gl_Position = uViewProj * uModelTransform * vec4(aPos, 1.0);
}


#type fragment
#version 330 core

layout(location=0) out vec4 color;

in vec2 vTexCoord;

uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture, vTexCoord);
}