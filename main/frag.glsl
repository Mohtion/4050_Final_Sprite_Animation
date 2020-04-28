#version 410


// Define INPUTS from fragment shader

uniform sampler2D texture00;

in vec2 tex1;

out vec4 fragment_color; //RGBA color
void main () {

   vec4 texColor = texture(texture00, tex1);
   vec3 texture = vec3(normalize(texColor));

   fragment_color = vec4(texture, 0.0);
}
