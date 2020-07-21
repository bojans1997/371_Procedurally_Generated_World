#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "../stb_image.h"
#include "shader.h"
#include "texture.h"
#include "Stack.h"

class Sphere {
	//position
public:

	Sphere(float x, float y, float z, float r, int sC, int rC);
	void draw();
	~Sphere();

protected:
	float x, y, z;
	float radius;
	float stackStep;
	int stackCount, sectorCount;
	std::vector<Stack*> stacks;
};
#endif