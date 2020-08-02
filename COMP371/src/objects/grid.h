#pragma once

#ifndef GRID_H
#define GRID_H

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "../stb_image.h"
#include "shader.h"
#include "texture.h"

class Grid
{
public:
	Grid(float size);
	~Grid();
	void draw(Shader *shader);
	void draw(Shader *shader, Texture *texture, GLuint depthMap);
protected:
	float size;
	GLuint VAO = 0, VBO = 0;
};
#endif