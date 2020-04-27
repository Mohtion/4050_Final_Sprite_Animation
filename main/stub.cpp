/******************************************************************************|
| CPSC 4050/6050 Computer Garphics Assignment 5, Daljit Singh Dhillon, 2020    |
| Reference:                                                                   |
|                                                                              |
| Some OpenGL setup code here including math_funcs, and gl_utils               |
| are from Angton Gerdelan and "Anton's OpenGL 4 Tutorials."                   |
| http://antongerdelan.net/opengl/                                             |
| Email: anton at antongerdelan dot net                                        |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
|******************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <math.h>
#include <time.h>

#include "maths_funcs.h"   // Anton's maths functions.
#include "gl_utils.h"      // Anton's opengl functions and small utilities like logs
#include "stb_image.h"     // Sean Barrett's image loader with Anton's load_texture()

using namespace std;

#define _USE_MATH_DEFINES
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

mat4 view_mat;
glm::mat4 proj_mat;
mat4 model_mat;

float user_light[3] = {0,0,0};
float user_spec = 0;
bool spec_on = 1;
bool texture_on = 0;
bool diffuse_on = 1;
typedef struct point{
	float xyz[3];
	float norm[3]; //x, y, z
	float uv[3];
	
	point()
    {}
} point;

const int triNum = 3;

typedef struct tri{
	point* v[triNum];

	tri(){};
} tri;

int size;
int ySteps = 2, thetaSteps = 2;

vector<point*> curvePoint;
vector< vector<point*> > surface;
vector<tri*> tris;

int pointCount;

//make list of tris
void makeTris (int rows, int cols){
	//go in circle, then down one row
	for(int i = 0; i < rows - 1; i++){
		for(int j = 0; j < cols - 1; j++){
			int currTri = (i * (cols) + j) * 2;
			tris[currTri] = new tri();
			tris[currTri + 1] = new tri();
			//add points counter clockwise
			tris[currTri]->v[0] = surface[i][j];
			tris[currTri]->v[1] = surface[i + 1][(j + 1) % (cols)];
			tris[currTri]->v[2] = surface[i + 1][j];
			tris[currTri + 1]->v[0] = surface[i][j];
			tris[currTri + 1]->v[1] = surface[i][(j + 1) % (cols)];
			tris[currTri + 1]->v[2] = surface[i + 1][(j + 1) % (cols)];
		}
	}
	//printf("\nerror not makeTris\n"); 
}

//draw rectangle
//makes rectangle placed in middle of coordinate system
void drawRectangle(float width, float height){
	float numFrames = 4;
	float halfwidth = width / 2;
	float halfheight = height / 2;
	surface[0][0] = new point();
	surface[0][1] = new point();
	surface[1][0] = new point();
	surface[1][1] = new point();
	surface[0][0]->xyz[0] = halfwidth;
	surface[0][0]->xyz[1] = halfheight;
	surface[0][0]->xyz[2] = 0;
	surface[0][0]->uv[0] = 0 + 1.0 / numFrames;
	surface[0][0]->uv[1] = 0 + 1.0 / numFrames;
	surface[0][1]->xyz[0] = -halfwidth;
	surface[0][1]->xyz[1] = halfheight;
	surface[0][1]->xyz[2] = 0;
	surface[0][1]->uv[0] = 0;
	surface[0][1]->uv[1] = 0 + 1.0 / numFrames;
	surface[1][0]->xyz[0] = halfwidth;
	surface[1][0]->xyz[1] = -halfheight;
	surface[1][0]->xyz[2] = 0;
	surface[1][0]->uv[0] = 0 + 1.0 / numFrames;
	surface[1][0]->uv[1] = 0;
	surface[1][1]->xyz[0] = -halfwidth;
	surface[1][1]->xyz[1] = -halfheight;
	surface[1][1]->xyz[2] = 0;
	surface[1][1]->uv[0] = 0;
	surface[1][1]->uv[1] = 0;
}
//draw tris of surface
//rows is num of vertex rows
//cols is num of vertex cols
void drawSurfaceTris(int rows, int cols){
	const int numTris = 2;
	tris = vector<tri*>(numTris);
	surface = vector< vector<point*> >(rows, vector<point*>(cols));
	//make surface
	float width = 400;
	float height = 400;
	drawRectangle(width, height);
	//printf("\nerror not drawingSurface\n");
	makeTris (rows, cols);
	/*
	if(runs == 0){
			for(int i = 0; i < curvePoint.size(); i++){
			printf("drawing \n");
			printf("%f \n",curvePoint[i]->x);
			}
			}
			*/
	//runs++;
	//printf("\nfinished drawingSurface %d\n", runs++); 
}

void loadSurfaceOfRevolution() 
{
/*------------------------------CREATE GEOMETRY-------------------------------*/
	drawSurfaceTris(ySteps, thetaSteps);
	printf("did we make it here\n");
	size = 2;
	int numElems = 3 + 2;
	//int numAttribs = 2;
	GLfloat *vp = new GLfloat[size * 3 * (numElems)];    // array of vertex points and normals
	//GLfloat *vn = new GLfloat[size * 9 * numAttribs];    // array of vertex normals
	for(int i = 0; i < size; i++){
		for(int j = 0; j < 3; j++){
			vp[i * 15 + j * 5] = tris[i]->v[j]->xyz[0];
			vp[i * 15 + j * 5 + 1] = tris[i]->v[j]->xyz[1];
			vp[i * 15 + j * 5 + 2] = tris[i]->v[j]->xyz[2];
			vp[i * 15 + j * 5 + 3] = tris[i]->v[j]->uv[0];
			vp[i * 15 + j * 5 + 4] = tris[i]->v[j]->uv[1];
			//printf("%f %f %f :: %f %f %f \n", vp[i * 9 + j * 6], vp[i * 9 + j * 6 + 1], vp[i * 9 + j * 6 + 2] , vp[i * 9 + j * 6 + 3], vp[i * 9 + j * 6 + 4], vp[i * 9 + j * 6 + 5]);
		}
	}
	printf("did we make it there\n");
/*
	GLfloat vp[18];
	//face 1, vertex 1
	vp[0] = -.1; //x
	vp[1] = -.1; //y
	vp[2] = 0; //z
	//face 1, vertex 2
	vp[3] = .1; //x
	vp[4] = -.1; //y
	vp[5] = 0; //z
	//face 1, vertex 3
	vp[6] = -.1; //x
	vp[7] =  .1; //y
	vp[8] =  0; //z
	
	//face 2, vertex 1
	vp[ 9] = -1; //x
	vp[10] =  1; //y
	vp[11] = 0; //z
	//face 2, vertex 2
	vp[12] =  1; //x
	vp[13] = -1; //y
	vp[14] = 0; //z
	//face 2, vertex 3
	vp[15] =  1; //x
	vp[16] =  1; //y
	vp[17] =  0; //z
*/	
	
	// VAO -- vertex attribute objects bundle the various things associated with vertices
	GLuint vao;
	glGenVertexArrays (1, &vao);   // generating and binding is common pattern in OpenGL
	glBindVertexArray (vao);       // basically setting up memory and associating it

	// VBO -- vertex buffer object to contain coordinates
	// MODIFY THE FOLLOWING BLOCK OF CODE APPRORIATELY FOR YOUR SURFACE OF REVOLUTION
	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * (3 + 2) * sizeof (float), vp, GL_STATIC_DRAW);
	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (numElems) * 
	sizeof(GLfloat), NULL);
/*
	//normals
	// Tell OpenGL how to locate the second attribute (texture coordinates) inside the buffer
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (numElems) * 
	sizeof(GLfloat),(void*)(3 * sizeof(GLfloat)));
*/	
	// VBO -- normals -- needed for shading calcuations
	// ADD CODE TO POPULATE AND LOAD PER-VERTEX SURFACE NORMALS  
	// [HINT] Vertex normals are organized in same order as that for vertex coordinates

    // VBO -- vt -- texture coordinates
	// ADD CODE TO POPULATE AND LOAD PER-VERTEX TEXTURE COORDINATES  
	// [HINT] texture coordinates are organized in same order as that for vertex coordinates
	// [HINT] there are two texture coordinates instead of three vertex coordinates for each vertex
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (numElems) * 
	sizeof(GLfloat),(void*)(3 * sizeof(GLfloat)));
}


	
void loadUniforms(GLuint shader_programme)
{	
/*---------------------------SET RENDERING DEFAULTS---------------------------*/

	// Choose vertex and fragment shaders to use as well as view and proj matrices.
	int model_mat_location = glGetUniformLocation (shader_programme, "model_mat");
	int view_mat_location  = glGetUniformLocation (shader_programme, "view_mat");
	int proj_mat_location  = glGetUniformLocation (shader_programme, "proj_mat");
	
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, glm::value_ptr(proj_mat));
	glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, model_mat.m);
	
	// WRITE CODE TO LOAD OTHER UNIFORM VARIABLES LIKE FLAGS FOR ENABLING OR DISABLING CERTAIN FUNCTIONALITIES
	int user_spec_location  = glGetUniformLocation (shader_programme, "user_spec");
	glUniform1f (user_spec_location, user_spec);
	int user_light_location  = glGetUniformLocation (shader_programme, "user_light");
	glUniform3fv (user_light_location, 1, user_light);
	int spec_on_location  = glGetUniformLocation (shader_programme, "spec_on");
	glUniform1i (spec_on_location, spec_on);
	int texture_on_location  = glGetUniformLocation (shader_programme, "texture_on");
	glUniform1i (texture_on_location, texture_on);
	int diffuse_on_location  = glGetUniformLocation (shader_programme, "diffuse_on");
	glUniform1i (diffuse_on_location, diffuse_on);
}

void drawSurfaceOfRevolution()
{
	// MODIFY THIS LINE OF CODE APPRORIATELY FOR YOUR SURFACE OF REVOLUTION
	glDrawArrays(GL_TRIANGLES, 0, 2 * 3 * (3 + 2));
}
	
void keyboardFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// MODIFY THIS FUNCTION FOR KEYBOARD INTERACTIVITY
	//GLFW Reference Links:
	// Callback Example: https://www.glfw.org/docs/3.3/input_guide.html#input_key
	// List of Keys: https://www.glfw.org/docs/3.3/group__keys.html
	
	//move light
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
		printf("\nLight -x \n");
		user_light[0] -= 1;
        // Example case. Key 'E' pressed. Doing nothing
	}

    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
		printf("\nLight -y\n");
		user_light[1] -= 1;
        // Example case. Key 'E' pressed. Doing nothing
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
		printf("\nLight +x\n");
		user_light[0] += 1;
        // Example case. Key 'E' pressed. Doing nothing
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
		printf("\nLight +y\n");
		user_light[1] += 1;
        // Example case. Key 'E' pressed. Doing nothing
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
		printf("\nLight -z\n");
		user_light[2] -= 1;
        // Example case. Key 'E' pressed. Doing nothing
	}
	else if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
		printf("\nLight +z\n");
		user_light[2] += 1;
        // Example case. Key 'E' pressed. Doing nothing
	}
	
	//togglers
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
    {
		if(spec_on){
			spec_on = 0;
			printf("\nspecular shading off\n");
		}
		else{
			spec_on = 1;
			printf("\nspecular shading on\n");
		}
	}
	else if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
		if(texture_on){
			texture_on = 0;
			printf("\ntexture albedo off\n");
		}
		else{
			texture_on = 1;
			printf("\ntexture albedo on\n");
		}
	} 
	else if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
		if(diffuse_on){
			diffuse_on = 0;
			printf("\ndiffuse shading off\n");
		}
		else{
			diffuse_on = 1;
			printf("\ndiffuse shading on\n");
		}
	} 

	//change specular exponent
	else if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
		user_spec -= 2;
		printf("\nspecular exponent -2\n");
	} 
	else if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
		user_spec += 2;
		printf("\nspecular exponent +2\n");
	} 
	if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
		// Close window when esacape is pressed
			glfwSetWindowShouldClose (g_window, 1);
	}

}
