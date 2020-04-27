#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/glm.hpp>

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

class SpriteRenderer
{
    public:
        SpriteRenderer(/*Shader &shader*/){
            initRenderData();
        }
        ~SpriteRenderer();

        void DrawSprite(/*Texture2D &texture, */glm::vec2 position, 
            glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, 
            glm::vec3 color = glm::vec3(1.0f));
    private:
        //Shader       shader; 
        unsigned int quadVAO;

        void initRenderData();
};

#endif