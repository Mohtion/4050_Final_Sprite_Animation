#version 410

layout (location = 0) in vec3 vtxPosition;
//layout (location = 1) in vec3 Normal;
layout (location = 1) in vec2 TextCoord;

uniform mat4 proj_mat, model_mat;
smooth out vec2 TextCoordInterp;
//smooth out vec3 NormalInterp;
//smooth out vec3 PositionInterp;

void main () {
	//PositionInterp = (/*view_mat */ model_mat * vec4(vtxPosition, 1.0)).xyz;
	TextCoordInterp = TextCoord;
	//normals transform
	//mat3 NormalMatrix = mat3(transpose(inverse(model_mat)));
	//NormalInterp = NormalMatrix * Normal;
	gl_Position = proj_mat * model_mat * vec4(vtxPosition, 1.0);
}
