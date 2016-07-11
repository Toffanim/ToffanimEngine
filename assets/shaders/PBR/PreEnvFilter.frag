#version 430 core

const float PI = 3.14159265359;

uniform samplerCube EnvMapSampler;
uniform float Roughness;

in vec4 FragPos;

layout(location = 0) out vec3 Color;

float radicalInverse_VdC(uint bits) {
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
 }
 // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
 vec2 Hammersley(uint i, uint N) {
     return vec2(float(i)/float(N), radicalInverse_VdC(i));
 }
 
 vec3 ImportanceSampleGGX(vec2 Xi, float Roughness, vec3 N)
 {
    float a = Roughness * Roughness;
	
	float Phi = 2 * PI * Xi.x;
	float CosTheta = sqrt( (1.0f - Xi.y) / ( 1 + (a*a - 1) * Xi.y ) ); 
	float SinTheta = sqrt( 1 - CosTheta*CosTheta);

	vec3 H;
	H.x = SinTheta * cos( Phi );
	H.y = SinTheta * sin( Phi );
	H.z = CosTheta;

	vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
	vec3 TangentX = normalize( cross( UpVector, N ) );
	vec3 TangentY = cross( N, TangentX );
	// Tangent to world space
	return TangentX * H.x + TangentY * H.y + N * H.z;
}

vec3 PrefilterEnvMap( float Roughness, vec3 R )
{
	vec3 N = R;
	vec3 V = R;
	vec3 PrefilteredColor = vec3(0.f);
	const uint NumSamples = 1024;
	float TotalWeight = 0.f;
	for( uint i = 0; i < NumSamples; i++ )
	{
		vec2 Xi = Hammersley( i, NumSamples );
		vec3 H = ImportanceSampleGGX( Xi, Roughness, N );
		vec3 L = 2 * dot( V, H ) * H - V;
		float NoL = max( dot( N, L ), 0.f );
		if( NoL > 0 )
		{
			PrefilteredColor += textureLod( EnvMapSampler , L, 0 ).rgb * NoL;
			TotalWeight += NoL;
		}
	}
	return PrefilteredColor / max(TotalWeight, 0.0001);
}

void main()
{
    vec3 R = normalize( FragPos.xyz );
    vec3 Result = PrefilterEnvMap( Roughness, R );
	Color = Result;
	//Color = vec3( Roughness );
}