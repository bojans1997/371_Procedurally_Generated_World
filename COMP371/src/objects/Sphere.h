#pragma once

#ifndef SPHERE_H
#define SPHERE_H
#define _USE_MATH_DEFINES

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>
#include <cmath>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "../stb_image.h"
#include "shader.h"
#include "texture.h"
#include "stack.h"

class Sphere
{
public:
	Sphere(float x, float y, float z, float radius, int sC, int rC);
	void draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix);
	~Sphere();
protected:
	float x, y, z;
	float radius;
	int stackCount, sectorCount;
	std::vector<Stack*> stacks;
};
#endif