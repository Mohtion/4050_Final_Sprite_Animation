#version 410
uniform mat4 proj_mat, view_mat, model_mat;
uniform vec3 user_light;
uniform bool spec_on, texture_on, diffuse_on;
uniform float user_spec;
//uniform sampler2D Image;
// Light position, in camera coordinates
//vec3 LightPosition = vec3(view_mat);
// The color of our light
//vec3 LightColor = vec3(1.0, 1.0, 1.0);
// And from the uniform outputs for the textures setup in main.cpp.
uniform sampler2D texture00;
uniform sampler2D texture01;

smooth in vec2 TextCoordInterp;
smooth in vec3 PositionInterp;
out vec4 FBColor;
void main()
{
    //texture
    //vec4 textureColor = texture(texture00, TextCoordInterp);
    //specular contribution

    /* Apply the mask to the specular contribution. The "1.0 –" is
    To invert the texture's alpha channel */
    //vec3 lightContribution = vec3(textureColor.a;// – 0.5);//textureColor.a);
    FBColor = texture(texture00, TextCoordInterp);//vec4(textureColor.rgba);
}
