#pragma once

#ifndef PAIR_H
#define PAIR_H

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
#include "character.h"
#include "Sphere.h"

class Pair
{
public:
	Pair(Character *letter, Sphere *sphere, Character *digit);
	~Pair();
	void draw(Shader *shader, Shader *sphereShader, GLuint modelRenderMode, glm::mat4 matrix);
	void draw(Shader *shader, Shader *sphereShader, GLuint modelRenderMode, glm::mat4 matrix, Texture *letterTexture, Texture *digitTexture);
protected:
	Character *letter;
	Sphere *sphere;
	Character *digit;
};
#endif