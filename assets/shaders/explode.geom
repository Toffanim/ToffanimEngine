#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in block
{
	vec2 Texcoord;
	vec3 Position;
	vec3 Normal;
} gs_in[]; 

out block
{
	vec2 Texcoord;
	vec3 CameraSpacePosition;
	vec3 CameraSpaceNormal;
} Out;

uniform float Time;
uniform mat4 MVP;
uniform mat4 MV;
uniform bool reverse_normal;
uniform mat4 inverse_proj;
uniform vec3 origin = vec3( 0.f, 0.f, 0.f );
uniform float magnitude;
uniform bool reverse;
uniform float reverse_from_max;


vec4 explode(vec4 position, vec3 normal)
{
    vec3 direction = normal * log( 1.f + Time) * magnitude;
    if(reverse)
	 direction = normal * (reverse_from_max / exp(exp(Time))); 
    return position + vec4(direction, 0.0f);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {

    vec3 normal = GetNormal();
	normal = normalize( vec3(gl_in[0].gl_Position) - origin );

    gl_Position = MVP * explode(gl_in[0].gl_Position, normal);
    Out.Texcoord = gs_in[0].Texcoord;
	Out.CameraSpaceNormal = vec3( MV * vec4(gs_in[0].Normal,0.f));
	Out.CameraSpacePosition = vec3( MV * gl_Position );
    EmitVertex();
    gl_Position = MVP * explode(gl_in[1].gl_Position, normal);
    Out.Texcoord = gs_in[1].Texcoord;
	Out.CameraSpaceNormal = vec3( MV * vec4(gs_in[1].Normal,0.f));
	Out.CameraSpacePosition =  vec3( MV * gl_Position );
    EmitVertex();
    gl_Position = MVP * explode(gl_in[2].gl_Position, normal);
    Out.Texcoord = gs_in[2].Texcoord;
	Out.CameraSpaceNormal = vec3( MV * vec4(gs_in[2].Normal,0.f));
	Out.CameraSpacePosition =  vec3( MV * gl_Position );
    EmitVertex();
    EndPrimitive();
}