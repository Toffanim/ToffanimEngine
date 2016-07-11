#version 410 core

#define COLOR	    0
#define NORMALANDMATERIALS		1

const float PI = 3.14159265359;
const float TWOPI = 6.28318530718;
const float PI_2 = 1.57079632679;
const float DEG2RAD = TWOPI / 360.0;

precision highp int;


layout(location = COLOR ) out vec4 Color;
layout(location = NORMALANDMATERIALS) out vec4 NormalAndMaterials;

in block
{
	vec2 Texcoord;
	vec3 CameraSpacePosition;
	vec3 CameraSpaceNormal;
} In; 

struct material
{
    float Metalness;
	float Roughness;
	sampler2D Texture;
	vec3 BaseColor;
	int HasTexture;
	float Transparency;
};

uniform material Material;

vec2 EncodeNormal( vec3 n )
{
    float f = sqrt(8*n.z+8);
    return n.xy / f + 0.5;
}

void main()
{
	vec3 n = normalize(In.CameraSpaceNormal);
	
	// gl_FrontFacing is not working on MacOSX Intel cards
	// Try this instead :
#if 0	
	if (dot(n, normalize(-In.CameraSpacePosition)) < 0.)
 #else
	if (!gl_FrontFacing)
#endif
		n = -n;
		
		
	//vec3 diffuseColor = mix( Material.BaseColor, texture(Material.Texture, In.Texcoord).rgb, Material.HasTexture);
	vec3 diffuseColor = Material.BaseColor;
	Color = vec4(diffuseColor, 1.f);
	NormalAndMaterials = vec4(EncodeNormal(n), Material.Metalness, Material.Roughness);
	
	//Color = vec4( 1.f);
	//NormalAndMaterials = vec4( n, 1.f);
}
