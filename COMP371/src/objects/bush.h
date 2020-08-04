#pragma once
#ifndef BUSH_H
#define BUSH_H

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include "../stb_image.h"
#include "shader.h"
#include "texture.h"
#include "cube.h"

class Bush
{
public:
	Bush(glm::vec3 position, glm::vec3 size);
	~Bush();
	void draw(Shader *shader);
protected:
	// Referenced from https://freestocktextures.com/texture/shrub-green-nature,767.html
	Texture *bushTexture = new Texture("src/textures/bush.jpg");
	glm::vec3 position;
	glm::vec3 size;
	Cube* bush;
};
#endif