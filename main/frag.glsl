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
smooth in vec3 NormalInterp;
smooth in vec3 PositionInterp;
out vec4 FBColor;
void main()
{
    vec3 basePosition = vec3(0.0f, 0.0f, -5.0f);
    vec3 LightPosition = basePosition + user_light;
    vec3 LightColor = vec3(1.0, 1.0, 1.0);
    /* After interpolation, normals probably are denormalized,
    so we need renormalize them */
    vec3 normal = normalize(NormalInterp);

    /*Calculate the rest of parameters exactly like we did in
    the
    vertex shader version of this shader*/
    vec3 lightVec = normalize(LightPosition - PositionInterp);
    vec3 viewVec = normalize(vec3(0.0f, 0.0f, -5.0f) - PositionInterp);
    //vec3 reflectVec = reflect(-lightVec, normal);
    vec3 halfwayDir = normalize(lightVec + viewVec);
    float spec = max(dot(normal, halfwayDir), 0.0);//max(dot(reflectVec, viewVec), 0.0);
    spec = pow(spec, 16.0 + user_spec);
    vec4 textureColor = texture(texture00, TextCoordInterp);

    vec3 specContrib = LightColor * spec;

    // No ambient contribution this time
    vec3 ambientContrib = vec3(0.0, 0.0, 0.0);
    vec3 diffContrib = LightColor * max(dot(lightVec, normal), 0.0);

    /* Apply the mask to the specular contribution. The "1.0 –" is
    To invert the texture's alpha channel */
    vec3 lightContribution = ambientContrib + (texture_on ? textureColor.rgb : (diffuse_on ? diffContrib : vec3(0,0,0))) + 
        (spec_on ? specContrib /* (1.0 - (texture_on ? textureColor.a : 0))*/ : vec3(0,0,0));// – 0.5);//textureColor.a);
    FBColor = vec4(textureColor.rgb * lightContribution, 1.0);
}
