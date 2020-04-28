
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
#include <iostream>

#include <math.h>
#include <time.h>

#include "maths_funcs.h"   // Anton's maths functions.
#include "gl_utils.h"      // Anton's opengl functions and small utilities like logs
#include "stb_image.h"     // Sean Barrett's image loader with Anton's load_texture()

#define _USE_MATH_DEFINES
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

mat4 view_mat;
mat4 proj_mat;
mat4 model_mat;

GLfloat tex1_vp[18], tex2_vp[18], tex3_vp[18], tex4_vp[18];

GLuint texture1_vbo;

int pointCount;

int count = 0;

void loadSurfaceOfRevolution()
{
/*------------------------------CREATE GEOMETRY-------------------------------*/
   GLfloat vp[18];

   vp[0] = -1;
   vp[1] = -1;
   vp[2] = 0;

   vp[3] = 1;
   vp[4] = -1;
   vp[5] = 0;

   vp[6] = -1;
   vp[7] = 1;
   vp[8] = 0;

   vp[9] = -1;
   vp[10] = 1;
   vp[11] = 0;

   vp[12] = 1;
   vp[13] = -1;
   vp[14] = 0;

   vp[15] = 1;
   vp[16] = 1;
   vp[17] = 0;


   GLfloat norm_vp[18];

   for(int n = 0; n < 18; n+=3)
   {
      norm_vp[n] = vp[n];
      norm_vp[n+1] = vp[n+1];
      norm_vp[n+2] = vp[n+2];
   }

   tex1_vp[0] = 0;
   tex1_vp[1] = 0;

   tex1_vp[3] = .25;
   tex1_vp[4] = 0;

   tex1_vp[6] = 0;
   tex1_vp[7] = 1;

   tex1_vp[9] = 0;
   tex1_vp[10] = 1;

   tex1_vp[12] = .25;
   tex1_vp[13] = 0;

   tex1_vp[15] = .25;
   tex1_vp[16] = 1;



   tex2_vp[0] = .25;
   tex2_vp[1] = 0;

   tex2_vp[3] = .5;
   tex2_vp[4] = 0;

   tex2_vp[6] = .25;
   tex2_vp[7] = 1;

   tex2_vp[9] = .25;
   tex2_vp[10] = 1;

   tex2_vp[12] = .5;
   tex2_vp[13] = 0;

   tex2_vp[15] = .5;
   tex2_vp[16] = 1;



   tex3_vp[0] = .5;
   tex3_vp[1] = 0;

   tex3_vp[3] = .75;
   tex3_vp[4] = 0;

   tex3_vp[6] = .5;
   tex3_vp[7] = 1;

   tex3_vp[9] = .5;
   tex3_vp[10] = 1;

   tex3_vp[12] = .75;
   tex3_vp[13] = 0;

   tex3_vp[15] = .75;
   tex3_vp[16] = 1;



   tex4_vp[0] = .75;
   tex4_vp[1] = 0;

   tex4_vp[3] = 1;
   tex4_vp[4] = 0;

   tex4_vp[6] = .75;
   tex4_vp[7] = 1;

   tex4_vp[9] = .75;
   tex4_vp[10] = 1;

   tex4_vp[12] = 1;
   tex4_vp[13] = 0;

   tex4_vp[15] = 1;
   tex4_vp[16] = 1;


   // VAO -- vertex attribute objects bundle the various things associated with vertices
   GLuint vao;
   glGenVertexArrays (1, &vao);
   glBindVertexArray (vao);

   // VBO -- vertex buffer object to contain coordinates
   GLuint points_vbo;
   glGenBuffers(1, &points_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
   glBufferData(GL_ARRAY_BUFFER, 18 * sizeof (GLfloat), vp, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(0);


   // VBO -- normals -- needed for shading calcuations
   GLuint normals_vbo;
   glGenBuffers(1, &normals_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
   glBufferData(GL_ARRAY_BUFFER, 18 * sizeof (GLfloat), norm_vp, GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(1);


   // VBO -- vt -- texture coordinates
   glGenBuffers(1, &texture1_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, texture1_vbo);
   glBufferData(GL_ARRAY_BUFFER, 18 * sizeof (GLfloat), tex1_vp, GL_STATIC_DRAW);
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(2);
}



void loadUniforms(GLuint shader_programme)
{

   int model_mat_location = glGetUniformLocation (shader_programme, "model_mat");
   int view_mat_location  = glGetUniformLocation (shader_programme, "view_mat");
   int proj_mat_location  = glGetUniformLocation (shader_programme, "proj_mat");

   glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
   glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat.m);
   glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, model_mat.m);

}


void drawSurfaceOfRevolution()
{
   glDrawArrays(GL_TRIANGLES, 0, 18);
}


void keyboardFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE))
   {
      // Close window when esacape is pressed
      glfwSetWindowShouldClose (g_window, 1);
   }
}

void updateSprite()
{

   if(count == 0)
   {
      glBindBuffer(GL_ARRAY_BUFFER, texture1_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof (GLfloat), tex2_vp);
      count++;
   }
   else if(count == 1)
   {
      glBindBuffer(GL_ARRAY_BUFFER, texture1_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof (GLfloat), tex3_vp);
      count++;
   }
   else if(count == 2)
   {
      glBindBuffer(GL_ARRAY_BUFFER, texture1_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof (GLfloat), tex4_vp);
      count++;
   }
   else
   {
      glBindBuffer(GL_ARRAY_BUFFER, texture1_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof (GLfloat), tex1_vp);
      count = 0;
   }
}


