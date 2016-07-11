#version 430 core

out VS_OUT
{
    vec2 tc;
} vs_out;

uniform int InstanceCount;

void main(void)
{
    const vec4 vertices[] = vec4[](vec4(-0.5,-1.0, -0.5, 1.0),
                                   vec4( 0.5, -1.0, -0.5, 1.0),
                                   vec4(-0.5, -1.0,  0.5, 1.0),
                                   vec4( 0.5, -1.0,  0.5, 1.0));

    int x = gl_InstanceID & (InstanceCount-1);
    int y = gl_InstanceID / InstanceCount;
    vec2 offs = vec2(x, y);

    vs_out.tc = (vertices[gl_VertexID].xz + offs + vec2(0.5)) / InstanceCount;
    gl_Position = vertices[gl_VertexID] + vec4(float(x - (InstanceCount/2)), 0.0,
                                               float(y - (InstanceCount/2)), 0.0);
}