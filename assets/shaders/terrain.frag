#version 430 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define OCCLUDEE        2
#define COLOR	    0

const float PI = 3.14159265359;
const float TWOPI = 6.28318530718;
const float PI_2 = 1.57079632679;
const float DEG2RAD = TWOPI / 360.0;

precision highp int;

uniform float Time;
uniform sampler2D Diffuse;
uniform sampler2D Specular;
uniform float SpecularPower;
uniform vec3 PixColor;
uniform bool UsePixColor;

layout(location = COLOR ) out vec4 Color;
layout(location = NORMAL) out vec4 Normal;

out vec4 color;

layout (binding = 1) uniform sampler2D tex_color;
layout (binding = 0) uniform sampler2D tex_displacement;

in TES_OUT
{
    vec3 CameraSpaceNormal;
    vec2 tc;
} fs_in;

void main(void)
{
    vec3 n = normalize(fs_in.CameraSpaceNormal);
    Color = texture(tex_color, fs_in.tc);
	if(UsePixColor)
	    Color = vec4(PixColor, 1.0);
	Normal = vec4( n, SpecularPower);
	//Color = Normal;
}