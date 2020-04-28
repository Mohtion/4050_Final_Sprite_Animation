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
#include "sprite_renderer.h"
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

SpriteRenderer  *Renderer;

void loadSurfaceOfRevolution() 
{
	Renderer = new SpriteRenderer();
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
	Renderer->DrawSprite(glm::vec2(0 + user_light[0], 0 + user_light[1]), glm::vec2(200.0f, 200.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
}
	
void keyboardFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// MODIFY THIS FUNCTION FOR KEYBOARD INTERACTIVITY
	//GLFW Reference Links:
	// Callback Example: https://www.glfw.org/docs/3.3/input_guide.html#input_key
	// List of Keys: https://www.glfw.org/docs/3.3/group__keys.html
	
	//move light
	float step = 10;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
		printf("\nLight -x \n");
		user_light[0] -= step;
        // Example case. Key 'E' pressed. Doing nothing
	}

    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
		printf("\nLight -y\n");
		user_light[1] -= step;
        // Example case. Key 'E' pressed. Doing nothing
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
		printf("\nLight +x\n");
		user_light[0] += step;
        // Example case. Key 'E' pressed. Doing nothing
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
		printf("\nLight +y\n");
		user_light[1] += step;
        // Example case. Key 'E' pressed. Doing nothing
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
		printf("\nLight -z\n");
		user_light[2] -= step;
        // Example case. Key 'E' pressed. Doing nothing
	}
	else if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
		printf("\nLight +z\n");
		user_light[2] += step;
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
