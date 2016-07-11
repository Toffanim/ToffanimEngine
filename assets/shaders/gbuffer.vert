#version 410 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2

layout(location = POSITION) in vec3 Position;
layout(location = NORMAL) in vec3 Normal;
layout(location = TEXCOORD) in vec2 TexCoord;

uniform mat4 MVP;
uniform mat4 MV;
uniform bool ReverseNormals;


out block
{
	vec2 Texcoord;
	vec3 CameraSpacePosition;
	vec3 CameraSpaceNormal;
} Out;

void main()
{	
	Out.Texcoord = TexCoord;
	vec3 p = Position;
	vec3 n = Normal;
	if(ReverseNormals)
	    n = -n;

	Out.CameraSpacePosition = vec3(MV * vec4(p, 1.0));
	Out.CameraSpaceNormal = vec3(MV * vec4(n, 0.0));
	gl_Position = MVP * vec4(p, 1.0);
}
