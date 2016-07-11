#version 330 core
layout (location = 0) in vec3 position;

uniform int Face;
out vec3 TexCoords;
void main()
{
    gl_Position = vec4(position, 1.0);
	
	// THIS IS ABSOLUTLY WRONG
	// DONT DO THIS IN PRODUCTION
	if ( Face == 0 )
	{
	    //X+
	    TexCoords = vec3(1.f, position.y, position.x);
	}
	else if ( Face == 1 )
	{
	    //X-
	    TexCoords = vec3(-1.f, position.y, position.x);
	}
	else if ( Face == 2 )
	{
	    //Y+
	    TexCoords = vec3(position.x, 1.f, position.y);
	}
	else if ( Face == 3 )
	{
	    //Y-
	    TexCoords = vec3(position.x, -1.f, position.y);
	}
	else if ( Face == 4 )
	{
	    //Z+
	    TexCoords = vec3(position.x, position.y, 1.f);
	}
	else if ( Face == 5 )
	{
	    //Z-
	    TexCoords = vec3(position.x, position.y, -1.f);
	}
	
}   