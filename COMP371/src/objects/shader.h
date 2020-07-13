#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad.h>
#include <glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
#endif