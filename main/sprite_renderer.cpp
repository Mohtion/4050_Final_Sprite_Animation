#include "sprite_renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdio.h>
using namespace std;

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
	float numFrames = 100;
	//float halfwidth = width / 2;
	//float halfheight = height / 2;
	surface[0][0] = new point();
	surface[0][1] = new point();
	surface[1][0] = new point();
	surface[1][1] = new point();
	surface[0][0]->xyz[0] = width;
	surface[0][0]->xyz[1] = height;
	surface[0][0]->xyz[2] = 0;
	surface[0][0]->uv[0] = 1.0 / numFrames;
	surface[0][0]->uv[1] = 1.0 / numFrames;
	surface[0][1]->xyz[0] = 0;
	surface[0][1]->xyz[1] = height;
	surface[0][1]->xyz[2] = 0;
	surface[0][1]->uv[0] = 0;
	surface[0][1]->uv[1] = 1.0 / numFrames;
	surface[1][0]->xyz[0] = width;
	surface[1][0]->xyz[1] = 0;
	surface[1][0]->xyz[2] = 0;
	surface[1][0]->uv[0] = 1.0 / numFrames;
	surface[1][0]->uv[1] = 0;
	surface[1][1]->xyz[0] = 0;
	surface[1][1]->xyz[1] = 0;
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
	float width = 1;
	float height = 1;
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

void SpriteRenderer::initRenderData()
{
    drawSurfaceTris(ySteps, thetaSteps);
	printf("did we make it here\n");
	size = 2;
	int numElems = 3 + 2;
	//int numAttribs = 2;
	GLfloat *vp = new GLfloat[size * 3 * (numElems)];    // array of vertex points and normals
	for(int i = 0; i < size; i++){
		for(int j = 0; j < 3; j++){
			vp[i * 15 + j * 5] = tris[i]->v[j]->xyz[0];
			vp[i * 15 + j * 5 + 1] = tris[i]->v[j]->xyz[1];
			vp[i * 15 + j * 5 + 2] = tris[i]->v[j]->xyz[2];
			vp[i * 15 + j * 5 + 3] = tris[i]->v[j]->uv[0];
			vp[i * 15 + j * 5 + 4] = tris[i]->v[j]->uv[1];
			printf("%f %f %f :: %f %f \n", 
                vp[i * 15 + j * 5], 
                vp[i * 15 + j * 5 + 1], 
                vp[i * 15 + j * 5 + 2], 
                vp[i * 15 + j * 5 + 3], 
                vp[i * 15 + j * 5 + 4]);
		}
	}
	printf("did we make it there\n");
	
	// VAO -- vertex attribute objects bundle the various things associated with vertices
	GLuint vao;
	glGenVertexArrays (1, &vao);   // generating and binding is common pattern in OpenGL
	glBindVertexArray (vao);       // basically setting up memory and associating it

	// VBO -- vertex buffer object to contain coordinates
	// MODIFY THE FOLLOWING BLOCK OF CODE APPRORIATELY FOR YOUR SURFACE OF REVOLUTION
	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * (numElems) * sizeof (float), vp, GL_STATIC_DRAW);
	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (numElems) * 
	sizeof(GLfloat), NULL);

    // VBO -- vt -- texture coordinates
	// ADD CODE TO POPULATE AND LOAD PER-VERTEX TEXTURE COORDINATES  
	// [HINT] texture coordinates are organized in same order as that for vertex coordinates
	// [HINT] there are two texture coordinates instead of three vertex coordinates for each vertex
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (numElems) * 
	sizeof(GLfloat),(void*)(3 * sizeof(GLfloat)));
}

void SpriteRenderer::DrawSprite(/*Texture2D &texture, */glm::vec2 position, 
  glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    extern glm::mat4 model_mat;
    model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(position, 0.0f));  

    model_mat = glm::translate(model_mat, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    model_mat = glm::rotate(model_mat, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); 
    model_mat = glm::translate(model_mat, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model_mat = glm::scale(model_mat, glm::vec3(size, 1.0f)); 
  
    //glActiveTexture(GL_TEXTURE0);
    //texture.Bind();

    glDrawArrays(GL_TRIANGLES, 0, 2 * 3 * (3 + 2));
} 

