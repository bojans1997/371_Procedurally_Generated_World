#pragma once

#ifndef AXIS_H
#define AXIS_H

#include <glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"

class Axis
{
public:
	unsigned int ID;

	Axis(int size);
	~Axis();
	void draw(Shader *shader);
protected:
	int size;
	unsigned int VAO, VBO;
};
#endif