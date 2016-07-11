#version 410 core

uniform sampler2D ColorBuffer;
uniform sampler2D NormalAndMaterialsBuffer;
uniform sampler2D DepthBuffer;

uniform samplerCube PreFilteredEnvironnementMap;
uniform sampler2D PreIntegratedBRDF;

uniform mat4 InverseProjection;
uniform unsigned int MipMapLevels = 6;

layout(location = 0, index = 0) out vec3 Color;

in block
{
	vec2 Texcoord;
} In; 



struct material
{
    float Metalness;
	float Roughness;
	float Transparency;
};

vec3 DecodeNormal( vec2 encodedNormal )
{     
    vec2 fenc = encodedNormal*4-2;
    float f = dot (fenc, fenc);
    float g = sqrt(1-f/4);
    vec3 n;
    n.xy = fenc *g;
    n.z = 1-f/2;
    return n;
}

void 
main()
{
    vec4 PixelColor = texture(ColorBuffer, In.Texcoord).rgba;
	vec4 NormalAndMaterials = texture(NormalAndMaterialsBuffer, In.Texcoord).rgba;
	float Depth = texture(DepthBuffer, In.Texcoord).r;
	
	vec3 BaseColor = PixelColor.rgb;
	material Material;
	Material.Metalness = NormalAndMaterials.z;
	Material.Roughness = NormalAndMaterials.a;
	

	vec2 xy = In.Texcoord * 2.0 -1.0;
	vec4 wP = InverseProjection * vec4(xy, Depth * 2.0 -1.0, 1.0);
	vec3 p = vec3(wP.xyz / wP.w);
	vec3 v = normalize(-p);
	vec3 n = normalize(DecodeNormal( NormalAndMaterials.xy ));
	float ndotv = clamp(dot(n, v), 0.0, 1.0);
	
    vec3 R = 2 * dot( v, n ) * n - v ;  
    float Roughness = Material.Roughness * (MipMapLevels-1);	
    vec3 PrefilteredColor = textureLod( PreFilteredEnvironnementMap, R, Roughness ).rgb;
    vec2 EnvBRDF = texture( PreIntegratedBRDF, vec2(Material.Roughness, ndotv) ).xy;

	vec3 SpecularColor = vec3(1.f);
    vec3 ApproximateSpecular = PrefilteredColor * ( SpecularColor * EnvBRDF.x + EnvBRDF.y );
	
	Color = ApproximateSpecular;
}