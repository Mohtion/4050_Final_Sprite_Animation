#version 410

layout (location = 0) in vec3 vtxPosition;
layout (location = 2) in vec2 vtxTexture1;

uniform mat4 proj_mat, view_mat, model_mat;

out vec2 tex1;

void main ()
{
   gl_Position = proj_mat*view_mat*model_mat*vec4(vtxPosition, 1.0);
   tex1 = vtxTexture1;
}

