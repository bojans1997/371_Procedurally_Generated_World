#include "character.h"

Character::Character(std::vector<Cube*> cubes) : cubes(cubes) {}

Character::~Character()
{
	for (std::vector<Cube*>::iterator it = cubes.begin(); it != cubes.end(); ++it) {
		delete *it;
	}
}

void Character::draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix)
{
	for (std::vector<Cube*>::iterator it = cubes.begin(); it != cubes.end(); ++it) {
		(*it)->draw(shader, modelRenderMode, matrix);
	}
}

void Character::draw(Shader *shader, GLuint modelRenderMode, glm::mat4 matrix, Texture *texture)
{
	for (std::vector<Cube*>::iterator it = cubes.begin(); it != cubes.end(); ++it) {
		(*it)->draw(shader, modelRenderMode, matrix, texture);
	}
}