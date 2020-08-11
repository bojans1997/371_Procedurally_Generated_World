#include "pair.h"

Pair::Pair(Character *letter, Character *digit, Sphere *sphere) : letter(letter), digit(digit), sphere(sphere) {}

Pair::~Pair()
{
	delete letter;
	delete digit;
	delete sphere;
}

void Pair::draw(Shader *shader, Shader *sphereShader, glm::mat4 matrix)
{
	letter->draw(shader, matrix);
	digit->draw(shader, matrix);
	sphere->draw(sphereShader, matrix);
}

void Pair::draw(Shader *textureShader, Shader *sphereShader, glm::mat4 matrix, Texture *texture)
{
	textureShader->setVec3("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	textureShader->setVec3("material.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
	textureShader->setVec3("material.specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));
	textureShader->setFloat("material.shininess", 0.3f);
	letter->draw(textureShader, matrix, texture);
	digit->draw(textureShader, matrix, texture);
	sphere->draw(sphereShader, matrix);
}