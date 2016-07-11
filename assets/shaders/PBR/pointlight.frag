#version 410 core

const float PI = 3.14159265359;
const float ONE_OVER_PI = 0.318309;

uniform sampler2D ColorBuffer;
uniform sampler2D NormalAndMaterialsBuffer;
uniform sampler2D DepthBuffer;
uniform samplerCube Shadow;

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

float chiGGX(float v)
{
    return v > 0 ? 1 : 0;
}

float D_GGX(float ndoth, float roughness)
{
#if 0
    //UE4 version, not working for now
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
	float ndoth2 = ndoth * ndoth;
	float d = (ndoth2 * (alpha2-1) +1);
	float result = alpha2 / (PI * d * d );
#else
    float alpha2 = roughness * roughness;
    float ndoth2 = ndoth * ndoth;
    float den = ndoth2 * alpha2 + (1 - ndoth2);
    float result = (chiGGX(ndoth) * alpha2) / ( PI * den * den );
#endif
	return (result);
}

float G1( float dotproduct ,float k)
{
    return dotproduct / (dotproduct * (1-k)+k);
}

float G_Schlick(float ndotl, float ndotv, float roughness)
{
	float a = roughness + 1.0;
	float k = a * a * 0.125;
	k = roughness / 2.f;

	float G1_v = G1(ndotv, k);
	float G1_l = G1(ndotl, k);

	return (G1_l * G1_v);
}

vec3 F_Schlick(vec3 f0, float u)
{
	return f0 + (vec3(1.0) - f0) * pow(1.0 - u, 5.0);
}

vec4 BRDF_Lambertian(float ndotl, material Material)
{
	vec4 color = texture(ColorBuffer, In.Texcoord).rgba;
	color.rgb = mix(color.rgb, vec3(0.0), Material.Metalness); //If metlaness = 1., only specular, so Lambertian = 0
    color *= ONE_OVER_PI;
	return color;
}

vec3 BRDF_CookTorrance(float ldoth, float ndoth, float ndotv, float ndotl, material Material)
{
    vec4 color = texture(ColorBuffer, In.Texcoord).rgba;
	vec3 F0 = mix(vec3(1.5), color.rgb, Material.Metalness);
	vec3 F = F_Schlick(F0, ldoth);
	float G = G_Schlick(ndotl, ndotv, Material.Roughness);
	float D = D_GGX(ndoth, Material.Roughness);

	//return vec3(F);
	return (F * G * D) / ( 4 * ndotl * ndotv);
}

layout(location = 0, index = 0) out vec4 Color;

uniform mat4 InverseProjection;
uniform float farPlane;

uniform light
{
	vec3 Position;
	vec3 Color;
	float Intensity;
	float InverseRadius;
} PointLight;

vec4 pointLight(in vec3 n, in vec3 v, in vec3 ldir, in material Material )
{
    vec3 l = normalize(ldir);
	vec3 h = normalize(v + l);

	float ndotv = clamp(dot(n, v), 0.0, 1.0);
	float ndotl = clamp(dot(n, l), 0.0, 1.0);
	float ndoth = clamp(dot(n, h), 0.0, 1.0);
	float ldoth = clamp(dot(l, h), 0.0, 1.0);

	vec4 fd = BRDF_Lambertian(ndotl, Material);
	vec3 fs = BRDF_CookTorrance(ldoth, ndoth, ndotv, ndotl, Material);

	float dist		= length(ldir);
	float dist2		= max(dot(ldir, ldir), 1e-4);
	float falloff	= (PointLight.Intensity / dist2) * max(0.0, 1.0 - dist * PointLight.InverseRadius);

	float fade		= max(0.0, (fd.a - 0.75) * 4.0);
	float shadow	= mix(1.0, falloff, fade);

	vec4 color = texture(ColorBuffer, In.Texcoord).rgba;
	vec3 final_color = (fd.rgb * fd.a + fs) * ndotl * PointLight.Intensity;
	//return vec4(fs,1.f);
	return vec4(final_color, fd.a);
}

void main(void)
{
	vec4 PixelColor = texture(ColorBuffer, In.Texcoord).rgba;
	vec4 NormalAndMaterials = texture(NormalAndMaterialsBuffer, In.Texcoord).rgba;
	float Depth = texture(DepthBuffer, In.Texcoord).r;
	
	if ( Depth < 0.99)
	{
	
	vec3 BaseColor = PixelColor.rgb;
	material Material;
	Material.Metalness = NormalAndMaterials.z;
	Material.Roughness = NormalAndMaterials.a;
	

	vec2 xy = In.Texcoord * 2.0 -1.0;
	vec4 wP = InverseProjection * vec4(xy, Depth * 2.0 -1.0, 1.0);
	vec3 p = vec3(wP.xyz / wP.w);
	vec3 v = normalize(-p);
	vec3 n = normalize(DecodeNormal( NormalAndMaterials.xy ));
	vec3 ldir = PointLight.Position - p;
	
	
	float closestDepth = texture( Shadow, ldir ).r;
	closestDepth *= farPlane;
	float currentDepth = length(ldir);
	float shadow = currentDepth - 0.05 > closestDepth ? 0.0 : 1.0;
        
    Color =  shadow * vec4(pointLight(n, v, ldir, Material));
	Color =  vec4(pointLight(n, v, ldir, Material));
	}
	else
	{
	    Color = vec4(0.f);
    }
}
