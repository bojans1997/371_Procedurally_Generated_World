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

	Cube();
	~Cube();
	void draw(Shader *shader, unsigned int modelRenderMode);
protected:
	unsigned int VAO, VBO;
};
#endif