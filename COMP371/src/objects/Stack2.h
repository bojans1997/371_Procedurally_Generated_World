#pragma once

#ifndef STACK_H
#define STACK_H
#define _USE_MATH_DEFINES

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "../stb_image.h"
#include "shader.h"
#include "texture.h"
#include "Sector.h"

class Stack {

public:

	Stack(float x, float y, float z, float sA1, float sA2, float radius, int sectorCount);
	void draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix);
	~Stack();

protected:
	float stackAngle1, stackAngle2;
	float radius;
	int sectorCount;
	float sectorAngle1, sectorAngle2;
	std::vector<Sector*> sectors;
};
#endif
