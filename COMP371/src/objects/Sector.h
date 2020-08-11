#pragma once

#ifndef SECTOR_H
#define SECTOR_H

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "../stb_image.h"
#include "shader.h"
#include "texture.h"

class Sector
{
public:
	Sector(float x, float y, float z, float sA1, float sA2, float r, float s1, float s2);
	void draw(Shader *shader, glm::mat4 matrix);
	~Sector();
protected:
	float x, y, z;
	glm::vec3 normal;
	float stackAngle1, stackAngle2, sectorAngle1, sectorAngle2;
	float radius;
	GLuint VAO = 0, VBO = 0;
};
#endif