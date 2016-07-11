#version 410 core

const float PI = 3.14159265359;
const unsigned int NUMBER_OF_SAMPLES = 1024;

in block
{
	vec2 Texcoord;
} In;

out vec4 Color;

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

// http://graphicrants.blogspot.com.au/2013/08/specular-brdf-reference.html
float GGX(float NdotV, float a)
{
	float k = a / 2;
	return NdotV / (NdotV * (1.0f - k) + k);
}

// http://graphicrants.blogspot.com.au/2013/08/specular-brdf-reference.html
float G_Smith(float a, float nDotV, float nDotL)
{
	return GGX(nDotL, a * a) * GGX(nDotV, a * a);
}

vec2 IntegrateBRDF( float Roughness, float NoV )
{
     vec3 V;
     V.x = sqrt( 1.0f - NoV * NoV ); // sin
     V.y = 0.0;
     V.z = NoV;	 // cos
	 
	 float A = 0.0;
	 float B = 0.0;

	 for( uint i = 0u; i < NUMBER_OF_SAMPLES; i++ )
	 {
		 vec2 Xi = Hammersley( i, NUMBER_OF_SAMPLES );
		 vec3 H = ImportanceSampleGGX( Xi, Roughness, vec3(0.0,0.0,1.0) );
		 vec3 L = 2.0 * dot( V, H ) * H - V;
		 
		 float NoL = max( L.z, 0.0 );
		 float NoH = max( H.z, 0.0 );
		 float VoH = max( dot( V, H ), 0.0 );

		if( NoL > 0 )
		{
			 //float G = G_Schlick(NoL, NoV, Roughness);
			 float G = G_Smith(Roughness, NoV, NoL);
			 float G_Vis = G * VoH / (NoH * NoV);
			 float Fc = pow( 1.0 - VoH, 5.0 );
			 A += (1.0 - Fc) * G_Vis;
			 B += Fc * G_Vis;
		}
	 }
	return vec2(A, B) / float(NUMBER_OF_SAMPLES) ;
}

void main()
{
    vec2 Result = IntegrateBRDF( In.Texcoord.x, In.Texcoord.y); 
    Color = vec4( Result, 0.f, 1.f);
}