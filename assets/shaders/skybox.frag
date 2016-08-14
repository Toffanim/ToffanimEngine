#version 410 core
in vec3 TexCoords;
in vec4 pos;
out vec4 color;

uniform samplerCube skybox;
uniform sampler2D DepthBuffer;
uniform vec2 screenSize;


void main()
{
             color = texture(skybox, TexCoords);    
}
