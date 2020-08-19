#pragma once
#ifndef APPLE_H
#define APPLE_H

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

class Apple
{
public:
	Apple(glm::vec3 position, glm::vec3 size);
	~Apple();
	glm::vec3 size;
	glm::vec3 position;
	void draw(Shader *shader);
	void moveApple(Shader *shader, glm::vec3 vector);
protected:
	// Referenced from https://www.dreamstime.com/light-red-marble-background-texture-light-red-canvas-marble-background-texture-image151066971
	Texture *appleTexture = new Texture("src/textures/apple.jpg");
	Cube* apple;
};
#endif