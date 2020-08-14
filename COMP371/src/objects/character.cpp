#include "character.h"

Character::Character(std::vector<Cube*> cubes) : cubes(cubes) {}

Character::~Character()
{
	for (std::vector<Cube*>::iterator it = cubes.begin(); it != cubes.end(); ++it) {
		delete *it;
	}
}

std::vector<Cube*> Character::getCubes() {
	return cubes;
}

void Character::draw(Shader *shader, glm::mat4 matrix)
{
	for (std::vector<Cube*>::iterator it = cubes.begin(); it != cubes.end(); ++it) {
		(*it)->draw2(shader, matrix);
	}
}

void Character::draw(Shader *shader, glm::mat4 matrix, Texture *texture)
{
	for (std::vector<Cube*>::iterator it = cubes.begin(); it != cubes.end(); ++it) {
		(*it)->draw2(shader, matrix, texture);
	}
}