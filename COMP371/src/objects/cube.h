#pragma once

#ifndef CUBE_H
#define CUBE_H

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"

class Cube
{
public:
	unsigned int ID;

	Cube(int x, int y, int z);
	~Cube();
	void draw(Shader *shader, unsigned int modelRenderMode);
	void draw(Shader* shader, unsigned int modelRenderMode, glm::mat4 matrix);
protected:
	int x, y, z;
	unsigned int VAO, VBO;
};
#endif