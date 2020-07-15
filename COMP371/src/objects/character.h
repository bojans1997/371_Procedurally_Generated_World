#pragma once

#ifndef CHARACTER_H
#define CHARACTER_H

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
#include "cube.h"

class Character
{
public:
	Character(std::vector<Cube*> cubes);
	~Character();
	void draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix);
	void draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix, Texture *texture);
protected:
	std::vector<Cube*> cubes;
};
#endif