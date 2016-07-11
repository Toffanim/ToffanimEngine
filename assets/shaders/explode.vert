#version 410 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2

layout(location = POSITION) in vec3 Position;
layout(location = NORMAL) in vec3 Normal;
layout(location = TEXCOORD) in vec2 TexCoord;

uniform float Time;
uniform mat4 MVP;
uniform mat4 MV;
uniform bool reverse_normal;


out block
{
	vec2 Texcoord;
	vec3 Position;
	vec3 Normal;
} Out;

void main()
{	
	Out.Texcoord = TexCoord;
	vec3 p = Position;
	vec3 n = Normal;
	if(reverse_normal)
	    n = -n;

    Out.Position = Position;
	Out.Normal = n;
	gl_Position = vec4(p, 1.0);

	//Out.CameraSpacePosition = vec3(MV * vec4(p, 1.0));
	//Out.CameraSpaceNormal = vec3(MV * vec4(n, 0.0));
	//gl_Position = MVP * vec4(p, 1.0);
}
