#pragma once
#ifndef CUBE_H
#define CUBE_H

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "../stb_image.h"
#include "shader.h"
#include "texture.h"

class Cube
{
public:
	Cube(int x, int y, int z);
	~Cube();
	void draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix);
	void draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix, Texture *texture);
protected:
	int x, y, z;
	GLuint VAO = 0, VBO = 0;
};
#endif