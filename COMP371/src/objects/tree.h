#pragma once
#ifndef TREE_H
#define TREE_H

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

class Tree
{
public:
	Tree(glm::vec3 position, int size);
	~Tree();
	void draw(Shader *shader);
	glm::vec3 position;
	int size;
protected:
	// Referenced from https://freestocktextures.com/texture/bark-nature-wood,35.html
	Texture *barkTexture = new Texture("src/textures/bark.jpg");
	// Referenced from https://freestocktextures.com/texture/nature-bush-green,688.html
	Texture *leavesTexture = new Texture("src/textures/leaves.jpg");
	Cube* bark;
	Cube* leaves;
};
#endif