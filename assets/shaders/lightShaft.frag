#version 410 core

#define NUM_SAMPLES 512

in block
{
	vec2 Texcoord;
} In; 

uniform sampler2D Texture;
uniform vec2 ScreenLightPos;
uniform float Density = 5.0f;
uniform float Weight = 10.0f;
uniform float Decay = 0.9f;
uniform float Exposure = 1.0f;

layout(location = 0, index = 0) out vec4  Color;

void main(void)
{
  // Calculate vector from pixel to light source in screen space.  
   vec2 deltaTexCoord = (In.Texcoord - ScreenLightPos.xy);  
  // Divide by number of samples and scale by control factor.  
  deltaTexCoord *= 1.0f / NUM_SAMPLES * Density;  
  // Store initial sample.  
   vec3 color = texture(Texture, In.Texcoord).rgb;  
  // Set up illumination decay factor.  
   float illuminationDecay = 1.0f;
vec2 newCoord = In.Texcoord;  
  // Evaluate summation from Equation 3 NUM_SAMPLES iterations.  
   for (int i = 0; i < NUM_SAMPLES; i++)  
  {  
    // Step sample location along ray.  
    newCoord -= deltaTexCoord;  
    // Retrieve sample at new location.  
    vec3 sampl = texture(Texture, newCoord).rgb;  //sample is a protected keyword for openGL it seems
    // Apply sample attenuation scale/decay factors.  
    sampl *= illuminationDecay * Weight;  
    // Accumulate combined color.  
    color += sampl;  
    // Update exponential decay factor.  
    illuminationDecay *= Decay;  
  }  
  // Output final color with a further scale control factor.  
   Color = vec4( color * Exposure, 1.f);
   //Color = vec4( deltaTexCoord, 0.0, 1.0);  
   //Color = vec4(In.Texcoord, 0.0, 1.0);
   //Color = vec4(abs(In.Texcoord - ScreenLightPos.xy), 0.0, 1.0);
   //Color = texture(Texture, In.Texcoord);
}
