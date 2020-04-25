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


//#include <math.h>
#include <time.h>

#include "maths_funcs.h"   // Anton's maths functions.
#include "gl_utils.h"      // Anton's opengl functions and small utilities like logs
#include "stb_image.h"     // Sean Barrett's image loader with Anton's load_texture()

using namespace std;

#define _USE_MATH_DEFINES
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

mat4 view_mat;
mat4 proj_mat;
mat4 model_mat;

float user_light[3] = {0,0,0};
float user_spec = 0;
bool spec_on = 1;
bool texture_on = 0;
bool diffuse_on = 1;
typedef struct point{
	float x;
	float y;
	float z;
	float norm[3]; //x, y, z
	float u;
	float v;
	point()
    : x(0)
    , y(0)
    , z(0)
    {}
  point(float X, float Y, float Z)
  : x(X)
  , y(Y)
  , z(Z)
  {}
} point;

const int triNum = 3;

typedef struct tri{
	point* v[triNum];

	tri(){};
} tri;

int size;
int ySteps = 20, thetaSteps = 30;
float ptX[4] = {40, 60, 20, 40};
float ptY[4] = {-40, -10, 20, 50};

vector<point*> curvePoint;
vector< vector<point*> > surface;
vector<tri*> tris;

int pointCount;

void drawCubicBezier(float* ptX, float* ptY, int rows) {
	//replace above line with your code
	float x = (float)ptX[0] / 100, y = (float)ptY[0] / 100;
	float t = 0;
	int i = 0;
	while(t <= 1){
		curvePoint[i] = new point();
		curvePoint[i]->x = x; curvePoint[i]->y = y; curvePoint[i]->z = 0;
		//calculate normal
		float mag = sqrt(
							pow(
								3 * pow(1 - t, 2)* (ptX[1] - ptX[0]) + 
								6 * (1 - t) * t * (ptX[2] - ptX[1]) +
								3 * pow(t, 2) * (ptX[3] - ptX[2]), 2
							)
							+
							pow(
								3 * pow(1 - t, 2)* (ptY[1] - ptY[0]) + 
								6 * (1 - t) * t * (ptY[2] - ptY[1]) +
								3 * pow(t, 2) * (ptY[3] - ptY[2]), 2
							)
						);
			//calculate derivatives and normalize 
			//they're flipped and dy is negative to make normal instead of tangent
		float dy = -(3 * pow(1 - t, 2)* (ptX[1] - ptX[0]) + 
								6 * (1 - t) * t * (ptX[2] - ptX[1]) +
								3 * pow(t, 2) * (ptX[3] - ptX[2])) / mag;
		float dx = (3 * pow(1 - t, 2)* (ptY[1] - ptY[0]) + 
								6 * (1 - t) * t * (ptY[2] - ptY[1]) +
								3 * pow(t, 2) * (ptY[3] - ptY[2])) / mag;
		float tStep = 1.0 / rows;
		t += tStep;
		curvePoint[i]->norm[0] = dx;
		curvePoint[i]->norm[1] = dy; 
		curvePoint[i]->norm[2] = 0;
		x = (pow(1 - t, 3)*ptX[0] + 
			3 * pow(1 - t, 2) * t * ptX[1] +
			3 * (1 - t) * pow(t, 2) * ptX[2] + 
			+ pow(t, 3) * ptX[3]) / 100;
		y = (pow(1 - t, 3)*ptY[0] + 
			3 * pow(1 - t, 2) * t * ptY[1] +
			3 * (1 - t) * pow(t, 2) * ptY[2] + 
			+ pow(t, 3) * ptY[3]) / 100;
		/*if(runs == 0){
			printf("%f %f %f %d %f \n", t, x, y, i, curvePoint[i]->x);
		}*/
		i++;/*
		if(runs == 0){
			for(int j = 0; j < curvePoint.size(); j++){
			printf("Bezier \n");
			printf("%f %d\n",curvePoint[j]->x, j);
			}
		}*/
	}
	//printf("\nerror not bezier\n"); 
}

//rows must equal rows of curve
void surfaceOfRotation(int rows, int cols){
	//rotate points around in circle
	for(int i = 0; i < rows; i++){
		
		float d = 0 ;
		for(int j = 0; j < cols; j++){
			//add points to surface
			//change the x and z
			//start now
			surface[i][j] = new point();
			surface[i][j]->x = cos(d) * curvePoint[i]->x;
			surface[i][j]->y = curvePoint[i]->y;
			surface[i][j]->z = sin(d) * curvePoint[i]->x;
			surface[i][j]->norm[0] = cos(d) * curvePoint[i]->norm[0];
			surface[i][j]->norm[1] = curvePoint[i]->norm[1];
			surface[i][j]->norm[2] = sin(d) * curvePoint[i]->norm[0];
			surface[i][j]->u = (float)j / cols;
			surface[i][j]->v = (float)i / rows;
			d += (float)2 * M_PI / cols;
			printf("%f %f %f %d\n", surface[i][j]->norm[0], surface[i][j]->norm[1], surface[i][j]->norm[2], i);
			//printf("%f %f \n", surface[i][j]->u, surface[i][j]->v);
		}
		//if(i == rows) printf("\nerror not SOR\n");
	}
	
	//printf("\nerror not SOR\n"); 
}

//make list of tris
void makeTris (int rows, int cols){
	//go in circle, then down one row
	for(int i = 0; i < rows - 1; i++){
		for(int j = 0; j < cols; j++){
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
//draw tris of surface
//rows is num of vertex rows
//cols is num of vertex cols
void drawSurfaceTris(int rows, int cols){
	const int numTris = (rows - 1) * (cols) * 2;
	tris = vector<tri*>(numTris);
	curvePoint = vector<point*> (rows);
	surface = vector< vector<point*> >(rows, vector<point*>(cols));
	//printf("\nerror not drawingSurface\n");
	drawCubicBezier(ptX, ptY, rows);
	surfaceOfRotation(rows, cols);
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
	size = (ySteps - 1) * (thetaSteps) * 2;
	int numElems = 3 + 3 + 2;
	//int numAttribs = 2;
	GLfloat *vp = new GLfloat[size * 3 * (numElems)];    // array of vertex points and normals
	//GLfloat *vn = new GLfloat[size * 9 * numAttribs];    // array of vertex normals
	for(int i = 0; i < size; i++){
		for(int j = 0; j < 3; j++){
			vp[i * 24 + j * 8] = tris[i]->v[j]->x;
			vp[i * 24 + j * 8 + 1] = tris[i]->v[j]->y;
			vp[i * 24 + j * 8 + 2] = tris[i]->v[j]->z;
			vp[i * 24 + j * 8 + 3] = tris[i]->v[j]->norm[0];
			vp[i * 24 + j * 8 + 4] = tris[i]->v[j]->norm[1];
			vp[i * 24 + j * 8 + 5] = tris[i]->v[j]->norm[2];
			vp[i * 24 + j * 8 + 6] = tris[i]->v[j]->u;
			vp[i * 24 + j * 8 + 7] = tris[i]->v[j]->v;
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
	glBufferData(GL_ARRAY_BUFFER, size * 3 * (3 + 3 + 2) * sizeof (float), vp, GL_STATIC_DRAW);
	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (numElems) * 
	sizeof(GLfloat), NULL);

	//normals
	// Tell OpenGL how to locate the second attribute (texture coordinates) inside the buffer
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (numElems) * 
	sizeof(GLfloat),(void*)(3 * sizeof(GLfloat)));
	
	// VBO -- normals -- needed for shading calcuations
	// ADD CODE TO POPULATE AND LOAD PER-VERTEX SURFACE NORMALS  
	// [HINT] Vertex normals are organized in same order as that for vertex coordinates

    // VBO -- vt -- texture coordinates
	// ADD CODE TO POPULATE AND LOAD PER-VERTEX TEXTURE COORDINATES  
	// [HINT] texture coordinates are organized in same order as that for vertex coordinates
	// [HINT] there are two texture coordinates instead of three vertex coordinates for each vertex
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (numElems) * 
	sizeof(GLfloat),(void*)(6 * sizeof(GLfloat)));
}


	
void loadUniforms(GLuint shader_programme)
{	
/*---------------------------SET RENDERING DEFAULTS---------------------------*/

	// Choose vertex and fragment shaders to use as well as view and proj matrices.
	int model_mat_location = glGetUniformLocation (shader_programme, "model_mat");
	int view_mat_location  = glGetUniformLocation (shader_programme, "view_mat");
	int proj_mat_location  = glGetUniformLocation (shader_programme, "proj_mat");
	
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat.m);
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
	glDrawArrays(GL_TRIANGLES, 0, (ySteps - 1) * (thetaSteps) * 2 * 3 * (3 + 3 + 2));
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
