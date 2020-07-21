#pragma once

#ifndef STACK_H
#define STACK_H

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
#include "Sector.h"

class Stack {
	//position
public:

	Stack();
	void draw();
	~Stack();

protected:
	float stackAngle1, stackAngle2;
	int sectorCount;
	float sectorAngle1, sectorAngle2;
	std::vector<Sector*> sectors;
};
#endif