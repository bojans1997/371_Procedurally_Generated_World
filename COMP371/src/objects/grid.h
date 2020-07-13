#pragma once

#ifndef GRID_H
#define GRID_H

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"

class Grid
{
public:
	unsigned int ID;

	Grid(int size);
	~Grid();
	void draw(Shader *shader);
protected:
	int size;
	GLuint VAO = 0, VBO = 0;
};
#endif