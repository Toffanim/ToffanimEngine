#version 430

uniform int MipMapLevel;
uniform samplerCube CubeMap;
in vec3 TexCoords;
out vec4 Color;

void main()
{
    Color = textureLod(CubeMap, TexCoords, MipMapLevel);
	//Color = texture(CubeMap, TexCoords, 5.0);
	//Color = vec4(MipMapLevel);
}