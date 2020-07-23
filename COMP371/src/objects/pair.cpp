#include "pair.h"

Pair::Pair(Character *letter, Character *digit, Sphere *sphere) : letter(letter), digit(digit), sphere(sphere) {}

Pair::~Pair()
{
	delete letter;
	delete digit;
	delete sphere;
}


void Pair::draw(Shader *shader, Shader *sphereShader, GLuint modelRenderMode, glm::mat4 matrix)
{
	letter->draw(shader, modelRenderMode, matrix);
	digit->draw(shader, modelRenderMode, matrix);
	sphere->draw(sphereShader, modelRenderMode, matrix);
}

void Pair::draw(Shader *shader, Shader *sphereShader, GLuint modelRenderMode, glm::mat4 matrix, Texture *letterTexture, Texture *digitTexture)
{
	shader->setVec3("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	shader->setVec3("material.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
	shader->setVec3("material.specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));
	shader->setFloat("material.shininess", 0.3f);
	letter->draw(shader, modelRenderMode, matrix, letterTexture);
	shader->setVec3("material.ambient", glm::vec3(0.24725f, 0.1995f, 0.0745f));
	shader->setVec3("material.diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
	shader->setVec3("material.specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
	shader->setFloat("material.shininess", 51.2f);
	digit->draw(shader, modelRenderMode, matrix, digitTexture);
	sphere->draw(sphereShader, modelRenderMode, matrix);
}