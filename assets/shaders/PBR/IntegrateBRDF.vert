#version 430 core

out VS_OUT
{
    vec2 tc;
} vs_out;

void main(void)
{
    const vec4 vertices[] = vec4[](vec4(-0.5,-1.0, -0.5, 1.0),
                                   vec4( 0.5, -1.0, -0.5, 1.0),
                                   vec4(-0.5, -1.0,  0.5, 1.0),
                                   vec4( 0.5, -1.0,  0.5, 1.0));

    vs_out.tc = (vertices[gl_VertexID].xz + vec2(1)) / 2.f;
    gl_Position = vertices[gl_VertexID];
}