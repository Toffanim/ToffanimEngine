#version 410 core
in vec3 TexCoords;
in vec4 pos;
out vec4 color;

uniform samplerCube skybox;
uniform sampler2D DepthBuffer;
uniform vec2 screenSize;


void main()
{
	// Converting (x,y,z) to range [0,1]
	float x = gl_FragCoord.x/screenSize.x;
	float y = gl_FragCoord.y/screenSize.y;
	float z = gl_FragCoord.z; // Already in range [0,1]


    float depth = texture(DepthBuffer, vec2(x, y)).r;
	//color = texture(skybox, TexCoords);
    if ( depth >= 0.99999999999 )
    {
             color = texture(skybox, TexCoords);    
    }
}
