#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "../stb_image.h"
#include "shader.h"

class Texture
{
public:
	Texture(std::string texturePath);
	~Texture();
	GLuint getTextureId();
protected:
	GLuint textureId;
};
#endif