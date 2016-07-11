#version 410 core

#define POSITION 0

layout(location = POSITION) in vec2 Position;

out block
{
	vec2 Texcoord;
} Out;

void main()
{	
    vec2 Coords = Position * 0.5 + 0.5;
	Coords.y = 1.f - Coords.y;
	Out.Texcoord = Coords;
	gl_Position = vec4(Position.x, Position.y, 0.0, 1.0);
}