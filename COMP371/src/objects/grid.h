#pragma once

#ifndef GRID_H
#define GRID_H

#include <glad.h>
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
	void draw();
protected:
	int size;
	unsigned int VAO, VBO;
	Shader *shader;
};
#endif