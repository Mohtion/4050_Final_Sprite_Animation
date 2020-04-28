#version 410


// Define INPUTS from fragment shader

uniform sampler2D texture00;

<<<<<<< HEAD
smooth in vec2 TextCoordInterp;
//smooth in vec3 PositionInterp;
out vec4 FBColor;
void main()
{
    //texture
    vec4 textureColor = texture(texture00, TextCoordInterp);
    //specular contribution

    /* Apply the mask to the specular contribution. The "1.0 –" is
    To invert the texture's alpha channel */
    //vec3 lightContribution = vec3(textureColor.a;// – 0.5);//textureColor.a);
    FBColor = textureColor;//vec4(textureColor.rgba);
=======
in vec2 tex1;

out vec4 fragment_color; //RGBA color
void main () {

   vec4 texColor = texture(texture00, tex1);
   vec3 texture = vec3(normalize(texColor));

   fragment_color = vec4(texture, 0.0);
>>>>>>> f16334a2d8180580581c70a4d97b5afb81016459
}
